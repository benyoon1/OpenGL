#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

class Window
{
public:
    GLFWwindow *m_glWindow;

    Window(const unsigned int m_uiWidth, const unsigned int m_uiHeight, Camera &camera);

    void ProcessInput();
    void UpdateFrame();
    unsigned int GetWidth() const { return m_uiWidth; }
    unsigned int GetHeight() const { return m_uiHeight; }

private:
    unsigned int m_uiWidth;
    unsigned int m_uiHeight;
    float m_fLastX;
    float m_fLastY;
    float m_fDeltaTime;
    float m_CurrentFrame;
    float m_fLastFrame;
    bool m_bFirstMouse;
    Camera &m_camera; // Add a reference to the Camera object

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif // WINDOW_H