//
// Created by Keir Davis on 24/06/2026.
//

#include "GameSettings.h"

#include <fstream>
#include <stdexcept>

#include "game/client/Minecraft.h"

GameSettings::GameSettings(Minecraft &minecraft, File mcDir) : minecraft(minecraft) {
    optionsFile = std::make_unique<File>(mcDir, u"options.txt");
    this->loadOptions();
}

jstring GameSettings::getKeybindingDescription(int_t index) {
    if (index < 0 || static_cast<std::size_t>(index) >= keyBindings.size()) {
        return u"";
    }

    const Keybinding *keyBinding = keyBindings[index];
    return keyBinding->keyDescription + u": " + lwjgl::Keyboard::getKeyName(keyBinding->key);
}

void GameSettings::setKeybinding(int_t index, int_t key) {
    if (index < 0 || static_cast<std::size_t>(index) >= keyBindings.size()) {
        return;
    }

    keyBindings[index]->key = key;
    saveOptions();
}

void GameSettings::setOptionValue(int_t index, int_t value) {
    if (index == 0) {
        music = !music;
    }

    if (index == 1) {
        sound = !sound;
    }

    if (index == 2) {
        invertMouse = !invertMouse;
    }

    if (index == 3) {
        showFramerate = !showFramerate;
    }

    if (index == 4) {
        renderDistance = (renderDistance + value) & 3;
    }

    if (index == 5) {
        viewBobbing = !viewBobbing;
    }

    if (index == 6) {
        anaglyph = !anaglyph;
    }

    if (index == 7) {
        limitFramerate = !limitFramerate;
    }

    if (index == 8) {
        difficulty = (difficulty + value) & 3;
    }

    if (index == 9) {
        fancyGraphics = !fancyGraphics;
    }

    saveOptions();
}

jstring GameSettings::getOptionDisplayString(int_t index) {
    if (index == 0) {
        return u"Music: " + onOff(music);
    }
    if (index == 1) {
        return u"Sound: " + onOff(sound);
    }
    if (index == 2) {
        return u"Invert mouse: " + onOff(invertMouse);
    }
    if (index == 3) {
        return u"Show FPS: " + onOff(showFramerate);
    }
    if (index == 4) {
        return u"Render distance: " + renderDistances[renderDistance];
    }
    if (index == 5) {
        return u"View bobbing: " + onOff(viewBobbing);
    }
    if (index == 6) {
        return u"3d anaglyph: " + onOff(anaglyph);
    }
    if (index == 7) {
        return u"Limit framerate: " + onOff(limitFramerate);
    }
    if (index == 8) {
        return u"Difficulty: " + difficultyLevels[difficulty];
    }
    if (index == 9) {
        return u"Graphics: " + jstring(fancyGraphics ? u"FANCY" : u"FAST");
    }
    return u"";
}

void GameSettings::loadOptions() {
    try {
        if (!optionsFile->exists()) {
            return;
        }

        std::ifstream in(optionsFile->toUtf8());
        std::string line;

        while (std::getline(in, line)) {
            std::string key;
            std::string value;
            if (!splitOptionLine(line, key, value)) {
                continue;
            }

            if (key == "music") {
                music = parseBool(value);
            }

            if (key == "sound") {
                sound = parseBool(value);
            }

            if (key == "invertYMouse") {
                invertMouse = parseBool(value);
            }

            if (key == "showFrameRate") {
                showFramerate = parseBool(value);
            }

            if (key == "viewDistance") {
                renderDistance = std::stoi(value);
            }

            if (key == "bobView") {
                viewBobbing = parseBool(value);
            }

            if (key == "anaglyph3d") {
                anaglyph = parseBool(value);
            }

            if (key == "limitFramerate") {
                limitFramerate = parseBool(value);
            }

            if (key == "difficulty") {
                difficulty = std::stoi(value);
            }

            if (key == "fancyGraphics") {
                fancyGraphics = parseBool(value);
            }

            for (Keybinding *keyBinding: keyBindings) {
                if (key == "key_" + String::toUtf8(keyBinding->keyDescription)) {
                    keyBinding->key = std::stoi(value);
                }
            }
        }
    } catch (std::exception &e) {
        printf("Failed to load options\n%s\n", e.what());
    }
}

void GameSettings::saveOptions() {
    const File parent = optionsFile->getParentFile();
    if (!parent.exists() && !parent.mkdirs()) {
        return;
    }

    std::ofstream out(optionsFile->toUtf8(), std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        return;
    }

    out << std::boolalpha;
    out << "music:" << music << '\n';
    out << "sound:" << sound << '\n';
    out << "invertYMouse:" << invertMouse << '\n';
    out << "showFrameRate:" << showFramerate << '\n';
    out << "viewDistance:" << renderDistance << '\n';
    out << "bobView:" << viewBobbing << '\n';
    out << "anaglyph3d:" << anaglyph << '\n';
    out << "limitFramerate:" << limitFramerate << '\n';
    out << "difficulty:" << difficulty << '\n';
    out << "fancyGraphics:" << fancyGraphics << '\n';

    for (const Keybinding *keyBinding: keyBindings) {
        out << "key_" << String::toUtf8(keyBinding->keyDescription) << ':' << keyBinding->key << '\n';
    }
}

// Util
jstring GameSettings::onOff(const bool value) {
    return value ? u"ON" : u"OFF";
}

bool GameSettings::parseBool(const std::string &value) {
    return value == "true";
}

bool GameSettings::splitOptionLine(const std::string &line, std::string &key, std::string &value) {
    const std::size_t separator = line.find(':');
    if (separator == std::string::npos) {
        return false;
    }

    key = line.substr(0, separator);
    value = line.substr(separator + 1);
    return true;
}
