//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUIDELETEWORLD_H
#define MCPORT_GUIDELETEWORLD_H

#include "GuiSelectWorld.h"

class GuiDeleteWorld : public GuiSelectWorld {
public:
    explicit GuiDeleteWorld(std::shared_ptr<GuiScreen> parentScreen);

    void initButtons() override;

    void selectWorld(int_t id) override;

    void deleteWorld(bool deleting, int_t id) override;
};

#endif //MCPORT_GUIDELETEWORLD_H
