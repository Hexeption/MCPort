//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiSelectWorld.h"

#include <memory>

#include "GuiDeleteWorld.h"
#include "game/client/Minecraft.h"
#include "game/world/World.h"
#include "java/String.h"

GuiSelectWorld::GuiSelectWorld(std::shared_ptr<GuiScreen> parentScreen) : parentScreen(std::move(parentScreen)) {
}

void GuiSelectWorld::initGui() {
    controlList.clear();
    const File minecraftDir = mc->getMinecraftDir();

    for (int_t var2 = 0; var2 < 5; ++var2) {
        const jstring levelName = u"World" + String::toString(var2 + 1);
        std::unique_ptr<NBTTagCompound> levelData = World::getLevelData(minecraftDir, levelName);
        if (levelData == nullptr) {
            controlList.push_back(std::make_unique<GuiButton>(var2, width / 2 - 100, height / 6 + 24 * var2,
                                                              u"- empty -"));
        } else {
            jstring buttonText = u"World " + String::toString(var2 + 1);
            buttonText += u" (";
            const long_t sizeOnDisk = levelData->getLong(u"SizeOnDisk");
            buttonText += String::toString(static_cast<float>(sizeOnDisk / 1024L * 100L / 1024L) / 100.0F);
            buttonText += u" MB)";
            controlList.push_back(std::make_unique<GuiButton>(var2, width / 2 - 100, height / 6 + 24 * var2,
                                                              buttonText));
        }
    }

    initButtons();
}

jstring GuiSelectWorld::getSaveName(const int_t id) {
    const File minecraftDir = mc->getMinecraftDir();
    const jstring levelName = u"World" + String::toString(id);
    return World::getLevelData(minecraftDir, levelName) != nullptr ? levelName : u"";
}

void GuiSelectWorld::initButtons() {
    controlList.push_back(std::make_unique<GuiButton>(5, width / 2 - 100, height / 6 + 120 + 12, u"Delete world..."));
    controlList.push_back(std::make_unique<GuiButton>(6, width / 2 - 100, height / 6 + 168, u"Cancel"));
}

void GuiSelectWorld::actionPerformed(GuiButton *button) {
    if (button->enabled) {
        if (button->id < 5) {
            selectWorld(button->id + 1);
        } else if (button->id == 5) {
            mc->displayGuiScreen(std::make_shared<GuiDeleteWorld>(shared_from_this()));
        } else if (button->id == 6) {
            mc->displayGuiScreen(parentScreen);
        }
    }
}

void GuiSelectWorld::selectWorld(const int_t id) {
    mc->displayGuiScreen(nullptr);
    if (!selected) {
        selected = true;
        mc->startWorld(u"World" + String::toString(id));
        mc->displayGuiScreen(nullptr);
    }
}

void GuiSelectWorld::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    drawCenteredString(fontRenderer, screenTitle, width / 2, 20, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}
