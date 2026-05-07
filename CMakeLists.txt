cmake_minimum_required(VERSION 3.22)

project(cs4212StarterCode
  VERSION 1.0
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)

add_compile_definitions(ENGINE_BUILD_GL_RENDER)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

#add_compile_options(-flto -march=native)
#add_link_options(-flto -march=native)
if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK}
        --suppress=missingInclude
        --enable=all
        --inline-suppr
        --inconclusive
        -i
        ${CMAKE_SOURCE_DIR})
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy-13)
  if(CLANGTIDY)
    set(CMAKE_C_CLANG_TIDY ${CLANGTIDY})
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
  else()
    message(SEND_ERROR "clang-tidy could not be located.")
  endif()
endif()

# Should be using at least C++17, but 20 is better, so
# make sure we switch to 20
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------------------
# PNG and ZLIB
#
# This macro will attempt to locate the PNG include directories, link directories and libraries. The INCLUDE_DIRECTORIES
# macro adds the resulting include directories to the search path.
# --------------------------------------------------------------------
FIND_PACKAGE(PNG REQUIRED)
if(NOT PNG_FOUND)
  message(STATUS "Could not find the PNG Libraries!")
endif(NOT PNG_FOUND)

Include(FetchContent)

# Catch2
if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.0 # or a later release
  )
  FetchContent_MakeAvailable(Catch2)
endif()

# Boost
# Necessary to get Win32 version to work, plus, not a bad idea to link statically for our purposes.
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)

find_package(Boost REQUIRED COMPONENTS program_options)
if(NOT ${Boost_FOUND})
  message(STATUS "Could not find the Boost Libraries!")
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "NintendoSwitch")
  add_compile_options(-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE -O2)
  add_link_options(-g -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE)

  include_directories(${NX_ROOT}/include ${DKP_INSTALL_PREFIX_INIT}/include)
endif()

# --------------------------------------------------------------------
# OpenGL Related Components
# --------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(glm CONFIG REQUIRED)
include($ENV{DEVKITPRO}/portlibs/switch/include/glm/CMakeLists.txt)
find_package(glfw3 CONFIG REQUIRED)

# not using Vulkan at the moment
# find_package(Vulkan)

# Add my renderlib
add_subdirectory(render_lib)

# Add the programs driectory.
add_subdirectory(programs)
