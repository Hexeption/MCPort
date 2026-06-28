//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUIMAINMENU_H
#define MCPORT_GUIMAINMENU_H

#include <array>
#include <memory>
#include <vector>

#include "GuiScreen.h"
#include "java/Random.h"

#include "LogoEffectRandomizer.h"

class GuiMainMenu : public GuiScreen {
private:
    static Random rand;

    std::array<jstring, 5> logoBlockLayers = {
        u" *   * * *   * *** *** *** *** *** ***",
        u" ** ** * **  * *   *   * * * * *    * ",
        u" * * * * * * * **  *   **  *** **   * ",
        u" *   * * *  ** *   *   * * * * *    * ",
        u" *   * * *   * *** *** * * * * *    * "
    };
    std::vector<std::vector<std::unique_ptr<LogoEffectRandomizer> > > logoEffects;
    jstring splashString = u"missingno";

    void drawLogo(float partialTicks);

    static std::vector<jstring> readSplashes();

protected:
    void keyTyped(char_t ch, int_t key) override;

    void actionPerformed(GuiButton *button) override;

public:
    GuiMainMenu();

    ~GuiMainMenu() override;

    void updateScreen() override;

    void initGui() override;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;

    static Random &getRandom();
};

#endif //MCPORT_GUIMAINMENU_H
