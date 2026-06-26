//
// Created by Keir Davis on 25/06/2026.
//

#include "GuiIngame.h"

#include "game/client/renderer/EntityRenderer.h"
#include "game/client/renderer/RenderHelper.h"
#include "game/client/renderer/ScaledResolution.h"
#include "game/client/renderer/Tessellator.h"
#include "game/client/renderer/RenderItem.h"
#include "game/inventory/InventoryPlayer.h"
#include "port/java/String.h"
#include "glad/glad.h"

namespace {
    RenderItem itemRenderer;
}

GuiIngame::GuiIngame(Minecraft &mc) : mc(mc) {
}

void GuiIngame::renderInventorySlot(const int_t slot, const int_t x, const int_t y, const float) {
    if (mc.thePlayer == nullptr) {
        return;
    }

    InventoryPlayer &inventory = mc.thePlayer->inventory;
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    itemRenderer.renderItemIntoGUI(*mc.fontRenderer, *mc.renderEngine, inventory.getStackInSlot(slot), x, y);
    itemRenderer.renderItemOverlayIntoGUI(*mc.fontRenderer, *mc.renderEngine, inventory.getStackInSlot(slot), x, y);
    glPopAttrib();
}

void GuiIngame::renderGameOverlay(float partialTicks, bool inScreen, int_t mouseX, int_t mouseY) {
    ScaledResolution var5 = ScaledResolution(mc.displayWidth, mc.displayHeight);
    int scaledWidth = var5.getScaledWidth();
    int scaledHeight = var5.getScaledHeight();
    FontRenderer &fontRenderer = *mc.fontRenderer;
    mc.entityRenderer->setupOverlayRendering();
    glEnable(GL_BLEND);
    if (mc.options->fancyGraphics) {
        renderVignette(mc.thePlayer->getBrightness(partialTicks), scaledWidth, scaledHeight);
    }

    glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/gui/icons.png"));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
    drawTexturedModalRect(scaledWidth / 2 - 7, scaledHeight / 2 - 7, 0, 0, 16, 16);
    glDisable(GL_BLEND);

    if (mc.thePlayer != nullptr) {
        InventoryPlayer &inventory = mc.thePlayer->inventory;
        glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/gui/gui.png"));
        drawTexturedModalRect(scaledWidth / 2 - 91, scaledHeight - 22, 0, 0, 182, 22);
        drawTexturedModalRect(scaledWidth / 2 - 91 - 1 + inventory.currentItem * 20, scaledHeight - 23, 0, 22, 24, 22);

        glPushMatrix();
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        RenderHelper::enableStandardItemLighting();
        glPopMatrix();
        glEnable(GL_RESCALE_NORMAL);
        glEnable(GL_NORMALIZE);

        for (int_t slot = 0; slot < 9; ++slot) {
            const int_t slotX = scaledWidth / 2 - 90 + slot * 20 + 2;
            const int_t slotY = scaledHeight - 16 - 3;
            renderInventorySlot(slot, slotX, slotY, partialTicks);
        }

        glDisable(GL_RESCALE_NORMAL);
        glDisable(GL_NORMALIZE);
        RenderHelper::disableStandardItemLighting();
    }

    jstring var21;
    if (mc.options->showFramerate) {
        fontRenderer.drawStringWithShadow(u"Minecraft Alpha v1.1.0 (" + mc.debug + u")", 2, 2, 16777215);
        fontRenderer.drawStringWithShadow(mc.debugInfoRenders(), 2, 12, 16777215);
        fontRenderer.drawStringWithShadow(mc.getEntityDebug(), 2, 22, 16777215);
        fontRenderer.drawStringWithShadow(mc.debugInfoEntities(), 2, 32, 16777215);
        if (mc.thePlayer != nullptr) {
            float wrappedYaw = mc.thePlayer->rotationYaw;
            while (wrappedYaw <= -180.0f) {
                wrappedYaw += 360.0f;
            }
            while (wrappedYaw > 180.0f) {
                wrappedYaw -= 360.0f;
            }
            fontRenderer.drawStringWithShadow(
                u"Pitch: " + String::toString(mc.thePlayer->rotationPitch) +
                u" Yaw: " + String::toString(wrappedYaw),
                2, 42, 16777215);
        }
    } else {
        fontRenderer.drawStringWithShadow(u"Minecraft Alpha v1.1.0", 2, 2, 16777215);
    }
}

void GuiIngame::renderVignette(float var1, int_t var2, int_t var3) {
    var1 = 1.0F - var1;
    if (var1 < 0.0F) {
        var1 = 0.0F;
    }

    if (var1 > 1.0F) {
        var1 = 1.0F;
    }

    prevVignetteBrightness = (float) ((double) prevVignetteBrightness + (double) (var1 - prevVignetteBrightness) *
                                      0.01);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    glColor4f(prevVignetteBrightness, prevVignetteBrightness, prevVignetteBrightness, 1.0F);
    glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/misc/vignette.png"));
    Tessellator var4 = Tessellator::instance;
    var4.startDrawingQuads();
    var4.addVertexWithUV(0.0, (double) var3, -90.0, 0.0, 1.0);
    var4.addVertexWithUV((double) var2, (double) var3, -90.0, 1.0, 1.0);
    var4.addVertexWithUV((double) var2, 0.0, -90.0, 1.0, 0.0);
    var4.addVertexWithUV(0.0, 0.0, -90.0, 0.0, 0.0);
    var4.draw();
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GuiIngame::updateTick() {
}
