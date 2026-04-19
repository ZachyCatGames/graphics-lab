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

#include <engine/gl/eng_PerspectiveCamera.h>
#include <engine/gl/eng_BlinnPhong.h>
#include <engine/gl/eng_Mesh.h>

using namespace eng;

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

int main(void)
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

    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;

    std::vector<float> vertexPositions {
        -3.0f, -3.0f, 0.0f,     // V0
         3.0f, -3.0f, 0.0f,     // V1
         0.0f,  5.0f, 0.0f      // V2
    };
    std::vector<float> vertexNormals {
        0.0f, 0.0f, 1.0f,     // V0
        0.0f, 0.0f, 1.0f,     // V1
        0.0f, 0.0f, 1.0f      // V2
    };

    /* Setup our mesh (single tri!) */
    gl::Mesh mesh(vertexPositions, vertexNormals, Vector3DF(0,0,0), nullptr);

    // Create a shader using my GLSLObject class
    eng::gl::BlinnPhong shader;

    /* Setup our camera. */
    constexpr eng::Vector3DF pos(0, 0, 0), viewDir(0, 0, -1);
    eng::gl::PerspectiveCamera cam(pos, viewDir, 0.5, winWidth, winHeight, 1.0);

    /* Misc parameters. */
    constexpr float phongExponent = 10.0;
    constexpr Vector4DF diffuseComponent{ 1.0, 1.0, 1.0, 1.0 };
    constexpr Vector4DF specularComponent{ 1.0, 1.0, 1.0, 1.0 };
    constexpr Vector4DF lightPos{ 0, 0, 2, 1.0 };
    constexpr Vector4DF eyePos{ 0, 0, 0, 1.0 };

    /* Copy eye and light position over. */
    shader.AssignEyePosition(eyePos)
          .AssignLightPosition(lightPos);

    /* Copy lighting components. */
    shader.AssignDiffuseComponent(diffuseComponent)
          .AssignSpecularComponent(specularComponent)
          .AssignPhongExponent(phongExponent);

    /* Loop until the user closes the window */
    glm::mat4 modelMatrix, normalMatrix;
    float rotationAngle = 0;
    while (!glfwWindowShouldClose(window))
    {
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();

        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Create the view matrix from our camera data. */
        glm::mat4 projectionMatrix = cam.GetProjectionMatrix(45.0f);
        glm::mat4 m_view = cam.GetViewMatrix();
        //glm::mat4 m_view = glm::lookAt(m_pos, m_pos - m_W, m_V);

        /* Setup model matrix. */
        modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::rotate(modelMatrix, rotationAngle, glm::vec3(0, 1, 0));
        normalMatrix = glm::transpose(glm::inverse(modelMatrix));

        /* Increment rotation angle. */
        rotationAngle += 0.05;
        if(rotationAngle > M_PI * 2)
            rotationAngle = 0;

        /* Copy the view and project matrices to the device. */
        shader.AssignProjectionMatrix(projectionMatrix)
              .AssignViewMatrix(m_view)
              .AssignModelMatrix(modelMatrix)
              .AssignNormalMatrix(normalMatrix);

        /* Render your objects here */
        /* (my amazing triangle) */
        shader.Activate();

        mesh.Render();

        shader.Deactivate();

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Check for movement inputs. */
        constexpr float moveRatePerFrame = 0.05;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam.MoveByW(-moveRatePerFrame);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam.MoveByU(-moveRatePerFrame);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam.MoveByW(moveRatePerFrame);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam.MoveByU(moveRatePerFrame);
        }

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
