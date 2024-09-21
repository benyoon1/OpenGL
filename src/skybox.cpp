#include "skybox.h"
#include "filesystem.h"
#include <stb_image.h>
#include <iostream>

Skybox::Skybox()
{
    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(skyboxVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    stbi_set_flip_vertically_on_load(false);

    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    std::vector<std::string> faces{
        "assets/sunset/px.png",
        "assets/sunset/nx.png",
        "assets/sunset/py.png",
        "assets/sunset/ny.png",
        "assets/sunset/pz.png",
        "assets/sunset/nz.png"};

    cubemapTexture = LoadCubemap(faces);
}

void Skybox::Draw()
{
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int Skybox::LoadCubemap(std::vector<std::string> faces)
{
    std::vector<std::string> facesAbsPath(6);
    for (int i = 0; i < 6; i++)
    {
        facesAbsPath[i] = FileSystem::getPath(faces[i]);
        // std::cout << "facesAbsPath" << facesAbsPath[i] << std::endl;
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < facesAbsPath.size(); i++)
    {
        unsigned char *data = stbi_load(facesAbsPath[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << facesAbsPath[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
