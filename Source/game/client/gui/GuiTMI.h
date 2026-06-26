//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_GUITMI_H
#define MCPORT_GUITMI_H

#include <vector>

#include "Gui.h"
#include "game/client/renderer/RenderItem.h"

class FontRenderer;
class Minecraft;
class RenderEngine;

class GuiTMI final : public Gui {
public:
    static constexpr int_t PANEL_W = 5 * 18 + 2;

    bool visible = false;

    void init(Minecraft *mc);

    void draw(FontRenderer &font, RenderEngine &engine, int_t mouseX, int_t mouseY,
              int_t panelX, int_t screenH);

    bool mouseClicked(int_t mouseX, int_t mouseY, int_t button, int_t panelX);

    void changePage(int_t delta);

    void toggle();

    void show() { visible = true; sharedVisible = true; }

private:
    static constexpr int_t COLS = 5;
    static constexpr int_t CELL = 18;
    static constexpr int_t FOOTER = 12;

    static RenderItem itemRenderer;
    static bool sharedVisible;

    Minecraft *mc = nullptr;
    std::vector<int_t> itemIds;
    int_t page = 0;
    int_t lastScreenH = 240;

    int_t itemsPerPage(int_t screenH) const { return ((screenH - FOOTER) / CELL) * COLS; }
    int_t totalPages(int_t screenH) const {
        const int_t ipp = itemsPerPage(screenH);
        return ipp > 0 ? (static_cast<int_t>(itemIds.size()) + ipp - 1) / ipp : 1;
    }

    static int_t stackCountFor(int_t id);
    void giveItem(int_t id, int_t count);
};

#endif //MCPORT_GUITMI_H
