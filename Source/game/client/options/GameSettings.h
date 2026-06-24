//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GAMESETTINGS_H
#define MCPORT_GAMESETTINGS_H
#include <memory>
#include <vector>

#include "Keybinding.h"
#include "java/File.h"
#include "lwjgl/Keyboard.h"

class Minecraft;

class GameSettings {
private:
    std::vector<jstring> renderDistances = {u"FAR", u"NORMAL", u"SHORT", u"TINY"};
    std::vector<jstring> difficultyLevels = {u"Peaceful", u"Easy", u"Normal", u"Hard"};

    std::unique_ptr<File> optionsFile;

public:
    bool music = true;
    bool sound = true;
    bool invertMouse = false;
    bool showFramerate = false;
    int_t renderDistance = 0;
    bool viewBobbing = true;
    bool anaglyph = false; // 3d mode
    bool limitFramerate = false;
    bool fancyGraphics = true;

    Keybinding keyBindForward = Keybinding(u"Forward", lwjgl::Keyboard::KEY_W);
    Keybinding keyBindLeft = Keybinding(u"Left", lwjgl::Keyboard::KEY_A);
    Keybinding keyBindBack = Keybinding(u"Back", lwjgl::Keyboard::KEY_S);
    Keybinding keyBindRight = Keybinding(u"Right", lwjgl::Keyboard::KEY_D);
    Keybinding keyBindJump = Keybinding(u"Jump", lwjgl::Keyboard::KEY_SPACE);
    Keybinding keyBindInventory = Keybinding(u"Inventory", lwjgl::Keyboard::KEY_I);
    Keybinding keyBindDrop = Keybinding(u"Drop", lwjgl::Keyboard::KEY_Q);
    Keybinding keyBindChat = Keybinding(u"Chat", lwjgl::Keyboard::KEY_T);
    Keybinding keyBindToggleFog = Keybinding(u"Toggle fog", lwjgl::Keyboard::KEY_F);

    std::vector<Keybinding *> keyBindings = {
        &keyBindForward, &keyBindLeft, &keyBindBack, &keyBindRight, &keyBindJump, &keyBindDrop, &keyBindInventory,
        &keyBindChat, &keyBindToggleFog
    };

    int_t numberOfOptions = 10;
    int_t difficulty = 2; // normal
    bool thirdPersonView = false;

    GameSettings(Minecraft &minecraft, File mcDir);

    jstring getKeybindingDescription(int_t index);

    void setKeybinding(int_t index, int_t key);

    void setOptionValue(int_t index, int_t value);

    jstring getOptionDisplayString(int_t index);

    void loadOptions();

    void saveOptions();

protected:
    Minecraft &minecraft;

private:
    static jstring onOff(const bool value);

    static bool parseBool(const std::string &value);

    static bool splitOptionLine(const std::string &line, std::string &key, std::string &value);
};


#endif //MCPORT_GAMESETTINGS_H
