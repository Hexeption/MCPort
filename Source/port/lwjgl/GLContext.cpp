//
// Created by Keir Davis on 22/06/2026.
//

#include "GLContext.h"

#include <iostream>
#include <stdexcept>
#include <csignal>
#include <memory>


#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "utils/SDLException.h"

namespace lwjgl {
    namespace GLContext {
        namespace detail {
            class GLContext {
            private:
                SDL_Window *window = nullptr;
                SDL_GLContext gl_context = nullptr;
                GLCapabilities capabilties;

            public:
                GLContext() {
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);


                    window = SDL_CreateWindow("MCPort", 854, 480,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
                    if (window == nullptr)
                        throw SDLException("creating SDL window");

                    gl_context = SDL_GL_CreateContext(window);
                    if (gl_context == nullptr)
                        throw SDLException("creating OpenGL context");

                    if (!SDL_GL_MakeCurrent(window, gl_context))
                        throw SDLException("making OpenGL context current");

                    if (!gladLoaderLoadGL())
                        throw std::runtime_error("Failed to load glad");

                    if (!SDL_GL_SetSwapInterval(0))
                        throw SDLException("disabling VSync");
                }

                ~GLContext() {
                    if (gl_context != nullptr) {
                        SDL_GL_DestroyContext(gl_context);
                        gl_context = nullptr;
                    }

                    if (window != nullptr) {
                        SDL_DestroyWindow(window);
                        window = nullptr;
                    }
                }

                SDL_Window *getWindow() const { return window; }
                SDL_GLContext getGLContext() const { return gl_context; }
                const GLCapabilities &getCapabilities() const { return capabilties; }
            };

            static std::unique_ptr<GLContext> &getContextStorage() {
                static std::unique_ptr<GLContext> context;
                return context;
            }

            static GLContext &getContext() {
                return *getContextStorage();
            }

            SDL_Window *getWindow() {
                return getContext().getWindow();
            }

            SDL_GLContext getGLContext() {
                return getContext().getGLContext();
            }
        }

        void instantiate() {
            auto &context = detail::getContextStorage();
            if (context == nullptr) {
                context = std::make_unique<detail::GLContext>();
            }

            if (!SDL_GL_MakeCurrent(detail::getContext().getWindow(), detail::getContext().getGLContext()))
                throw SDLException("making cached OpenGL context current");
        }

        void destroy() {
            auto &context = detail::getContextStorage();
            context.reset();
        }

        const detail::GLCapabilities &getCapabilities() {
            return detail::getContext().getCapabilities();
        }
    }
}
