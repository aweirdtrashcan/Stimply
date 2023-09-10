cmake_minimum_required(VERSION "3.20")

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)

include_directories("./src")

project("StimplyEngine")
file(GLOB_RECURSE TargetH "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE TargetC "${CMAKE_SOURCE_DIR}/src/*.cpp")

set(TARGET_SRC ${TargetC} ${TargetH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_compile_definitions(_DEBUG=1)

if (WIN32 OR MSVC)
    add_compile_definitions(S_WINDOWS_=1)
endif ()

if (UNIX)
    add_compile_definitions(S_LINUX_=1)
endif ()


add_link_options(-lglfw)
add_link_options(-lvulkan)

add_executable("StimplyEngine" ${TARGET_SRC})