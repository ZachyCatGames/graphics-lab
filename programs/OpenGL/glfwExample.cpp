#include <cstdlib>
#include <iostream>
#include <vector>

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
    /* Initialize the library */
    if (!glfwInit()) {
        exit (-1);
    }
    // throw std::runtime_error("Error! initialization of glfw failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    int winWidth = 1000;
    float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
    int winHeight = winWidth / aspectRatio;
    
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(err != GLEW_OK) {
        std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
        exit(EXIT_FAILURE);
    }

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glClearColor(0.0, 0.7, 1.0, 1.0);
    glClearColor(7.0 / 255.0, 35.0 / 255.0, 220.0 / 255.0, 1.0);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    // Need to set a projection matrix that fits the aspect ratio set
    // by the window frame.
    //
    // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    float halfWidth = 15.0 / 2.0;
    float halfHeight = halfWidth / aspectRatio;
    constexpr float near = 5.0f;
    constexpr float far  = -5.0f;

    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    /* Disable the mouse cursor and enable raw input if supported. */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GL_TRUE);

    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;

    std::vector<Vertex> vertices {
        { { -3.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  3.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.0f,  5.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 1.0f } }
    };

    char* argv2[] = {
        "program",
        "--rendermode", "opengl"
    };
    Engine engine;
    engine.Initialize(3, argv2);

    IObjectFactory* pObjFactory = engine.GetObjectFactory();

    /* Misc parameters. */
    constexpr float phongExponent = 10.0;
    constexpr Vector3DF diffuseComponent{ 1.0, 1.0, 1.0 };
    constexpr Vector3DF specularComponent{ 1.0, 1.0, 1.0 };
    constexpr Vector4DF lightPos{ 0, 0, 2, 1.0 };

    // Create a shader using my GLSLObject class
    std::vector<shdr::PointLight> lights;
    Material material{
        .ambientLight = Vector3DF::Zero(),
        .diffuse = diffuseComponent,
        .specular = specularComponent,
        .shininess = phongExponent
    };
    Handle<gl::BlinnPhong> shader = pObjFactory->CreatePhong(material, lights);

    /* Setup our mesh (single tri!) */
    Handle<gl::Mesh> mesh = pObjFactory->CreateMesh(vertices, Vector3DF(0,0,0), shader);

    /* Setup our camera. */
    constexpr eng::Vector3DF pos(0, 0, 10), viewDir(0, 0, -1);
    Handle<gl::PerspectiveCamera> cam = pObjFactory->CreatePerspectiveCamera(pos, viewDir, 0.5, winWidth, winHeight, 1.0);

    /* Add things to the scene. */
    std::shared_ptr<Scene> scene = engine.GetActiveScene();
    scene->cameras.Insert("main", cam);
    scene->shapes.Insert("myCoolTriangle", mesh);
    scene->shaders.Insert("myCoolerShader", shader);

    /* Grab our renderer. */
    IRenderer* pRenderer = engine.GetRenderer();

    GLuint texID;
    {
        png::image<png::rgb_pixel> texPngImage;
        texPngImage.read(argv[1]);

        int pngWidth = texPngImage.get_width();
        int pngHeight = texPngImage.get_height();
        std::print("PNG Width:  {}\n", pngWidth);
        std::print("PNG Height: {}\n", pngHeight);

        std::vector<float> texData(pngWidth * pngHeight * 3);

        size_t idx = 0;
        for (int row = 0; row < pngHeight; row++) {
            for (int col = 0; col < pngWidth; col++) {
                png::rgb_pixel pixel = texPngImage[pngHeight - row - 1][col];
                texData[idx++] = ((float)pixel.red) / 255.0f;
                texData[idx++] = ((float)pixel.green) / 255.0f;
                texData[idx++] = ((float)pixel.blue) / 255.0f;
            }
        }

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pngWidth, pngHeight, 0, GL_RGB, GL_FLOAT, texData.data());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /* Copy eye and light position over. */
    shader->AssignLightPosition(lightPos);

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

        // This activates texture unit 0, the next texture we bind will be bound into TU0.
        glActiveTexture(GL_TEXTURE0);

        // Bind our texture into texture unit 0.
        glBindTexture(GL_TEXTURE_2D, texID);
        shader->SetTextureId(0);

        /* Render your objects here */
        /* (my amazing triangle) */
        pRenderer->Render("main", nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);

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
