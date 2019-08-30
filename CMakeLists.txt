cmake_minimum_required(VERSION 3.10)

project(silhouette)

add_subdirectory(../canvas canvas)
add_subdirectory(../assimp assimp)

set(silhouette_HEADER_FILES
    include/silhouette/image/Image.h
    include/silhouette/import/Import.h
    include/silhouette/scene/Mesh.h
    include/silhouette/scene/Scene.h
    )

set(silhouette_SOURCE_FILES
    src/image/Image.cpp
    src/import/Collada.cpp
    src/scene/Scene.cpp
    )

set(silhouette_TEST_FILES
    tests/scene/SceneTests.cpp
    )

add_library(silhouette ${silhouette_HEADER_FILES} ${silhouette_SOURCE_FILES})
target_include_directories(silhouette PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(silhouette PRIVATE ${Assimp_SOURCE_DIR}/include ${Assimp_BINARY_DIR}/include)
target_link_libraries(silhouette canvas assimp)
set_target_properties(silhouette PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER silhouette
    )

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Enable warnings and multi-process compilation.
    target_compile_options(silhouette PUBLIC /W3 /WX /MP)

    # Enable memory debugging.
    target_compile_definitions(silhouette PUBLIC "-D_CRTDBG_MAP_ALLOC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    # Enable warnings.
    target_compile_options(silhouette PUBLIC -Wall -Wextra -pedantic -Werror)
endif()

add_executable(silhouetteTests ${silhouette_TEST_FILES})
target_link_libraries(silhouetteTests PUBLIC silhouette testing)
set_target_properties(silhouetteTests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER silhouette
    )

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Enable warnings and multi-process compilation.
    target_compile_options(silhouetteTests PUBLIC /W3 /WX /MP)

    # Enable memory debugging.
    target_compile_definitions(silhouetteTests PUBLIC "-D_CRTDBG_MAP_ALLOC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    # Enable warnings.
    target_compile_options(silhouetteTests PUBLIC -Wall -Wextra -pedantic -Werror)
endif()
