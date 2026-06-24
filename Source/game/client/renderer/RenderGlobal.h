//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERGLOBAL_H
#define MCPORT_RENDERGLOBAL_H

class Minecraft;
class RenderEngine;

class RenderGlobal {
private:
    Minecraft &minecraft;
    RenderEngine &renderEngine;

public:
    RenderGlobal(Minecraft &minecraft, RenderEngine &renderEngine);
};


#endif //MCPORT_RENDERGLOBAL_H
