cmake_minimum_required(VERSION 3.13)
project(Grapic
        LANGUAGES C CXX
        HOMEPAGE_URL http://licence-info.univ-lyon1.fr/grapic)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

if (NOT DEFINED CMAKE_BUILD_TYPE)
    # Set default build type to Debug
    message(WARNING "No build type was provided - defaulting to Debug. Use -DCMAKE_BUILD_TYPE=Debug|Release to set the build type")
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# Setup languages settings
set(CMAKE_CXX_STANDARD 11)

# Setup external projects
add_subdirectory(extern)

# Setup Grapic library
set(GRAPIC_LIB "Grapic")
add_library(${GRAPIC_LIB})
add_subdirectory(src)

# Setup projects
add_subdirectory(apps)
