//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUISCREEN_H
#define MCPORT_GUISCREEN_H

#include <memory>
#include <vector>

#include "Gui.h"
#include "GuiButton.h"

class Minecraft;

class GuiScreen : public Gui, public std::enable_shared_from_this<GuiScreen> {
protected:
    Minecraft *mc = nullptr;
    std::vector<std::unique_ptr<GuiButton> > controlList;
    FontRenderer *fontRenderer = nullptr;

    virtual void keyTyped(char_t ch, int_t key);

    virtual void mouseClicked(int_t mouseX, int_t mouseY, int_t mouseButton);

    virtual void mouseMovedOrUp(int_t mouseX, int_t mouseY, int_t mouseButton);

    virtual void actionPerformed(GuiButton *button);

public:
    int_t width = 0;
    int_t height = 0;
    bool allowUserInput = false;

    virtual ~GuiScreen() = default;

    virtual void drawScreen(int_t mouseX, int_t mouseY, float partialTicks);

    static jstring getClipboardString();

    void setWorldAndResolution(Minecraft *minecraft, int_t width, int_t height);

    virtual void initGui();

    void handleInput();

    virtual void handleMouseInput();

    void handleKeyboardInput();

    virtual void updateScreen();

    virtual void onGuiClosed();

    void drawDefaultBackground();

    void drawWorldBackground(int_t scroll);

    void drawBackground(int_t scroll);

    virtual bool doesGuiPauseGame();

    virtual void deleteWorld(bool deleting, int_t id);
};

#endif //MCPORT_GUISCREEN_H
