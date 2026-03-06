#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "BezierCurve.h"
#include "BezierSurface.h"
#include "Mesh.h"
#include "Renderer.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int curveSegments = 20;
bool segmentKeyPressed = false;

int surfaceMode = 1; // 1 = shaded, 2 = normals, 3 = uv
bool modeKeyPressed = false;

int surfaceUSegments = 20;
int surfaceVSegments = 20;
bool surfaceResolutionKeyPressed = false;

bool wireframeMode = false;
bool wireframeKeyPressed = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    (void)window;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window;
    (void)xoffset;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    bool upPressed = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool downPressed = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;

    if (!segmentKeyPressed) {
        if (upPressed) {
            curveSegments++;
            if (curveSegments > 200) {
                curveSegments = 200;
            }
            std::cout << "Curve segments: " << curveSegments << '\n';
            segmentKeyPressed = true;
        } else if (downPressed) {
            curveSegments--;
            if (curveSegments < 1) {
                curveSegments = 1;
            }
            std::cout << "Curve segments: " << curveSegments << '\n';
            segmentKeyPressed = true;
        }
    }

    if (!upPressed && !downPressed) {
        segmentKeyPressed = false;
    }

    bool onePressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    bool twoPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
    bool threePressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;

    if (!modeKeyPressed) {
        if (onePressed) {
            surfaceMode = 1;
            std::cout << "Surface mode: shaded\n";
            modeKeyPressed = true;
        } else if (twoPressed) {
            surfaceMode = 2;
            std::cout << "Surface mode: normals\n";
            modeKeyPressed = true;
        } else if (threePressed) {
            surfaceMode = 3;
            std::cout << "Surface mode: uv\n";
            modeKeyPressed = true;
        }
    }

    if (!onePressed && !twoPressed && !threePressed) {
        modeKeyPressed = false;
    }

    bool leftBracketPressed = glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS;
    bool rightBracketPressed = glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS;

    if (!surfaceResolutionKeyPressed) {
        if (leftBracketPressed) {
            surfaceUSegments--;
            surfaceVSegments--;
            if (surfaceUSegments < 2) surfaceUSegments = 2;
            if (surfaceVSegments < 2) surfaceVSegments = 2;
            std::cout << "Surface resolution: " << surfaceUSegments << " x " << surfaceVSegments << '\n';
            surfaceResolutionKeyPressed = true;
        } else if (rightBracketPressed) {
            surfaceUSegments++;
            surfaceVSegments++;
            if (surfaceUSegments > 80) surfaceUSegments = 80;
            if (surfaceVSegments > 80) surfaceVSegments = 80;
            std::cout << "Surface resolution: " << surfaceUSegments << " x " << surfaceVSegments << '\n';
            surfaceResolutionKeyPressed = true;
        }
    }

    if (!leftBracketPressed && !rightBracketPressed) {
        surfaceResolutionKeyPressed = false;
    }

    bool fPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
    if (!wireframeKeyPressed && fPressed) {
        wireframeMode = !wireframeMode;
        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            std::cout << "Wireframe: ON\n";
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            std::cout << "Wireframe: OFF\n";
        }
        wireframeKeyPressed = true;
    }

    if (!fPressed) {
        wireframeKeyPressed = false;
    }
}

std::vector<float> makeCubeVertices() {
    return {
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f
    };
}

std::vector<float> flattenVec3(const std::vector<glm::vec3>& points) {
    std::vector<float> data;
    data.reserve(points.size() * 3);

    for (const auto& p : points) {
        data.push_back(p.x);
        data.push_back(p.y);
        data.push_back(p.z);
    }

    return data;
}

