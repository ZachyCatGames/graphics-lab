#include "engine/eng_RenderBuffer.h"
#include "engine/gl/eng_Texture.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <print>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

#include <engine/eng_Engine.h>
#include <engine/gl/eng_PerspectiveCamera.h>
#include <engine/gl/eng_BlinnPhong.h>
#include <engine/gl/eng_Mesh.h>

#include <engine/png++/png.hpp>

using namespace eng;

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

int main(int argc, char** argv)
{
    char* argv2[] = {
        "program",
        "--rendermode", "opengl",
        "--winwidth", "800",
        "--winheight", "800"
    };
    Engine engine;
    engine.Initialize(7, argv2);

    /* Get render buffer. */
    Handle<RenderBuffer> renderBuffer = engine.GetDisplayRenderBuffer();
    assert(renderBuffer);

    /* Get image dims. */
    size_t winWidth  = renderBuffer->GetWidth();
    size_t winHeight = renderBuffer->GetHeight();

    // Need to set a projection matrix that fits the aspect ratio set
    // by the window frame.
    //
    // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    float aspectRatio    = float(renderBuffer->GetWidth()) / renderBuffer->GetHeight();
    float halfWidth      = 15.0 / 2.0;
    float halfHeight     = halfWidth / aspectRatio;
    constexpr float near = 5.0f;
    constexpr float far  = -5.0f;

    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;

    #if 0
    std::vector<Vertex> vertices {
        { { -3.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  3.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.0f,  5.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 1.0f } }
    };
    #endif

    std::vector<Vertex> vertices {
        // Front face (looking down -Z)
        { { -1.0f, -1.0f, -1.0f }, { 0, 0, -1 }, { 0.0f, 0.5f } }, // v1
        { {  1.0f, -1.0f, -1.0f }, { 0, 0, -1 }, { 0.5f, 0.5f } },
        { {  1.0f,  1.0f, -1.0f }, { 0, 0, -1 }, { 0.5f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f }, { 0, 0, -1 }, { 0.0f, 0.5f } }, // v2
        { {  1.0f,  1.0f, -1.0f }, { 0, 0, -1 }, { 0.5f, 1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { 0, 0, -1 }, { 0.0f, 1.0f } },

        // Right face (facing positive X)
        { {  1.0f, -1.0f, -1.0f }, { 1, 0, 0}, { 0.5f, 0.5f } }, // v1
        { {  1.0f, -1.0f,  1.0f }, { 1, 0, 0}, { 1.0f, 0.5f } },
        { {  1.0f,  1.0f,  1.0f }, { 1, 0, 0}, { 1.0f, 1.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 1, 0, 0}, { 0.5f, 0.5f } }, // v2
        { {  1.0f,  1.0f,  1.0f }, { 1, 0, 0}, { 1.0f, 1.0f } },
        { {  1.0f,  1.0f, -1.0f }, { 1, 0, 0}, { 0.5f, 1.0f } },

        // Back face (facing positive Z)
        { {  1.0f, -1.0f,  1.0f }, { 0, 0, 1 }, { 0.0f, 0.0f } }, // v1
        { { -1.0f, -1.0f,  1.0f }, { 0, 0, 1 }, { 0.5f, 0.0f } },
        { { -1.0f,  1.0f,  1.0f }, { 0, 0, 1 }, { 0.5f, 0.5f } },
        { {  1.0f, -1.0f,  1.0f }, { 0, 0, 1 }, { 0.0f, 0.0f } }, // v2
        { { -1.0f,  1.0f,  1.0f }, { 0, 0, 1 }, { 0.5f, 0.5f } },
        { {  1.0f,  1.0f,  1.0f }, { 0, 0, 1 }, { 0.0f, 0.5f } },

        // Left face (facing negative X)
        { { -1.0f, -1.0f,  1.0f }, { -1, 0, 0}, { 0.5f, 0.0f } }, // v1
        { { -1.0f, -1.0f, -1.0f }, { -1, 0, 0}, { 1.0f, 0.0f } },
        { { -1.0f,  1.0f, -1.0f }, { -1, 0, 0}, { 1.0f, 0.5f } },
        { { -1.0f, -1.0f,  1.0f }, { -1, 0, 0}, { 0.5f, 0.0f } }, // v2
        { { -1.0f,  1.0f, -1.0f }, { -1, 0, 0}, { 1.0f, 0.5f } },
        { { -1.0f,  1.0f,  1.0f }, { -1, 0, 0}, { 0.5f, 0.5f } },

        // Top face (facing positive Y)
        { { -1.0f,  1.0f, -1.0f }, { 0, 1, 0 } }, // v1
        { {  1.0f,  1.0f, -1.0f }, { 0, 1, 0 } },
        { {  1.0f,  1.0f,  1.0f }, { 0, 1, 0 } },
        { { -1.0f,  1.0f, -1.0f }, { 0, 1, 0 } }, // v2
        { {  1.0f,  1.0f,  1.0f }, { 0, 1, 0 } },
        { { -1.0f,  1.0f,  1.0f }, { 0, 1, 0 } },

        // Bottom face (facing negative Y)
        { { -1.0f, -1.0f,  1.0f }, { 0, -1, 0 } }, // v1
        { {  1.0f, -1.0f,  1.0f }, { 0, -1, 0 } },
        { {  1.0f, -1.0f, -1.0f }, { 0, -1, 0 } },
        { { -1.0f, -1.0f,  1.0f }, { 0, -1, 0 } }, // v2
        { {  1.0f, -1.0f, -1.0f }, { 0, -1, 0 } },
        { { -1.0f, -1.0f, -1.0f }, { 0, -1, 0 } },
    };

    GLuint texID;
    Handle<gl::Texture> texHandle = engine.OpenTextureFromPNG("textureAtlas.png").StaticCast<gl::Texture>();
    Handle<gl::Texture> texHandle2 = engine.OpenTextureFromPNG("earth_daymap_2k.png").StaticCast<gl::Texture>();

    /* Misc parameters. */
    constexpr float phongExponent = 60.0;
    constexpr Vector3DF diffuseComponent{ 1.0, 1.0, 1.0 };
    constexpr Vector3DF specularComponent{ 4.0, 4.0, 4.0 };
    constexpr Vector3DF lightPos{ -3, 0, 2 };

    // Create a shader using my GLSLObject class
    Material material{
        .texture = texHandle,
        .ambientLight = Vector3DF::Zero(),
        .diffuse = diffuseComponent,
        .specular = specularComponent,
        .shininess = phongExponent
    };
    Handle<gl::BlinnPhong> shader = engine.CreatePhong(material).StaticCast<gl::BlinnPhong>();

    Material material2{
        .texture = texHandle2,
        .ambientLight = Vector3DF::Zero(),
        .diffuse = diffuseComponent,
        .specular = specularComponent,
        .shininess = phongExponent
    };
    Handle<gl::BlinnPhong> shader2 = engine.CreatePhong(material2).StaticCast<gl::BlinnPhong>();

    /* Setup our mesh (single tri!) */
    Handle<gl::Mesh> mesh = engine.CreateMesh(vertices, Vector3DF(0,0,0), shader).StaticCast<gl::Mesh>();

    /* A Sphere! */
    Handle<gl::Mesh> sphere = engine.CreateSphere(Vector3DF(-3, 0, 0), -1.0f, shader2).StaticCast<gl::Mesh>();

    /* Setup our camera. */
    static constexpr eng::Vector3DF pos(0, 0, 10), viewDir(0, 0, -1);
    Handle<gl::PerspectiveCamera> cam = engine.CreatePerspectiveCamera(pos, viewDir, 0.5, winWidth, winHeight, 1.0).StaticCast<gl::PerspectiveCamera>();

    /* Add things to the scene. */
    std::shared_ptr<Scene> scene = engine.GetActiveScene();
    scene->cameras.Insert("main", cam);
    scene->shapes.Insert("myCoolTriangle", mesh);
    scene->shapes.Insert("myCoolSphere", sphere);
    scene->shaders.Insert("myCoolerShader", shader);
    scene->shaders.Insert("myCoolestShader", shader2);
    scene->EmplacePointLight(lightPos, {});

    /* Grab our renderer. */
    IRenderer* pRenderer = engine.GetRenderer();

    GLFWwindow* window = engine.GetGlfwWindow();

    /* Loop until the user closes the window */
    glm::mat4 modelMatrix, normalMatrix;
    constexpr float sensitivity = 0.2f;
    float rotationAngle = 0;
    float lastX = winWidth / 2.0f;
    float lastY = winHeight / 2.0f;
    float yaw = 0;
    float pitch = 0;
    while (!glfwWindowShouldClose(window))
    {
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();

        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Increment rotation angle. */
        //mesh->IncrementRotationAngle(0.05);

        /* Render your objects here */
        /* (my amazing triangle) */
        pRenderer->Render("main", renderBuffer);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Check for movement inputs. */
        constexpr float moveRatePerFrame = 0.05;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam->MoveByW(-moveRatePerFrame);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam->MoveByU(-moveRatePerFrame);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam->MoveByW(moveRatePerFrame);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam->MoveByU(moveRatePerFrame);
        }

        /* Get mouse position. */
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        /* Calculate diffs + update previous pos. */
        float xDiff = xpos - lastX;
        float yDiff = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

        /* Increment and clamp yaw & pitch. */
        yaw += xDiff * sensitivity;
        pitch += yDiff * sensitivity;
        if (pitch > 89.9f)
            pitch = 89.9f;
        else if (pitch < -89.9f)
            pitch = -89.9f;

        std::cout << xpos << ", " << ypos << '\n';

        cam->Rotate(pitch, yaw);

        if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS) {
            std::cout << "fps: " << 1.0/timeDiff << std::endl;
        }
        if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
  
    glfwTerminate();
    return 0;
}
