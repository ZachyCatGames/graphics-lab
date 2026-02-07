if (${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  set(UTIL_PLAT_SRC
    framebuffer/nx/NativeWindowWriter.plat-nx.cpp framebuffer/nx/NativeWindowWriter.plat-nx.h
  )
endif()

add_library (cs4212-util
#  ArgumentParsing.cpp ArgumentParsing.h
#  handleGraphicsArgs.cpp handleGraphicsArgs.h
  model_obj.cpp model_obj.h
  framebuffer/Framebuffer.cpp framebuffer/Framebuffer.h
  framebuffer/IFramebufferWriter.h
  framebuffer/FramebufferWriterBase.h
  framebuffer/PngWriter.cpp framebuffer/PngWriter.h
  vec.h
  ray.h
  ICamera.h
  PerspectiveCamera.cpp PerspectiveCamera.h
  ${UTIL_PLAT_SRC}
)
target_compile_definitions(cs4212-util PUBLIC HAS_GLM)
#target_link_libraries(cs4212-util PRIVATE Boost::program_options)
#target_link_libraries(cs4212-util PUBLIC glm::glm)

# TODO: do smth more elegant
add_executable (cs4212-exec
  main.cpp
)

target_link_libraries(cs4212-exec PRIVATE cs4212-util)
target_link_libraries(cs4212-exec PRIVATE PNG::PNG)