int main() {
    std::vector<glm::vec3> controlPoints = {
        glm::vec3(-2.5f, -1.5f, 0.0f),
        glm::vec3(-1.0f,  1.5f, 0.0f),
        glm::vec3( 1.0f, -1.5f, 0.0f),
        glm::vec3( 2.5f,  1.5f, 0.0f)
    };

    std::vector<std::vector<glm::vec3>> surfaceControlGrid = {
        {
            glm::vec3(-1.5f, 0.0f, -1.5f),
            glm::vec3(-0.5f, 1.2f, -1.5f),
            glm::vec3( 0.5f, 1.2f, -1.5f),
            glm::vec3( 1.5f, 0.0f, -1.5f)
        },
        {
            glm::vec3(-1.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 2.0f, -0.5f),
            glm::vec3( 0.5f, 2.0f, -0.5f),
            glm::vec3( 1.5f, 0.5f, -0.5f)
        },
        {
            glm::vec3(-1.5f, 0.5f,  0.5f),
            glm::vec3(-0.5f, 2.0f,  0.5f),
            glm::vec3( 0.5f, 2.0f,  0.5f),
            glm::vec3( 1.5f, 0.5f,  0.5f)
        },
        {
            glm::vec3(-1.5f, 0.0f,  1.5f),
            glm::vec3(-0.5f, 1.2f,  1.5f),
            glm::vec3( 0.5f, 1.2f,  1.5f),
            glm::vec3( 1.5f, 0.0f,  1.5f)
        }
    };

    BezierCurve bezier(controlPoints);
    BezierSurface bezierSurface(surfaceControlGrid);
    Renderer renderer;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RenderingGeometryProject", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    std::cout << "__________________ Controls __________________\n";
    std::cout << "W A S D  : Move camera\n";
    std::cout << "Mouse    : Look around\n";
    std::cout << "Scroll   : Zoom\n";
    std::cout << "UP/DOWN  : Change Bezier curve segments\n";
    std::cout << "[ / ]    : Change Bezier surface resolution\n";
    std::cout << "1 / 2 / 3: Surface visualization modes\n";
    std::cout << "F        : Toggle wireframe mode\n";
    std::cout << "ESC      : Exit\n";
    std::cout << "______________________________________________\n";
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader phongShader("../shaders/phong.vert", "../shaders/phong.frag");
    Shader lineShader("../shaders/line.vert", "../shaders/line.frag");
    Shader surfaceShader("../shaders/surface.vert", "../shaders/surface.frag");

    if (phongShader.ID == 0 || lineShader.ID == 0 || surfaceShader.ID == 0) {
        std::cerr << "Failed to load shader program(s)\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    Mesh cubeMesh;
    std::vector<float> cubeVertices = makeCubeVertices();
    cubeMesh.upload(cubeVertices, 6);
    cubeMesh.setAttribute(0, 3, 6, 0);
    cubeMesh.setAttribute(1, 3, 6, 3);
    cubeMesh.setDrawMode(GL_TRIANGLES);

    Mesh controlLineMesh;
    std::vector<float> controlLineData = flattenVec3(controlPoints);
    controlLineMesh.upload(controlLineData, 3);
    controlLineMesh.setAttribute(0, 3, 3, 0);
    controlLineMesh.setDrawMode(GL_LINE_STRIP);

    Mesh controlPointMesh;
    controlPointMesh.upload(controlLineData, 3);
    controlPointMesh.setAttribute(0, 3, 3, 0);
    controlPointMesh.setDrawMode(GL_POINTS);

    Mesh curveMesh;
    curveMesh.setDrawMode(GL_LINE_STRIP);

    Mesh surfaceMesh;
    std::vector<float> surfaceData = bezierSurface.sampleUniformInterleaved(surfaceUSegments, surfaceVSegments);
    surfaceMesh.upload(surfaceData, 8);
    surfaceMesh.setAttribute(0, 3, 8, 0);
    surfaceMesh.setAttribute(1, 3, 8, 3);
    surfaceMesh.setAttribute(2, 2, 8, 6);
    surfaceMesh.setDrawMode(GL_TRIANGLES);

    glm::vec3 lightPos(2.5f, 3.0f, 3.0f);
    int previousSegments = -1;
    int previousUSegments = -1;
    int previousVSegments = -1;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (curveSegments != previousSegments) {
            std::vector<glm::vec3> sampledCurve = bezier.sampleUniform(curveSegments);
            std::vector<float> curveData = flattenVec3(sampledCurve);
            curveMesh.upload(curveData, 3, GL_DYNAMIC_DRAW);
            curveMesh.setAttribute(0, 3, 3, 0);
            previousSegments = curveSegments;
        }

        if (surfaceUSegments != previousUSegments || surfaceVSegments != previousVSegments) {
            std::vector<float> newSurfaceData =
                bezierSurface.sampleUniformInterleaved(surfaceUSegments, surfaceVSegments);
            surfaceMesh.upload(newSurfaceData, 8, GL_DYNAMIC_DRAW);
            surfaceMesh.setAttribute(0, 3, 8, 0);
            surfaceMesh.setAttribute(1, 3, 8, 3);
            surfaceMesh.setAttribute(2, 2, 8, 6);

            previousUSegments = surfaceUSegments;
            previousVSegments = surfaceVSegments;
        }

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
            0.1f,
            100.0f
        );

        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(-2.0f, 0.0f, -2.0f));
        cubeModel = glm::rotate(
            cubeModel,
            static_cast<float>(glfwGetTime()) * glm::radians(25.0f),
            glm::vec3(0.5f, 1.0f, 0.0f)
        );

        renderer.drawLitMesh(
            phongShader,
            cubeMesh,
            cubeModel,
            view,
            projection,
            glm::vec3(0.8f, 0.3f, 0.4f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            lightPos,
            camera.Position
        );

        glm::mat4 curveModel = glm::mat4(1.0f);
        curveModel = glm::translate(curveModel, glm::vec3(0.0f, -1.5f, 0.0f));

        renderer.drawLineMesh(
            lineShader,
            controlLineMesh,
            curveModel,
            view,
            projection,
            glm::vec3(0.8f, 0.8f, 0.8f)
        );

        renderer.drawPointMesh(
            lineShader,
            controlPointMesh,
            curveModel,
            view,
            projection,
            glm::vec3(1.0f, 1.0f, 0.2f),
            10.0f
        );

        renderer.drawLineMesh(
            lineShader,
            curveMesh,
            curveModel,
            view,
            projection,
            glm::vec3(0.2f, 0.9f, 1.0f)
        );

        surfaceShader.use();

        glm::mat4 surfaceModel = glm::mat4(1.0f);
        surfaceModel = glm::translate(surfaceModel, glm::vec3(1.6f, -0.8f, 0.0f));

        surfaceShader.setMat4("model", surfaceModel);
        surfaceShader.setMat4("view", view);
        surfaceShader.setMat4("projection", projection);
        surfaceShader.setInt("mode", surfaceMode);
        surfaceShader.setVec3("objectColor", glm::vec3(0.3f, 0.7f, 0.9f));
        surfaceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        surfaceShader.setVec3("lightPos", lightPos);
        surfaceShader.setVec3("viewPos", camera.Position);

        surfaceMesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
