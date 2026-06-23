cmake_minimum_required(VERSION 4.2)
project(MCPort)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_subdirectory(External)

add_executable(MCPort
        Source/client/RubyDung.cpp
        Source/client/RubyDung.h
        Source/client/HitResult.cpp
        Source/client/HitResult.h
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
        Source/client/Textures.cpp
        Source/client/Textures.h
        Source/client/level/Frustum.cpp
        Source/client/level/Frustum.h
        Source/client/level/LevelRenderer.cpp
        Source/client/level/LevelRenderer.h
        Source/client/level/DirtyChunkSorter.cpp
        Source/client/level/DirtyChunkSorter.h
        Source/client/level/LevelListener.h
        Source/client/level/Level.cpp
        Source/client/level/Level.h
        Source/client/level/Tesselator.cpp
        Source/client/level/Tesselator.h
        Source/client/level/Chunk.cpp
        Source/client/level/Chunk.h
        Source/port/lwjgl/Keyboard.cpp
        Source/port/lwjgl/Keyboard.h
        Source/port/java/Random.cpp
        Source/port/java/Random.h
        Source/client/phys/AABB.cpp
        Source/client/phys/AABB.h
        Source/client/level/PerlinNoiseFilter.cpp
        Source/client/level/PerlinNoiseFilter.h
        Source/client/level/Tesselator.cpp
        Source/client/level/Tesselator.h
        Source/client/level/Chunk.cpp
        Source/client/level/Chunk.h
        Source/client/level/tile/Tile.cpp
        Source/client/level/tile/Tile.h
        Source/client/level/tile/GrassTile.cpp
        Source/client/level/tile/GrassTile.h
        Source/client/level/tile/DirtTile.cpp
        Source/client/level/tile/DirtTile.h
        Source/client/level/tile/Bush.cpp
        Source/client/level/tile/Bush.h
        Source/client/Entity.cpp
        Source/client/Entity.h
        Source/client/Player.cpp
        Source/client/Player.h
        Source/client/particle/Particle.cpp
        Source/client/particle/Particle.h
        Source/client/particle/ParticleEngine.cpp
        Source/client/particle/ParticleEngine.h
        Source/port/utils/GLU.cpp
        Source/port/utils/GLU.h
        Source/port/lwjgl/Mouse.cpp
        Source/port/lwjgl/Mouse.h
)

target_include_directories(MCPort PRIVATE "Source")
target_include_directories(MCPort PRIVATE "Source/port")

target_link_libraries(MCPort PRIVATE SDL3::SDL3 glad stb ZLIB::ZLIB)

add_custom_command(
        TARGET MCPort POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:MCPort>/Assets
)
