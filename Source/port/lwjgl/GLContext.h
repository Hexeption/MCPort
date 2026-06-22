//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_GLCONTEXT_H
#define MCPORT_GLCONTEXT_H

#include <string>
#include <set>

#include "SDL3/SDL.h"
#include "glad/glad.h"

namespace lwjgl {
    namespace GLContext {
        namespace detail {
            struct GLCapabilities {
            private:
                std::set<std::string> caps;

            public:
                void add(const std::string &cap) {
                    caps.insert(cap);
                }

                bool operator[](const std::string &cap) const {
                    return caps.find(cap) != caps.end();
                }
            };

            SDL_Window *getWindow();

            SDL_GLContext getGLContext();
        }

        void instantiate();

        void destroy();

        const detail::GLCapabilities &getCapabilities();
    }
}


#endif //MCPORT_GLCONTEXT_H
