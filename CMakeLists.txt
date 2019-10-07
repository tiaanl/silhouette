cmake_minimum_required(VERSION 3.10)

if (TARGET silhouette)
    return()
endif ()

project(silhouette)

add_subdirectory(../canvas canvas)

option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "" OFF)
option(ASSIMP_NO_EXPORT "" ON)
option(ASSIMP_BUILD_COLLADA_IMPORTER "" ON)
add_subdirectory(../assimp assimp)

set(silhouette_HEADER_FILES
    include/silhouette/Image/Image.h
    include/silhouette/Import/Import.h
    include/silhouette/Scene/Scene.h
    )

set(silhouette_SOURCE_FILES
    src/Image/Image.cpp
    src/Import/Collada.cpp
    src/Scene/Scene.cpp
    )

set(silhouette_TEST_FILES
    tests/Scene/SceneTests.cpp
    )

nucleus_add_library(silhouette ${silhouette_HEADER_FILES} ${silhouette_SOURCE_FILES})
target_include_directories(silhouette PRIVATE ${Assimp_SOURCE_DIR}/include ${Assimp_BINARY_DIR}/include)
target_link_libraries(silhouette PUBLIC canvas assimp)

nucleus_add_executable(silhouetteTests ${silhouette_TEST_FILES})
target_link_libraries(silhouetteTests PUBLIC silhouette testing)
