#include "engine/eng_ExportableRenderBuffer.h"
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>

#include <engine/eng_Engine.h>

#include <engine/eng_model_obj.h>

#include <engine/framebuffer/fb_PngWriter.h>

#include <array>
#include <print>
#include <cstdio>
#include <memory>
#include <thread>
#include <vector>
#include <algorithm>

using namespace eng;

constexpr int img_width  = 600;
constexpr int img_height = 600;

    constexpr float vp_height = 2.0;
    constexpr float vp_width  = 1.0;

    constexpr eng::Vector3DF pos{0, 0, 7.0};
    constexpr eng::Vector3DF dir = Vector3DF{0.0, 0, -1.0};
    constexpr float foc_len = 0.25;

    constexpr auto aspect_ratio = static_cast<float>(img_width) / img_height;

fb::Framebuffer fbb(img_width, img_height);

#ifdef __SWITCH__
#include "framebuffer/nx/NativeWindowWriter.plat-nx.h"
#endif

#define VECTOR_COLOR_IMG

int main(int argc, char** argv) {
    char* test[] = {
        "program",
        "--rendermode", "opengl",
        "--winwidth", "800",
        "--winheight", "800"
    };

    Engine engine;
    engine.Initialize(7, test);

    ModelOBJ objLoader;
    objLoader.import("dragon.obj");

    assert(objLoader.getNumberOfMeshes() > 0);
    const auto& mesh = objLoader.getMesh(0);

    std::print("{} {}\n", mesh.startIndex, mesh.triangleCount);
    std::print("{} {}\n", objLoader.getNumberOfVertices(), objLoader.getNumberOfTriangles());
    
    Material material {
        .texture = nullptr,
        .ambientLight = Vector3DF(0,0,0),
        .diffuse = Vector3DF(0.5f, 0.5f, 0.5f)
    };

    Handle<IShader> shader = engine.CreateLambertian(material);
    assert(shader);

    std::print("mesh triangleCount: {}\n", mesh.triangleCount);
    std::print("mesh startIndex: {}\n", mesh.startIndex);

    std::print("number of indices: {}\n", objLoader.getNumberOfIndices());
    std::print("number of vertices: {}\n", objLoader.getNumberOfVertices());
    std::print("number of triangles: {}\n", objLoader.getNumberOfTriangles());
    std::print("number of meshes: {}\n", objLoader.getNumberOfMeshes());

    std::vector<Vertex> vertices;
    vertices.reserve(mesh.triangleCount * 3);
    const int* indexBuffer = objLoader.getIndexBuffer();
    int start = mesh.startIndex;
    int end = start + mesh.triangleCount * 3;
    for (int i = start; i < end; i++) {
        const auto& vertex = objLoader.getVertex(indexBuffer[i]);
        
        Vertex newVertex{
            .position = Vector3DF(vertex.position),
            .normal = Vector3DF(vertex.normal),
            .texCoord = Vector2DF(vertex.texCoord)
        };
        
        vertices.push_back(newVertex);
    }

    std::print("vertex buffer size: {}\n", vertices.size());

    Handle<IShape> model = engine.CreateMesh(vertices, Vector3DF::Zero(), shader);
    assert(model);

    std::shared_ptr<Scene> scene = engine.GetActiveScene();
    assert(scene != nullptr);

    /* Add a point light. */
    scene->EmplacePointLight(pos, Vector3DF(1,1,1));

    scene->shapes.Insert("dragon", model);
    scene->shaders.Insert("lambertian", shader);

    Handle<ICamera> camera = engine.CreatePerspectiveCamera(pos, dir, foc_len, img_width, img_height, vp_width);
    assert(camera);
    scene->cameras.Insert("main", camera);

    Handle<RenderBuffer> renderBuffer = nullptr;
    std::string renderString = engine.GetRenderString();
    if (renderString == "opengl") {
        renderBuffer = engine.GetDisplayRenderBuffer();
    } else {
        renderBuffer = engine.CreateExportableRenderBuffer(img_width, img_height);
    }
    assert(renderBuffer.IsValid());

    std::cerr << "test\n";

    if (renderString == "opengl") {
        GLFWwindow* window = engine.GetGlfwWindow();

        /* Get image dims. */
        size_t winWidth  = renderBuffer->GetWidth();
        size_t winHeight = renderBuffer->GetHeight();

        glm::mat4 modelMatrix, normalMatrix;
        constexpr float sensitivity = 0.2f;
        float rotationAngle = 0;
        float lastX = winWidth / 2.0f;
        float lastY = winHeight / 2.0f;
        float yaw = 0;
        float pitch = 0;
        while (!glfwWindowShouldClose(window))
        {
            // Clear the window's buffer (or clear the screen to our
            // background color)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* Increment rotation angle. */
            //mesh->IncrementRotationAngle(0.05);

            /* Render your objects here */
            /* (my amazing triangle) */
            engine.RenderActiveScene("main", renderBuffer);

            // Swap the front and back buffers
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            /* Check for movement inputs. */
            constexpr float moveRatePerFrame = 0.05;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera->MoveByW(-moveRatePerFrame);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera->MoveByU(-moveRatePerFrame);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera->MoveByW(moveRatePerFrame);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera->MoveByU(moveRatePerFrame);
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

            //std::cout << xpos << ", " << ypos << '\n';

            camera->Rotate(pitch, yaw);

            if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS) {
                //std::cout << "fps: " << 1.0/timeDiff << std::endl;
            }
            if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, 1);
            }
        }
    } else {
        engine.RenderActiveScene("main", renderBuffer);
        auto fb = renderBuffer.StaticCast<ExportableRenderBuffer>()->ExportToFramebuffer();
        fb::PngWriter fbWriter("out.png");
        fbWriter.WriteFramebuffer(fb);
    }

}
