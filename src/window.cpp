#include "window.h"
#include <iostream>

Window::Window(const unsigned int width, const unsigned int height, Camera &camera) : m_uiWidth(width),
                                                                                      m_uiHeight(height),
                                                                                      m_camera(camera),
                                                                                      m_fLastX(width / 2.0f),
                                                                                      m_fLastY(height / 2.0f),
                                                                                      m_fDeltaTime(0.0f),
                                                                                      m_CurrentFrame(0.0f),
                                                                                      m_fLastFrame(0.0f),
                                                                                      m_bFirstMouse(true)
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    m_glWindow = glfwCreateWindow(m_uiWidth, m_uiHeight, "OpenGL", NULL, NULL);
    if (m_glWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_glWindow);
    glfwSetWindowUserPointer(m_glWindow, this); // Set the user pointer to the instance

    glfwSetFramebufferSizeCallback(m_glWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_glWindow, mouse_callback);
    glfwSetScrollCallback(m_glWindow, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(m_glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Window::ProcessInput()
{
    if (glfwGetKey(m_glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_glWindow, true);

    // get WASD keys
    if (glfwGetKey(m_glWindow, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.ProcessKeyboard(FORWARD, m_fDeltaTime);
    if (glfwGetKey(m_glWindow, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.ProcessKeyboard(BACKWARD, m_fDeltaTime);
    if (glfwGetKey(m_glWindow, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.ProcessKeyboard(LEFT, m_fDeltaTime);
    if (glfwGetKey(m_glWindow, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.ProcessKeyboard(RIGHT, m_fDeltaTime);
}

void Window::UpdateFrame()
{
    m_CurrentFrame = static_cast<float>(glfwGetTime());
    m_fDeltaTime = m_CurrentFrame - m_fLastFrame;
    m_fLastFrame = m_CurrentFrame;
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    Window *instance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (instance)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (instance->m_bFirstMouse)
        {
            instance->m_fLastX = xpos;
            instance->m_fLastY = ypos;
            instance->m_bFirstMouse = false;
        }

        float xoffset = xpos - instance->m_fLastX;
        float yoffset = instance->m_fLastY - ypos;

        instance->m_fLastX = xpos;
        instance->m_fLastY = ypos;

        instance->m_camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Window *instance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (instance)
    {
        instance->m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}