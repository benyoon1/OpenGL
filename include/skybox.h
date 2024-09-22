#ifndef SKYBOX_H
#define SKYBOX_H

#include "cube.h"
#include <vector>
#include <string>

class Skybox : public Cube
{
public:
    unsigned int cubemapTexture;
    unsigned int skyboxVAO;

    Skybox();
    void Draw(Shader &shader, const glm::mat4 &projection,
              glm::mat4 &view, Camera &camera);
    unsigned int LoadCubemap(std::vector<std::string> faces);
};

#endif