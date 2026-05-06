#include "engine/gl/eng_IRenderable.h"
#include <engine/eng_Engine.h>

#include <engine/raytracer/eng_ObjectFactory.h>
#include <engine/raytracer/eng_ThreadedRenderer.h>

#include <engine/gl/eng_ObjectFactory.h>
#include <engine/gl/eng_Renderer.h>

#include <engine/png++/png.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <engine/gl/eng_IRenderable.h>

#include <cstdlib>
#include <print>

namespace eng {

namespace {

void InitializeGlfw() {
    if (!glfwInit())
        exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

} // namespace

void Engine::InitializeOpenGL() {
    /* Initialize glew. */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::print("GLEW Error: {}\n", err);
        exit(-1);
    }

    std::printf("Renderer: %s\n", glGetString (GL_RENDERER));
    std::printf("OpenGL version supported: %s\n", glGetString(GL_VERSION));

    /* Enable depth test. */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glClearColor(0.0, 0.7, 1.0, 1.0);
    glClearColor(7.0 / 255.0, 35.0 / 255.0, 220.0 / 255.0, 1.0);

    /* Setup viewport dims. */
    glViewport(0, 0, m_displayRenderBuffer->GetWidth(), m_displayRenderBuffer->GetHeight());


}

int Engine::Initialize(int argc, char** argv) {
    /* Initialize Graphics Args. */
    m_gArgs.process(argc, argv);

    /* Create the initial scene. */
    m_pActiveScene = std::make_shared<Scene>();

    m_renderMode = m_gArgs.renderMode;

    if (m_renderMode == "raytracer") {
        /* Setup Object Factory. */
        m_pObjFactory = std::make_unique<rt::ObjectFactory>(this);

        /* Setup renderer. */
        m_pRenderer = std::make_unique<rt::ThreadedRenderer>(m_pActiveScene, m_gArgs.rpp, m_gArgs.randpix, m_gArgs.recursionDepth, m_gArgs.numCpus);
    } else if (m_renderMode == "opengl") {
        /* Setup Object Factory. */
        m_pObjFactory = std::make_unique<gl::ObjectFactory>();

        /* Setup renderer. */
        m_pRenderer = std::make_unique<gl::Renderer>(m_pActiveScene);
    
        /* Init glfw. */
        InitializeGlfw();

        /* Create a GLFW window. */
        m_pWindow = glfwCreateWindow(m_gArgs.windowWidth, m_gArgs.windowHeight, "test", nullptr, nullptr);

        /* Disable the mouse cursor and enable raw input if supported. */
        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GL_TRUE);

        /* Setup main window. */
        m_displayRenderBuffer = m_pObjFactory->CreateDisplayRenderBuffer(m_pWindow);
        assert(m_displayRenderBuffer);

        /* Bind the main window as the current context. */
        gl::SetContextCurrent(m_displayRenderBuffer);

        /* Initialize OpenGL. */
        this->InitializeOpenGL();
    }

    return 0;
}

Handle<Texture> Engine::OpenTextureFromPNG(std::string_view path) {
    png::image<png::rgb_pixel> texPngImage;
    texPngImage.read(path.data());

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

    return this->CreateTexture(texData.data(), pngWidth, pngHeight);
}

} // namespace eng
