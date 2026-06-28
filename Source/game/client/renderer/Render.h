//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_RENDER_H
#define MCPORT_RENDER_H


#include "game/phys/AxisAlignedBB.h"
#include "java/String.h"
#include "java/Type.h"

#include "game/entity/Entity.h"
#include "game/client/renderer/FontRenderer.h"

class RenderManager;

class Render {
protected:
    RenderManager *renderManager = nullptr;
    float shadowSize = 0.0f;
    float shadowOpaque = 1.0f;

    void loadTexture(const jstring &texture);

    void loadDownloadableImageTexture(const jstring &url, const jstring &fallback);

public:
    virtual ~Render() = default;

    virtual void doRender(Entity &entity, double x, double y, double z, float yaw, float partialTicks) = 0;

    void setRenderManager(RenderManager &manager);

    void doRenderShadowAndFire(Entity &entity, double x, double y, double z, float shadowAlpha, float partialTicks);

    FontRenderer *getFontRendererFromRenderManager();

    static void renderOffsetAABB(AxisAlignedBB &box, double x, double y, double z);

    static void renderAABB(AxisAlignedBB &box);
};


#endif //MCPORT_RENDER_H
