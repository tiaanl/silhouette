cmake_minimum_required(VERSION 3.19)

if (TARGET silhouette)
    return()
endif ()

project(silhouette)

option(USE_ASSIMP "Use assimp import types" OFF)

if (USE_ASSIMP)
    find_package(assimp CONFIG REQUIRED)
endif ()

add_subdirectory(../floats floats)

configure_file(${silhouette_SOURCE_DIR}/include/silhouette/config.cmake.hpp ${silhouette_BINARY_DIR}/silhouette/config.hpp)

set(HEADER_FILES
    ${silhouette_BINARY_DIR}/silhouette/config.hpp
    include/silhouette/codec/image/png.h
    include/silhouette/codec/scene/collada.hpp
    include/silhouette/codec/scene/obj.h
    include/silhouette/color.h
    include/silhouette/image.h
    include/silhouette/scene.h
    )

set(SOURCE_FILES
    src/codec/image/png.cpp
    src/codec/scene/collada.cpp
    src/codec/scene/obj.cpp
    src/ext/stb_image.cpp
    src/ext/tiny_obj_loader.cpp
    src/image.cpp
    src/scene.cpp
    )

nucleus_add_library(silhouette ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(silhouette PUBLIC ${silhouette_BINARY_DIR})
target_link_libraries(silhouette PUBLIC floats)
if (USE_ASSIMP)
    if (APPLE)
        message(${ASSIMP_INCLUDE_DIRS})
        message(${ASSIMP_LIBRARIES})
        target_include_directories(silhouette PRIVATE ${ASSIMP_INCLUDE_DIRS})
        target_link_libraries(silhouette PRIVATE ${ASSIMP_LIBRARIES})
    else ()
        target_link_libraries(silhouette PRIVATE assimp::assimp)
    endif ()
endif ()

set(TEST_FILES
    tests/codec/scene/obj_tests.cpp
    # tests/image_tests.cpp
    # tests/scene_tests.cpp
    )

nucleus_add_executable(silhouette_tests ${TEST_FILES})
target_link_libraries(silhouette_tests PRIVATE silhouette tests_main)
