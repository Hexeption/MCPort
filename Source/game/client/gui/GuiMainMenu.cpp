//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiMainMenu.h"

#include <ctime>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <memory>

#include <glad/glad.h>

#include "GuiButton.h"
#include "GuiOptions.h"
#include "GuiSelectWorld.h"
#include "LogoEffectRandomizer.h"
#include "game/block/Block.h"
#include "game/client/MathHelper.h"
#include "game/client/Minecraft.h"
#include "game/client/renderer/RenderBlocks.h"
#include "game/client/renderer/ScaledResolution.h"
#include "game/client/renderer/Tessellator.h"
#include "utils/GLU.h"

Random GuiMainMenu::rand;

GuiMainMenu::GuiMainMenu() {
    const std::vector<jstring> var1 = readSplashes();
    if (!var1.empty()) {
        splashString = var1[rand.nextInt(static_cast<int_t>(var1.size()))];
    }
}

GuiMainMenu::~GuiMainMenu() = default;

std::vector<jstring> GuiMainMenu::readSplashes() {
    const std::array<std::filesystem::path, 3> paths = {
        std::filesystem::path("Assets/title/splashes.txt"),
        std::filesystem::path("../Assets/title/splashes.txt"),
        std::filesystem::path("../../Assets/title/splashes.txt")
    };

    std::ifstream stream;
    for (const auto &path: paths) {
        stream.open(path);
        if (stream.is_open()) {
            break;
        }
        stream.clear();
    }

    std::vector<jstring> splashes;
    std::string line;
    while (std::getline(stream, line)) {
        const std::string::size_type first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            continue;
        }
        const std::string::size_type last = line.find_last_not_of(" \t\r\n");
        splashes.push_back(String::fromUtf8(line.substr(first, last - first + 1)));
    }
    return splashes;
}

void GuiMainMenu::updateScreen() {
    if (!logoEffects.empty()) {
        for (auto &var1: logoEffects) {
            for (auto &var2: var1) {
                var2->updateLogoEffects();
            }
        }
    }
}

void GuiMainMenu::keyTyped(char_t, int_t) {
}

void GuiMainMenu::initGui() {
    const std::time_t now = std::time(nullptr);
    const std::tm *var1 = std::localtime(&now);
    if (var1 != nullptr) {
        const int_t month = var1->tm_mon + 1;
        const int_t day = var1->tm_mday;
        if (month == 11 && day == 9) {
            splashString = u"Happy birthday, ez!";
        } else if (month == 6 && day == 1) {
            splashString = u"Happy birthday, Notch!";
        } else if (month == 12 && day == 24) {
            splashString = u"Merry X-mas!";
        } else if (month == 1 && day == 1) {
            splashString = u"Happy new year!";
        } else if (month == 7 && day == 18) {
            splashString = u"Happy birthday, Hexy!";
        }
    }

    controlList.clear();
    controlList.push_back(std::make_unique<GuiButton>(1, width / 2 - 100, height / 4 + 48, u"Singleplayer"));
    controlList.push_back(std::make_unique<GuiButton>(2, width / 2 - 100, height / 4 + 72, u"Multiplayer"));
    controlList.push_back(std::make_unique<GuiButton>(3, width / 2 - 100, height / 4 + 96, u"Play tutorial level"));
    controlList.push_back(std::make_unique<GuiButton>(0, width / 2 - 100, height / 4 + 120 + 12, u"Options..."));
    controlList[1]->enabled = false;
    controlList[2]->enabled = false;
}

void GuiMainMenu::actionPerformed(GuiButton *button) {
    if (button->id == 0) {
        mc->displayGuiScreen(std::make_shared<GuiOptions>(shared_from_this(), mc->options.get()));
    }

    if (button->id == 1) {
        mc->displayGuiScreen(std::make_shared<GuiSelectWorld>(shared_from_this()));
    }
}

