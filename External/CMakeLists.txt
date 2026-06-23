include(FetchContent)

# Glad
add_subdirectory(glad)

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
endif()
