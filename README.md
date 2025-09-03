# 3D Rover Explorer with OpenGL

A simple OpenGL-based 3D rover explorer featuring hierarchical robot arm animation, dynamic skybox, shadow mapping, spotlight and more.

## Preview

![3D Rover Explorer Demo](/assets/gif/valley1.gif)
![3D Rover Explorer Demo](/assets/gif/valley2.gif)
![3D Rover Explorer Demo](/assets/gif/valley3.gif)

## Features

- Robot arm animation
- Dynamic skybox (day/night cycle)
    - Subtle lighting transitions at sunrise/sunset
- Flashlight from robot arm
- Shadow mapping (from the Sun and the flashlight)
- OBJ model loading via Assimp
- Simple camera controls (orbit, pan, zoom)
- Phong lighting
- Cross-platform build with CMake

## Controls

- WASD: move camera
- Mouse drag: pan camera
- Scroll wheel: zoom
- Mouse left click: turn on flashlight
- Left Shift: run / speed boost while moving
- Space: speed up Sun rotation

## Prerequisites:

### MacOS

- Install Homebrew
- Install dependencies:
    ```sh
    brew install glfw assimp
    ```

### Debian/Ubuntu

- Install dependencies:
    ```sh
       sudo apt-get update
       sudo apt-get install libglfw3-dev libassimp-dev libgl1-mesa-dev libx11-dev libpthread-stubs0-dev libxrandr-dev libxi-dev
    ```

## Build & Run

Run from project root:

```sh
./configure.sh       # generate build files via CMake
./build.sh           # build
build/opengl         # run executable
```

## Planned Work

- Collision detection
- Add stars at night
- Make the Sun look more realistic
- Add unit/integration tests
- Integrate Dear ImGui
- Procedural terrain generation
