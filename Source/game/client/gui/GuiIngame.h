//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_GUIINGAME_H
#define MCPORT_GUIINGAME_H
#include "Gui.h"
#include "game/client/Minecraft.h"

#include "game/client/renderer/RenderItem.h"

class GuiIngame : public Gui {
private:
    Minecraft &mc;
    float prevVignetteBrightness = 1.0f;

    void renderInventorySlot(int_t slot, int_t x, int_t y, float partialTicks);

public:
    GuiIngame(Minecraft &mc);

    void renderGameOverlay(float partialTicks, bool inScreen, int_t mouseX, int_t mouseY);

    void renderVignette(float var1, int_t var2, int_t var3);

    void updateTick();
};


#endif //MCPORT_GUIINGAME_H
