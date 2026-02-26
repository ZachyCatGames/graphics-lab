if (${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  set(UTIL_PLAT_SRC
    sources/framebuffer/nx/fb_NativeWindowWriter.plat-nx.cpp
  )
endif()

add_library (render_lib
  # Framebuffer Library.
  sources/framebuffer/fb_Framebuffer.cpp
  sources/framebuffer/fb_PngWriter.cpp

  # Shader Library.
  sources/shader/detail/shdr_LambertianImpl.cpp
  sources/shader/shdr_BlinnPhong.cpp
  sources/shader/shdr_Mirror.cpp
  sources/shader/shdr_NormalShader.cpp

  # Shape Library
  sources/shape/shape_Sphere.cpp
  sources/shape/shape_Triangle.cpp

  # Core Library.
  sources/eng_ArgumentParsing.cpp
  sources/eng_HandleGraphicsArgs.cpp
  sources/eng_PerspectiveCamera.cpp
  sources/eng_Scene.cpp

  ${UTIL_PLAT_SRC}
)
target_compile_definitions(render_lib PUBLIC HAS_GLM)
target_link_libraries(render_lib PUBLIC Boost::program_options)
#target_link_libraries(cs4212-util PUBLIC glm::glm)

target_include_directories(render_lib PUBLIC include)

target_link_libraries(render_lib PRIVATE PNG::PNG)

