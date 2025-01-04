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
    Shader terrainShader("shaders/terrain.vs", "shaders/terrain.fs");

    // load objects
    Model ourModel("assets/gm-bigcity/gm_bigcity.obj");
    Cube lampCube;

    float vertices[] = {
        // positions          // texture coords
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    // Setup VAO, VBO, EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the image
    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/rough-concrete-wall-texture.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    terrainShader.use();
    terrainShader.setInt("texture1", 0);

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

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        terrainShader.use();
        terrainShader.setMat4("projection", projection);
        terrainShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 100.0f, 0.0f));
        model = glm::scale(model, glm::vec3(500.0f));
        terrainShader.setMat4("model", model);

        // Render the plane
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // draw the objects
        // ourModel.Draw(modelShader, projection, view, camera, animatedLightPos);
        lampCube.Draw(lightCubeShader, projection, view, camera, animatedLightPos);
        skybox.Draw(skyboxShader, projection, view, camera); // draw skybox as last

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)s
        glfwSwapBuffers(window.m_glWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
