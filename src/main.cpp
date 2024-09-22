#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "window.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "filesystem.h"
#include "cube.h"
#include "skybox.h"

#include <iostream>

int main()
{
    // settings
    const unsigned int SCR_WIDTH = 1920;
    const unsigned int SCR_HEIGHT = 1080;
    const float AMPLITUDE = 600.0f;

    Camera camera(glm::vec3(100.0f, 200.0f, 100.0f)); // camera position
    glm::vec3 lightPos(0.0f, 150.0f, 0.0f);           // lamp position

    Window window(SCR_WIDTH, SCR_HEIGHT, camera); // GLFW window

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader lightCubeShader("shaders/light_cube.vs", "shaders/light_cube.fs");
    Shader modelShader("shaders/model_loading.vs", "shaders/model_loading.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");

    // load objects
    Model ourModel("assets/gm-bigcity/gm_bigcity.obj");
    Cube lampCube;

    stbi_set_flip_vertically_on_load(false); // reset flip before loading skybox
    Skybox skybox;

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // render loop
    while (!glfwWindowShouldClose(window.m_glWindow))
    {
        window.UpdateFrame();
        window.ProcessInput();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Displace lamp along the x-axis
        float timeValue = glfwGetTime();
        float Zdisplacement = sin(timeValue) * AMPLITUDE;
        float Ydisplacement = cos(timeValue) * AMPLITUDE;
        glm::vec3 animatedLightPos = lightPos + glm::vec3(0.0f, Ydisplacement, Zdisplacement);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // draw the objects
        ourModel.Draw(modelShader, projection, view, camera, animatedLightPos);
        lampCube.Draw(lightCubeShader, projection, view, camera, animatedLightPos);
        skybox.Draw(skyboxShader, projection, view, camera); // draw skybox as last

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)s
        glfwSwapBuffers(window.m_glWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
