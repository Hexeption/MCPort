//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiDeleteWorld.h"

#include "GuiYesNo.h"
#include "game/client/Minecraft.h"
#include "game/world/World.h"

GuiDeleteWorld::GuiDeleteWorld(std::shared_ptr<GuiScreen> parentScreen) : GuiSelectWorld(std::move(parentScreen)) {
    screenTitle = u"Delete world";
}

void GuiDeleteWorld::initButtons() {
    controlList.push_back(std::make_unique<GuiButton>(6, width / 2 - 100, height / 6 + 168, u"Cancel"));
}

void GuiDeleteWorld::selectWorld(const int_t id) {
    const jstring saveName = getSaveName(id);
    if (!saveName.empty()) {
        mc->displayGuiScreen(std::make_shared<GuiYesNo>(shared_from_this(),
                                                        u"Are you sure you want to delete this world?",
                                                        u"'" + saveName + u"' will be lost forever!", id));
    }
}

void GuiDeleteWorld::deleteWorld(const bool deleting, const int_t id) {
    if (deleting) {
        const File minecraftDir = mc->getMinecraftDir();
        World::deleteWorld(minecraftDir, getSaveName(id));
    }

    mc->displayGuiScreen(parentScreen);
}
