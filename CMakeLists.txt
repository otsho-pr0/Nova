cmake_minimum_required(VERSION 3.31)

project(Nova VERSION 0.0.1
    LANGUAGES C CXX
    DESCRIPTION "Direct3D 11 Engine"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

set(BUILD_SHARED_LIBS off)
set(ASSIMP_BUILD_TESTS off)
set(ASSIMP_BUILD_ASSIMP_TOOLS off)
set(ASSIMP_INSTALL off)

add_subdirectory(thirdparty/assimp/)

file(GLOB_RECURSE SOURCES src/*.c*)
file(GLOB_RECURSE IMGUI_SOURCES thirdparty/imgui/*.c*)
add_executable(Nova ${SOURCES} ${IMGUI_SOURCES} thirdparty/stb/stb_image.c)
target_include_directories(Nova PRIVATE thirdparty/stb/)
target_include_directories(Nova PRIVATE thirdparty/imgui/)
target_include_directories(Nova PUBLIC include/)
target_link_libraries(Nova PUBLIC dxgi d3d11 d3dcompiler dwmapi assimp)

if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
    add_compile_definitions(DEBUG) 
else() 
    add_compile_definitions(NDEBUG) 
endif()