void GuiMainMenu::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    Tessellator &tessellator = Tessellator::instance;
    drawLogo(partialTicks);
    glBindTexture(GL_TEXTURE_2D, mc->renderEngine->getTexture(u"/gui/logo.png"));
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    tessellator.setColorOpaque_I(16777215);
    glPushMatrix();
    glTranslatef(static_cast<float>(width / 2 + 90), 70.0f, 0.0f);
    glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
    float var5 = 1.8f - MathHelper::abs(MathHelper::sin(
                                            static_cast<float>(System::currentTimeMillis() % 1000L) / 1000.0f *
                                            static_cast<float>(std::acos(-1.0)) * 2.0f) * 0.1f);
    var5 = var5 * 100.0f / static_cast<float>(fontRenderer->getStringWidth(splashString) + 32);
    glScalef(var5, var5, var5);
    drawCenteredString(fontRenderer, splashString, 0, -8, 16776960);
    glPopMatrix();
    jstring copyright = u"Copyright Mojang Specifications. Do not distribute.";
    drawString(fontRenderer, copyright, width - fontRenderer->getStringWidth(copyright) - 2, height - 10, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}

void GuiMainMenu::drawLogo(const float partialTicks) {
    int_t var3;
    if (logoEffects.empty()) {
        logoEffects.resize(logoBlockLayers[0].size());
        for (int_t var2 = 0; var2 < static_cast<int_t>(logoEffects.size()); ++var2) {
            logoEffects[var2].reserve(logoBlockLayers.size());
            for (var3 = 0; var3 < static_cast<int_t>(logoBlockLayers.size()); ++var3) {
                logoEffects[var2].push_back(std::make_unique<LogoEffectRandomizer>(this, var2, var3));
            }
        }
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    ScaledResolution var14(mc->displayWidth, mc->displayHeight);
    var3 = 120 * var14.scaleFactor;
    GLU::gluPerspective(70.0f, static_cast<float>(mc->displayWidth) / static_cast<float>(var3), 0.05f, 100.0f);
    glViewport(0, mc->displayHeight - var3, mc->displayWidth, var3);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(true);

    for (int_t var4 = 0; var4 < 3; ++var4) {
        glPushMatrix();
        glTranslatef(0.4f, 0.6f, -12.0f);
        if (var4 == 0) {
            glClear(GL_DEPTH_BUFFER_BIT);
            glTranslatef(0.0f, -0.4f, 0.0f);
            glScalef(0.98f, 1.0f, 1.0f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if (var4 == 1) {
            glDisable(GL_BLEND);
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        if (var4 == 2) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_COLOR, GL_ONE);
        }

        glScalef(1.0f, -1.0f, 1.0f);
        glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.89f, 1.0f, 0.4f);
        glTranslatef(static_cast<float>(-static_cast<int_t>(logoBlockLayers[0].size())) * 0.5f,
                     static_cast<float>(-static_cast<int_t>(logoBlockLayers.size())) * 0.5f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, mc->renderEngine->getTexture(u"/terrain.png"));
        if (var4 == 0) {
            glBindTexture(GL_TEXTURE_2D, mc->renderEngine->getTexture(u"/title/black.png"));
        }

        RenderBlocks var5;
        for (int_t var6 = 0; var6 < static_cast<int_t>(logoBlockLayers.size()); ++var6) {
            for (int_t var7 = 0; var7 < static_cast<int_t>(logoBlockLayers[var6].size()); ++var7) {
                const char16_t var8 = logoBlockLayers[var6][var7];
                if (var8 != u' ') {
                    glPushMatrix();
                    LogoEffectRandomizer *var9 = logoEffects[var7][var6].get();
                    float var10 = static_cast<float>(var9->prevHeight + (var9->height - var9->prevHeight) *
                                                     static_cast<double>(partialTicks));
                    float var11 = 1.0f;
                    float var12 = 1.0f;
                    const float var13 = 0.0f;
                    if (var4 == 0) {
                        var11 = var10 * 0.04f + 1.0f;
                        var12 = 1.0f / var11;
                        var10 = 0.0f;
                    }

                    glTranslatef(static_cast<float>(var7), static_cast<float>(var6), var10);
                    glScalef(var11, var11, var11);
                    glRotatef(var13, 0.0f, 1.0f, 0.0f);
                    var5.renderBlockAsItem(Block::stone, var12);
                    glPopMatrix();
                }
            }
        }

        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glViewport(0, 0, mc->displayWidth, mc->displayHeight);
    glEnable(GL_CULL_FACE);
}

Random &GuiMainMenu::getRandom() {
    return rand;
}
