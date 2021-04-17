cmake_minimum_required(VERSION 3.10)

if (TARGET silhouette)
    return()
endif ()

project(silhouette)

find_package(assimp CONFIG REQUIRED)

add_subdirectory(../floats floats)

set(silhouette_HEADER_FILES
    include/silhouette/color.h
    include/silhouette/image.h
    include/silhouette/scene.h
    )

set(silhouette_SOURCE_FILES
    src/image.cpp
    src/scene.cpp
    )

nucleus_add_library(silhouette ${silhouette_HEADER_FILES} ${silhouette_SOURCE_FILES})
target_link_libraries(silhouette PUBLIC floats)
target_link_libraries(silhouette PRIVATE assimp::assimp)

set(TEST_FILES
    tests/image_tests.cpp
    tests/scene_tests.cpp
    )

nucleus_add_executable(silhouetteTests ${TEST_FILES})
target_link_libraries(silhouetteTests PRIVATE silhouette testsMain)
