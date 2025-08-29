// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "core/application.h"

#include <iostream>
// clang-format on

Application::Application() : m_window(), m_camera(), m_sunLight(), m_spotlight()
{
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glEnable(GL_DEPTH_TEST);

    // construct GL-dependent resources AFTER GLAD
    m_robotArm = std::make_unique<RobotArm>("assets/robot_arm/wrist.obj", "assets/robot_arm/lower_arm.obj",
                                            "assets/robot_arm/upper_arm.obj");
    m_window.setCamera(&m_camera);
    m_window.setRobotArm(m_robotArm.get());

    m_valley = std::make_unique<Model>("assets/valley/valley_az_usa.obj");
    m_skybox = std::make_unique<Skybox>();

    m_sunShadow = std::make_unique<ShadowMap>();
    m_spotShadow = std::make_unique<ShadowMap>();

    m_modelShader = std::make_unique<Shader>("shaders/model.vs", "shaders/model.fs");
    m_skyboxShader = std::make_unique<Shader>("shaders/skybox.vs", "shaders/skybox.fs");
    m_depthShader = std::make_unique<Shader>("shaders/shadowMapping.vs", "shaders/shadowMapping.fs");
}

void Application::run()
{
    while (!glfwWindowShouldClose(m_window.getGlfwWindow()))
    {
        // update window and scene objects
        update();

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderDepthPass();
        renderMainPass();

        glfwSwapBuffers(m_window.getGlfwWindow());
        glfwPollEvents();
    }
}

void Application::update()
{
    // reset values every frame
    m_sunSpeed = 0.1f;
    m_spotlightGain = 1.0f;

    m_window.updateFrame();
    m_window.processInput(m_sunSpeed, m_spotlightGain);

    // scene update
    m_sunLight.update(m_sunSpeed);
    m_robotArm->updateWristPose(m_camera);
    m_spotlight.update(*m_robotArm);
}

void Application::renderDepthPass()
{
    // 1. sun depth pass
    m_sunShadow->bind();
    m_valley->drawShadowMap(*m_depthShader, m_sunLight.getLightSpaceMatrix(), m_valley->getModelMatrix());
    m_robotArm->drawShadowMap(*m_depthShader, m_sunLight.getLightSpaceMatrix());
    m_sunShadow->unbind();

    // 2. spotlight depth pass
    m_spotShadow->bind();
    m_valley->drawShadowMap(*m_depthShader, m_spotlight.getSpotLightSpaceMatrix(), m_valley->getModelMatrix());
    m_spotShadow->unbind();
}

void Application::renderMainPass()
{
    int width, height;
    glfwGetFramebufferSize(m_window.getGlfwWindow(), &width, &height); // high DPI bugfix
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = static_cast<float>(width) / static_cast<float>(height > 0 ? height : 1);
    glm::mat4 projection = glm::perspective(glm::radians(m_camera.getZoom()), aspect, 0.1f, 5000.0f);
    glm::mat4 view = m_camera.getViewMatrix();

    // bind shadow maps to texture units
    m_sunShadow->bindTexture(GL_TEXTURE0 + ShadowMap::kSunShadowTextureNum);
    m_spotShadow->bindTexture(GL_TEXTURE0 + ShadowMap::kSpotShadowTextureNum);

    m_valley->configureShader(*m_modelShader, m_camera, m_sunLight, m_spotlight, *m_robotArm, m_spotlightGain);
    m_valley->draw(*m_modelShader, projection, view, m_camera, m_sunLight.getSunPosition(),
                   m_robotArm->getSpotlightPos());

    m_robotArm->configureShader(*m_modelShader);
    m_robotArm->draw(*m_modelShader, projection, m_camera, m_sunLight.getSunPosition(), m_robotArm->getSpotlightPos());

    m_skybox->draw(*m_skyboxShader, projection, m_camera, m_sunLight.getSunDirection(), glm::vec2(width, height));
}
