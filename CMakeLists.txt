cmake_minimum_required(VERSION 3.10)

if (TARGET silhouette)
    return()
endif ()

project(silhouette)

find_package(assimp CONFIG REQUIRED)

add_subdirectory(../floats floats)

set(HEADER_FILES
    include/silhouette/color.h
    include/silhouette/image.h
    include/silhouette/scene.h
    )

set(SOURCE_FILES
    src/image.cpp
    src/scene.cpp
    )

nucleus_add_library(silhouette ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(silhouette PUBLIC floats)
target_link_libraries(silhouette PRIVATE assimp::assimp)

set(TEST_FILES
    tests/image_tests.cpp
    tests/scene_tests.cpp
    )

nucleus_add_executable(silhouette_tests ${TEST_FILES})
target_link_libraries(silhouette_tests PRIVATE silhouette tests_main)
