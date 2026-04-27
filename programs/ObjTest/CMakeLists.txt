add_executable(ObjTest
  main.cpp
)

target_link_libraries(ObjTest PRIVATE render_lib)

target_link_libraries (ObjTest PRIVATE GLEW::GLEW)
target_link_libraries (ObjTest PRIVATE glfw)

if(APPLE)
    target_link_libraries(ObjTest PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(ObjTest PRIVATE OpenGL::GL)
endif()
