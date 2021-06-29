cmake_minimum_required(VERSION 3.19)

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
if (APPLE)
  message(${ASSIMP_INCLUDE_DIRS})
  message(${ASSIMP_LIBRARIES})
  target_include_directories(silhouette PRIVATE ${ASSIMP_INCLUDE_DIRS})
  target_link_libraries(silhouette PRIVATE ${ASSIMP_LIBRARIES})
else ()
  target_link_libraries(silhouette PRIVATE assimp::assimp)
endif ()

set(TEST_FILES
    tests/image_tests.cpp
    tests/scene_tests.cpp
    )

nucleus_add_executable(silhouette_tests ${TEST_FILES})
target_link_libraries(silhouette_tests PRIVATE silhouette tests_main)
