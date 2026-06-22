cmake_minimum_required(VERSION 4.2)
project(MCPort)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(External)

add_executable(MCPort
        Source/client/RubyDung.cpp
        Source/client/RubyDung.h
        Source/port/Main.cpp
        Source/port/utils/SDLException.h
        Source/port/lwjgl/Display.cpp
        Source/port/lwjgl/Display.h
        Source/port/java/Type.h
        Source/port/java/String.cpp
        Source/port/java/String.h
        Source/port/lwjgl/GLContext.cpp
        Source/port/lwjgl/GLContext.h
        Source/port/java/System.cpp
        Source/port/java/System.h
        Source/client/Timer.cpp
        Source/client/Timer.h
)

target_include_directories(MCPort PRIVATE "Source")
target_include_directories(MCPort PRIVATE "Source/port")

target_link_libraries(MCPort PRIVATE SDL3::SDL3 glad)

