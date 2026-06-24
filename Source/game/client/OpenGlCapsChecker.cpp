//
// Created by Keir Davis on 24/06/2026.
//

#include "OpenGlCapsChecker.h"

#include "lwjgl/GLContext.h"

bool OpenGlCapsChecker::tryCheckOcclusionCapable = false;


bool OpenGlCapsChecker::checkARBOcclusion() {
   return tryCheckOcclusionCapable && lwjgl::GLContext::getCapabilities()["GL_ARB_occlusion_query"];
}
