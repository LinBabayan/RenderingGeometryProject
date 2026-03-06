# Rendering Geometry Project

This project implements parametric geometry rendering using modern OpenGL.  
The application demonstrates the generation and visualization of Bézier curves and Bézier surfaces with real-time rendering and interactive controls.

The goal of the project is to explore parametric curve and surface discretization and visualize the results using the OpenGL graphics pipeline.

---

## Implemented Features

### Bézier Curve

- Implementation of Bézier curves using **Bernstein polynomials**
- **Uniform parametric discretization** based on a user-defined number of segments
- Visualization of:
  - Bézier curve
  - Control polygon
  - Control points

---

### Bézier Surface (Bonus)

- Implementation of a **Bézier surface patch**
- Surface discretization using **uniform sampling in parametric space (u, v)**
- Triangle mesh generation for rendering
- Each vertex stores:
  - **Position**
  - **Normal**
  - **Parametric coordinates (u, v)**

---

## Surface Visualization Modes

The surface can be visualized in three different modes:

1. **Shaded surface rendering**
2. **Surface normals visualization**
3. **Parametric (UV) visualization**

---

## Rendering

The application uses the **modern OpenGL pipeline** and includes:

- Phong lighting model
- Real-time rendering
- Interactive camera movement
- Rendering of a simple 3D mesh (cube)

---

## Interactive Features

- Adjustable Bézier **curve resolution**
- Adjustable Bézier **surface resolution**
- **Wireframe rendering toggle**

---

## Controls

| Key | Function |
|----|----|
| W A S D | Move camera |
| Mouse | Rotate camera |
| Scroll | Zoom |
| UP / DOWN | Change Bézier curve discretization |
| [ / ] | Change Bézier surface resolution |
| 1 | Shaded surface mode |
| 2 | Surface normals visualization |
| 3 | Parametric UV visualization |
| F | Toggle wireframe mode |
| ESC | Exit program |

---

## Project Structure
