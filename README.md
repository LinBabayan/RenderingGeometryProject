Rendering Geometry Project

This project implements parametric geometry rendering using modern OpenGL. The application demonstrates the generation and visualization of Bézier curves and Bézier surfaces with real-time rendering and interactive controls.

The goal of the project is to explore parametric curve and surface discretization and visualize the results using the OpenGL graphics pipeline.

Implemented Features
Bézier Curve

Implementation of Bézier curves using Bernstein polynomials

Uniform parametric discretization based on a user-defined number of segments

Visualization of the Bézier curve

Visualization of the control polygon

Visualization of control points

Bézier Surface (Bonus)

Implementation of a Bézier surface patch

Surface discretization using uniform sampling in parametric space (u,v)

Triangle mesh generation for surface rendering

Each vertex stores:

position

normal

parametric coordinates (u,v)

Surface Visualization Modes

The surface can be visualized in different modes:

Shaded surface rendering

Surface normals visualization

Parametric (UV) visualization

Rendering

Modern OpenGL rendering pipeline

Phong lighting model

Real-time rendering

Interactive camera movement

Rendering of a simple mesh (cube)

Additional Interactive Features

Adjustable Bézier curve resolution

Adjustable Bézier surface resolution

Wireframe rendering mode

Controls

W A S D – Move camera
Mouse – Rotate camera view
Scroll – Zoom in/out

UP / DOWN – Increase or decrease Bézier curve segments

[ / ] – Increase or decrease Bézier surface resolution

1 – Shaded surface mode
2 – Surface normals visualization
3 – Parametric UV visualization

F – Toggle wireframe rendering

ESC – Exit the program

Project Structure

RenderingGeometryProject

src
  Source files

include
  Header files

shaders
  GLSL shader programs

external
  External dependencies (GLAD)

CMakeLists.txt
  Project build configuration

Compilation
Requirements

C++ compiler
CMake
OpenGL
GLFW
GLM

Install dependencies (Ubuntu)

sudo apt install build-essential cmake libglfw3-dev libglm-dev libgl1-mesa-dev

Build the project

mkdir build
cd build
cmake ..
make

Run the program

./RenderingGeometryProject

Scene Overview

The application renders three main elements in the scene:

• a rotating cube with Phong lighting
• a Bézier curve together with its control polygon and control points
• a Bézier surface patch with multiple visualization modes

The camera can freely navigate the scene to inspect the geometry and rendering results.
