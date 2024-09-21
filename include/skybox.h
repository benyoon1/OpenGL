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
    void Draw();
    unsigned int LoadCubemap(std::vector<std::string> faces);
};

#endif