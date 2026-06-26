//
// Created by Keir Davis on 25/06/2026.
//

#include "Render.h"

#include <glad/glad.h>

#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/RenderManager.h"
#include "game/client/renderer/Tessellator.h"
#include "game/entity/Entity.h"

void Render::loadTexture(const jstring &texture) {
    RenderEngine &engine = *renderManager->renderEngine;
    engine.bindTexture(engine.getTexture(texture));
}

void Render::loadDownloadableImageTexture(const jstring &url, const jstring &fallback) {
    RenderEngine &engine = *renderManager->renderEngine;
    engine.bindTexture(engine.getTextureForDownloadableImage(url, fallback));
}

void Render::setRenderManager(RenderManager &manager) {
    renderManager = &manager;
}

void Render::doRenderShadowAndFire(Entity &entity, double x, double y, double z, float, float) {
}

FontRenderer *Render::getFontRendererFromRenderManager() {
    return renderManager->getFontRenderer();
}

void Render::renderOffsetAABB(AxisAlignedBB &box, double x, double y, double z) {
    glDisable(GL_TEXTURE_2D);

    Tessellator &t = Tessellator::instance;

    t.startDrawingQuads();
    t.setTranslationD(x, y, z);

    t.setNormal(0.0f, 0.0f, -1.0f);
    t.addVertex(box.minX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.minX, box.minY, box.minZ);

    t.setNormal(0.0f, 0.0f, 1.0f);
    t.addVertex(box.minX, box.minY, box.maxZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.maxZ);

    t.setNormal(0.0f, -1.0f, 0.0f);
    t.addVertex(box.minX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);
    t.addVertex(box.minX, box.minY, box.maxZ);

    t.setNormal(0.0f, 1.0f, 0.0f);
    t.addVertex(box.minX, box.maxY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.minX, box.maxY, box.minZ);

    t.setNormal(-1.0f, 0.0f, 0.0f);
    t.addVertex(box.minX, box.minY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.minZ);
    t.addVertex(box.minX, box.minY, box.minZ);

    t.setNormal(1.0f, 0.0f, 0.0f);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);

    t.setTranslationD(0.0, 0.0, 0.0);
    t.draw();

    glEnable(GL_TEXTURE_2D);
}

void Render::renderAABB(AxisAlignedBB &box) {
    Tessellator &t = Tessellator::instance;
    t.startDrawingQuads();

    t.addVertex(box.minX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.minX, box.minY, box.minZ);

    t.addVertex(box.minX, box.minY, box.maxZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.maxZ);

    t.draw();
}
