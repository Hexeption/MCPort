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
        Source/port/lwjgl/BufferUtils.cpp
        Source/port/lwjgl/BufferUtils.h
        Source/port/java/Type.h
        Source/port/java/String.cpp
        Source/port/java/String.h
        Source/port/lwjgl/GLContext.cpp
        Source/port/lwjgl/GLContext.h
        Source/port/java/System.cpp
        Source/port/java/System.h
        Source/client/Timer.cpp
        Source/client/Timer.h
        Source/port/lwjgl/Keyboard.cpp
        Source/port/lwjgl/Keyboard.h
        Source/client/level/LevelListener.h
        Source/client/level/Level.cpp
        Source/client/level/Level.h
        Source/port/java/Random.cpp
        Source/port/java/Random.h
        Source/client/phys/AABB.cpp
        Source/client/phys/AABB.h
        Source/client/level/PerlinNoiseFilter.cpp
        Source/client/level/PerlinNoiseFilter.h
        Source/client/level/tile/Tile.cpp
        Source/client/level/tile/Tile.h
        Source/client/level/tile/GrassTile.cpp
        Source/client/level/tile/GrassTile.h
        Source/client/level/tile/DirtTile.cpp
        Source/client/level/tile/DirtTile.h
        Source/client/level/tile/Bush.cpp
        Source/client/level/tile/Bush.h
)

target_include_directories(MCPort PRIVATE "Source")
target_include_directories(MCPort PRIVATE "Source/port")

target_link_libraries(MCPort PRIVATE SDL3::SDL3 glad)
