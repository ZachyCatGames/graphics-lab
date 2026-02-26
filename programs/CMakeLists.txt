# Add our main program subdirectory
add_subdirectory(main)

add_subdirectory(TriChallenge)

#
# OpenGL Example
#
# This builds a simple OpenGL example that you can use to get started with OpenGL.
#
#add_subdirectory(OpenGL)
# add_subdirectory(Vulkan)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  # Some examples
  #add_subdirectory(examples)
endif()

#
# Unit Tests - by enabling testing with cmake, we can create a target that lets unit tests be run. After re-building
# yourproject with ENABLE_TESTING called, a make target, called test will be enabled. You can then type
#
# make test
#
if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  enable_testing()
  add_subdirectory(utests)
endif()
