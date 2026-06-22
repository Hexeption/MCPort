//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_SDLEXCEPTION_H
#define MCPORT_SDLEXCEPTION_H

#include <stdexcept>
#include <string>
#include <string_view>

#include "SDL3/SDL_error.h"

class SDLException final : public std::runtime_error {
private:
    static std::string buildMessage(std::string_view context) {
        const char *error = SDL_GetError();
        std::string message = "SDL error";

        if (!context.empty()) {
            message += " in ";
            message += context;
        }

        message += ": ";
        message += (error != nullptr && error[0] != '\0') ? error : "unknown error";
        return message;
    }

public:
    explicit SDLException(std::string_view context = {})
        : std::runtime_error(buildMessage(context)) {
    }
};

#endif //MCPORT_SDLEXCEPTION_H
