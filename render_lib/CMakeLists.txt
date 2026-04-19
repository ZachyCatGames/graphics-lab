if (${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  set(UTIL_PLAT_SRC
    sources/framebuffer/nx/fb_NativeWindowWriter.plat-nx.cpp
  )
endif()

add_library (render_lib
  # Framebuffer Library.
  sources/framebuffer/fb_PngWriter.cpp

  # Shader Library.
  sources/raytracer/shader/shdr_BlinnPhong.cpp
  sources/raytracer/shader/shdr_Diffuse.cpp
  sources/raytracer/shader/shdr_Lambertian.cpp
  sources/raytracer/shader/shdr_Mirror.cpp
  sources/raytracer/shader/shdr_NormalShader.cpp

  # Shape Library
  sources/raytracer/shape/shape_Mesh.cpp
  sources/raytracer/shape/shape_Sphere.cpp
  sources/raytracer/shape/shape_Triangle.cpp

  # Demo Library
  #sources/demo/demo_DrawFramebuffer.cpp
  #sources/demo/demo_SceneRenderHelper.cpp

  # Core Library.
  sources/eng_ArgumentParsing.cpp
  sources/eng_Engine.cpp
  sources/eng_HandleGraphicsArgs.cpp
  sources/eng_model_obj.cpp
  sources/eng_Scene.cpp
  sources/eng_SceneLoader.cpp
  sources/eng_SceneParser_JSON.cpp

  # RT Core Library
  sources/raytracer/eng_Bvh.cpp
  sources/raytracer/eng_ObjectFactory.cpp
  sources/raytracer/eng_PerspectiveCamera.cpp
  sources/raytracer/eng_RayCaster.cpp
  sources/raytracer/eng_RendererBase.cpp
  sources/raytracer/eng_ThreadedRenderer.cpp

  # GL Core Library
  sources/gl/eng_PerspectiveCamera.cpp

  ${UTIL_PLAT_SRC}
)
target_compile_definitions(render_lib PUBLIC HAS_GLM)
target_link_libraries(render_lib PUBLIC Boost::program_options)
#target_link_libraries(cs4212-util PUBLIC glm::glm)

target_include_directories(render_lib PUBLIC include)

target_link_libraries(render_lib PRIVATE PNG::PNG)

