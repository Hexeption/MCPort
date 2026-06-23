include(FetchContent)

# Glad
add_subdirectory(glad)

# ZLIB
FetchContent_Declare(
        ZLIB
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG v1.3.2
)
FetchContent_MakeAvailable(ZLIB)

if (TARGET zlibstatic AND NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
elseif (TARGET zlib AND NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB ALIAS zlib)
endif ()

# SDL3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.10
)
FetchContent_MakeAvailable(SDL3)

# STB

FetchContent_Declare(
        STB
        GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_MakeAvailable(STB)

FetchContent_GetProperties(STB)
if (NOT TARGET stb)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif ()
