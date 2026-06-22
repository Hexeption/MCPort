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