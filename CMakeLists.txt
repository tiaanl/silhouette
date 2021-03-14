cmake_minimum_required(VERSION 3.10)

if (TARGET silhouette)
    return()
endif ()

project(silhouette)

find_package(assimp CONFIG REQUIRED)

add_subdirectory(../canvas canvas)

set(silhouette_HEADER_FILES
    include/silhouette/Image.h
    include/silhouette/Import/Import.h
    include/silhouette/Scene/Scene.h
    )

set(silhouette_SOURCE_FILES
    src/Image/Image.cpp
    src/Import/Collada.cpp
    src/Scene/Scene.cpp
    )

nucleus_add_library(silhouette ${silhouette_HEADER_FILES} ${silhouette_SOURCE_FILES})
target_link_libraries(silhouette PUBLIC canvas)
target_link_libraries(silhouette PRIVATE assimp::assimp)

find_package(Catch2 CONFIG REQUIRED)

set(TEST_FILES
    tests/Scene/SceneTests.cpp
    )

nucleus_add_executable(silhouetteTests tests/Main.cpp ${TEST_FILES})
target_link_libraries(silhouetteTests PRIVATE silhouette)
target_link_libraries(silhouetteTests PRIVATE Catch2::Catch2)
