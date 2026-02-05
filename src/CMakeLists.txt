add_library (cs4212-util
#  ArgumentParsing.cpp ArgumentParsing.h
#  handleGraphicsArgs.cpp handleGraphicsArgs.h
  model_obj.cpp model_obj.h
  Framebuffer.cpp Framebuffer.h
  vec.h
  ray.h
  ICamera.h
  PerspectiveCamera.cpp PerspectiveCamera.h
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
