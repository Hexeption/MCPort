cmake_minimum_required(VERSION 4.2)
project(MCPort)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(MCPORT_BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif ()

option(MCPORT_ENABLE_EDGE_BARRIER "Enable invisible collision barriers at the edge of the level" ON)
option(MCPORT_MODERN_BLOCK_CONTROLS "Use modern Minecraft block controls: left click breaks, right click places" ON)

add_subdirectory(External)

add_executable(MCPort
        Source/game/client/Minecraft.cpp
        Source/game/client/Minecraft.h
        Source/port/Main.cpp
        Source/port/java/Math.cpp
        Source/port/java/Math.h
        Source/port/java/Random.cpp
        Source/port/java/Random.h
        Source/port/java/String.cpp
        Source/port/java/String.h
        Source/port/java/System.cpp
        Source/port/java/System.h
        Source/port/java/Type.h
        Source/port/lwjgl/BufferUtils.cpp
        Source/port/lwjgl/BufferUtils.h
        Source/port/lwjgl/Display.cpp
        Source/port/lwjgl/Display.h
        Source/port/lwjgl/GLContext.cpp
        Source/port/lwjgl/GLContext.h
        Source/port/lwjgl/Keyboard.cpp
        Source/port/lwjgl/Keyboard.h
        Source/port/lwjgl/Mouse.cpp
        Source/port/lwjgl/Mouse.h
        Source/port/utils/GLU.cpp
        Source/port/utils/GLU.h
        Source/port/utils/SDLException.h
        Source/game/client/Timer.cpp
        Source/game/client/Timer.h
)

set_target_properties(MCPort PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${MCPORT_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MCPORT_BIN_DIR}/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MCPORT_BIN_DIR}/release"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MCPORT_BIN_DIR}/relwithdebinfo"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${MCPORT_BIN_DIR}/minsizerel"
)

target_include_directories(MCPort PRIVATE "Source")
target_include_directories(MCPort PRIVATE "Source/port")

target_compile_definitions(MCPort PRIVATE
        MCPORT_ENABLE_EDGE_BARRIER=$<BOOL:${MCPORT_ENABLE_EDGE_BARRIER}>
        MCPORT_MODERN_BLOCK_CONTROLS=$<BOOL:${MCPORT_MODERN_BLOCK_CONTROLS}>
)

target_link_libraries(MCPort PRIVATE SDL3::SDL3 glad stb ZLIB::ZLIB)

add_custom_command(
        TARGET MCPort POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:MCPort> $<TARGET_RUNTIME_DLLS:MCPort>
        COMMAND_EXPAND_LISTS
)

add_custom_command(
        TARGET MCPort POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:MCPort>/Assets
)
