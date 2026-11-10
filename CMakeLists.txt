cmake_minimum_required(VERSION 3.24)
project(app CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
 
include(FetchContent)
 
FetchContent_Declare(glfw  GIT_REPOSITORY https://github.com/glfw/glfw.git       GIT_TAG 3.4)
FetchContent_Declare(box2d GIT_REPOSITORY https://github.com/erincatto/box2d.git GIT_TAG v3.1.0)
FetchContent_Declare(tracy GIT_REPOSITORY https://github.com/wolfpld/tracy.git   GIT_TAG v0.11.1)

FetchContent_MakeAvailable(glfw box2d tracy)

add_library(glad 3rd/glad/src/glad.c)
target_include_directories(glad PUBLIC 3rd/glad/include)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)

add_executable(app ${SRC})
target_include_directories(app PRIVATE src)
target_link_libraries(app PRIVATE glad glfw box2d Tracy::TracyClient)
