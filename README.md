# 3D Model Viewer with OpenGL

## Preview

![3D Model Viewer Demo](/assets/gif/valley1.gif)
![3D Model Viewer Demo](/assets/gif/valley2.gif)
![3D Model Viewer Demo](/assets/gif/valley3.gif)

## Prerequisites

### For MacOS

Get homebrew and run `brew install glfw assimp`

### For Linux

Install the dependencies listed below:

```sh
   sudo apt-get update
   sudo apt-get install libglfw3-dev libassimp-dev libgl1-mesa-dev libx11-dev libpthread-stubs0-dev libxrandr-dev libxi-dev
```

## To Run

Run:

```sh
./configure.sh       # generate build files with Ninja via CMake
./build.sh           # build
build/opengl         # run executable
```
