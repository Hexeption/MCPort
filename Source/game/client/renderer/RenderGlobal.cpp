//
// Created by Keir Davis on 24/06/2026.
//

#include "RenderGlobal.h"

#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <vector>

#include <glad/glad.h>

#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "Tessellator.h"
#include "WorldRenderer.h"
#include "game/client/MathHelper.h"
#include "game/block/Block.h"
#include "game/client/Minecraft.h"
#include "game/client/options/GameSettings.h"
#include "game/entity/EntityPlayer.h"
#include "game/phys/ICamera.h"
#include "game/phys/AxisAlignedBB.h"
#include "game/phys/MovingObjectPosition.h"
#include "game/util/Vec3D.h"
#include "java/System.h"
#include "game/world/World.h"

RenderGlobal::RenderGlobal(Minecraft &minecraft, RenderEngine &renderEngine) : minecraft(minecraft),
                                                                               renderEngine(renderEngine),
                                                                               globalRenderBlocks(
                                                                                   std::make_unique<RenderBlocks>()) {
    constexpr int_t maxRendererGrid = 64;
    glRenderListBase = static_cast<int_t>(glGenLists(maxRendererGrid * maxRendererGrid * maxRendererGrid * 3));
    starGLCallList = static_cast<int_t>(glGenLists(3));
    glPushMatrix();
    glNewList(static_cast<GLuint>(starGLCallList), GL_COMPILE);
    renderStars();
    glEndList();
    glPopMatrix();

    glSkyList = starGLCallList + 1;
    glNewList(static_cast<GLuint>(glSkyList), GL_COMPILE);
    Tessellator &tessellator = Tessellator::instance;
    constexpr int_t skySize = 64;
    const int_t skyRepeat = 256 / skySize + 2;
    const float skyHeight = 16.0f;
    for (int_t x = -skySize * skyRepeat; x <= skySize * skyRepeat; x += skySize) {
        for (int_t z = -skySize * skyRepeat; z <= skySize * skyRepeat; z += skySize) {
            tessellator.startDrawingQuads();
            tessellator.addVertex(static_cast<double>(x), static_cast<double>(skyHeight), static_cast<double>(z));
            tessellator.addVertex(static_cast<double>(x + skySize), static_cast<double>(skyHeight),
                                  static_cast<double>(z));
            tessellator.addVertex(static_cast<double>(x + skySize), static_cast<double>(skyHeight),
                                  static_cast<double>(z + skySize));
            tessellator.addVertex(static_cast<double>(x), static_cast<double>(skyHeight),
                                  static_cast<double>(z + skySize));
            tessellator.draw();
        }
    }
    glEndList();

    glSkyList2 = starGLCallList + 2;
    glNewList(static_cast<GLuint>(glSkyList2), GL_COMPILE);
    const float skyHeight2 = -16.0f;
    for (int_t x = -skySize * skyRepeat; x <= skySize * skyRepeat; x += skySize) {
        for (int_t z = -skySize * skyRepeat; z <= skySize * skyRepeat; z += skySize) {
            tessellator.startDrawingQuads();
            tessellator.addVertex(static_cast<double>(x + skySize), static_cast<double>(skyHeight2),
                                  static_cast<double>(z));
            tessellator.addVertex(static_cast<double>(x), static_cast<double>(skyHeight2),
                                  static_cast<double>(z));
            tessellator.addVertex(static_cast<double>(x), static_cast<double>(skyHeight2),
                                  static_cast<double>(z + skySize));
            tessellator.addVertex(static_cast<double>(x + skySize), static_cast<double>(skyHeight2),
                                  static_cast<double>(z + skySize));
            tessellator.draw();
        }
    }
    glEndList();
}

RenderGlobal::~RenderGlobal() = default;

void RenderGlobal::changeWorld(World *world) {
    if (theWorld != nullptr) {
        theWorld->setRenderGlobal(nullptr);
    }
    for (auto &renderer: worldRenderers) {
        if (renderer != nullptr) {
            renderer->stopRendering();
        }
    }
    minBlockX = minBlockY = minBlockZ = 0;
    maxBlockX = maxBlockY = maxBlockZ = 0;
    worldRenderers.clear();
    sortedWorldRenderers.clear();
    worldRenderersToUpdate.clear();
    prevSortX = -9999.0;
    prevSortY = -9999.0;
    prevSortZ = -9999.0;
    theWorld = world;
    if (theWorld != nullptr) {
        globalRenderBlocks = std::make_unique<RenderBlocks>(*theWorld);
    } else {
        globalRenderBlocks = std::make_unique<RenderBlocks>();
    }
    if (theWorld != nullptr) {
        theWorld->setRenderGlobal(this);
        loadRenderers();
    }
}

int_t RenderGlobal::getRenderRadius() const {
    if (minecraft.options == nullptr) {
        return 4;
    }

    switch (minecraft.options->renderDistance) {
        case 0:
            return 5;
        case 1:
            return 4;
        case 2:
            return 3;
        default:
            return 2;
    }
}

WorldRenderer *RenderGlobal::getWorldRenderer(const int_t x, const int_t y, const int_t z) {
    if (x < 0 || y < 0 || z < 0 || x >= renderChunksWide || y >= renderChunksTall || z >= renderChunksDeep) {
        return nullptr;
    }
    const int_t index = (z * renderChunksTall + y) * renderChunksWide + x;
    return index >= 0 && index < static_cast<int_t>(worldRenderers.size()) ? worldRenderers[index].get() : nullptr;
}

void RenderGlobal::loadRenderers() {
    renderDistance = minecraft.options != nullptr ? minecraft.options->renderDistance : 0;
    for (auto &renderer: worldRenderers) {
        if (renderer != nullptr) {
            renderer->stopRendering();
        }
    }

    int_t renderDistanceBlocks = 64 << (3 - renderDistance);
    if (renderDistanceBlocks > 400) {
        renderDistanceBlocks = 400;
    }

    renderChunksWide = renderDistanceBlocks / 16 + 1;
    renderChunksTall = 8;
    renderChunksDeep = renderDistanceBlocks / 16 + 1;
    worldRenderers.clear();
    sortedWorldRenderers.clear();
    worldRenderersToUpdate.clear();
    const int_t rendererCount = renderChunksWide * renderChunksTall * renderChunksDeep;
    worldRenderers.resize(static_cast<size_t>(rendererCount));
    sortedWorldRenderers.reserve(static_cast<size_t>(rendererCount));
    minBlockX = minBlockY = minBlockZ = 0;
    maxBlockX = renderChunksWide;
    maxBlockY = renderChunksTall;
    maxBlockZ = renderChunksDeep;

    int_t displayListOffset = 0;
    for (int_t x = 0; x < renderChunksWide; ++x) {
        for (int_t y = 0; y < renderChunksTall; ++y) {
            for (int_t z = 0; z < renderChunksDeep; ++z) {
                auto renderer = std::make_unique<WorldRenderer>(theWorld, x * 16, y * 16, z * 16, 16,
                                                                glRenderListBase + displayListOffset);
                renderer->isWaitingOnOcclusionQuery = false;
                renderer->isVisible = true;
                renderer->isInFrustum = true;
                renderer->markDirty();
                const int_t index = (z * renderChunksTall + y) * renderChunksWide + x;
                sortedWorldRenderers.push_back(renderer.get());
                worldRenderersToUpdate.push_back(renderer.get());
                worldRenderers[index] = std::move(renderer);
                displayListOffset += 3;
            }
        }
    }

    if (theWorld != nullptr && minecraft.thePlayer != nullptr) {
        markRenderersForNewPosition(MathHelper::floor_double(minecraft.thePlayer->posX),
                                    MathHelper::floor_double(minecraft.thePlayer->posY),
                                    MathHelper::floor_double(minecraft.thePlayer->posZ));
        EntityPlayer &player = *minecraft.thePlayer;
        std::sort(sortedWorldRenderers.begin(), sortedWorldRenderers.end(),
                  [&player](const WorldRenderer *lhs, const WorldRenderer *rhs) {
                      return lhs->distanceToEntitySquared(player) < rhs->distanceToEntitySquared(player);
                  });
    }
}

void RenderGlobal::markRenderersForNewPosition(int_t x, int_t, int_t z) {
    x -= 8;
    z -= 8;
    minBlockX = minBlockY = minBlockZ = std::numeric_limits<int_t>::max();
    maxBlockX = maxBlockY = maxBlockZ = std::numeric_limits<int_t>::min();
    const int_t widthBlocks = renderChunksWide * 16;
    const int_t halfWidth = widthBlocks / 2;

    for (int_t rendererX = 0; rendererX < renderChunksWide; ++rendererX) {
        int_t blockX = rendererX * 16;
        int_t wrapX = blockX + halfWidth - x;
        if (wrapX < 0) {
            wrapX -= widthBlocks - 1;
        }
        wrapX /= widthBlocks;
        blockX -= wrapX * widthBlocks;
        minBlockX = std::min(minBlockX, blockX);
        maxBlockX = std::max(maxBlockX, blockX);

        for (int_t rendererZ = 0; rendererZ < renderChunksDeep; ++rendererZ) {
            int_t blockZ = rendererZ * 16;
            int_t wrapZ = blockZ + halfWidth - z;
            if (wrapZ < 0) {
                wrapZ -= widthBlocks - 1;
            }
            wrapZ /= widthBlocks;
            blockZ -= wrapZ * widthBlocks;
            minBlockZ = std::min(minBlockZ, blockZ);
            maxBlockZ = std::max(maxBlockZ, blockZ);

            for (int_t rendererY = 0; rendererY < renderChunksTall; ++rendererY) {
                const int_t blockY = rendererY * 16;
                minBlockY = std::min(minBlockY, blockY);
                maxBlockY = std::max(maxBlockY, blockY);
                WorldRenderer *renderer = getWorldRenderer(rendererX, rendererY, rendererZ);
                if (renderer == nullptr) {
                    continue;
                }
                const bool wasDirty = renderer->needsUpdate;
                renderer->setPosition(blockX, blockY, blockZ);
                if (!wasDirty && renderer->needsUpdate) {
                    worldRenderersToUpdate.push_back(renderer);
                }
            }
        }
    }
}

void RenderGlobal::renderWorld(float) {
    if (theWorld == nullptr) {
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glDepthMask(true);
    glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/terrain.png"));

    theWorld->updatingLighting();

    if (minecraft.thePlayer == nullptr) {
        return;
    }

    updateRenderers(*minecraft.thePlayer, false);
    sortAndRender(*minecraft.thePlayer, 0, 0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sortAndRender(*minecraft.thePlayer, 1, 0.0);
    glDisable(GL_BLEND);
}

void RenderGlobal::clipRenderersByFrustum(ICamera &camera, float) {
    for (int_t i = 0; i < static_cast<int_t>(worldRenderers.size()); ++i) {
        auto &renderer = worldRenderers[i];
        if (renderer != nullptr && !renderer->skipAllRenderPasses() &&
            (!renderer->isInFrustum || ((i + frustumCheckOffset) & 15) == 0)) {
            renderer->updateInFrustum(camera);
        }
    }
    ++frustumCheckOffset;
}

bool RenderGlobal::updateRenderers(EntityPlayer &player, bool force) {
    std::sort(worldRenderersToUpdate.begin(), worldRenderersToUpdate.end(),
              [&player](const WorldRenderer *lhs, const WorldRenderer *rhs) {
                  if (lhs->isInFrustum && !rhs->isInFrustum) {
                      return false;
                  }
                  if (rhs->isInFrustum && !lhs->isInFrustum) {
                      return true;
                  }
                  return lhs->distanceToEntitySquared(player) > rhs->distanceToEntitySquared(player);
              });

    const int_t updateCount = static_cast<int_t>(worldRenderersToUpdate.size());
    const int_t lastIndex = updateCount - 1;
    for (int_t processed = 0; processed < updateCount; ++processed) {
        WorldRenderer *renderer = worldRenderersToUpdate[lastIndex - processed];
        if (renderer == nullptr) {
            continue;
        }

        if (!force) {
            if (renderer->distanceToEntitySquared(player) > 1024.0f) {
                if (renderer->isInFrustum) {
                    if (processed >= 3) {
                        return false;
                    }
                } else if (processed >= 1) {
                    return false;
                }
            }
        } else if (!renderer->isInFrustum) {
            continue;
        }

        renderer->updateRenderer(*globalRenderBlocks);
        const auto found = std::find(worldRenderersToUpdate.begin(), worldRenderersToUpdate.end(), renderer);
        if (found != worldRenderersToUpdate.end()) {
            worldRenderersToUpdate.erase(found);
        }
        renderer->needsUpdate = false;
    }

    return worldRenderersToUpdate.empty();
}

int_t RenderGlobal::sortAndRender(EntityPlayer &player, int_t pass, double partialTicks) {
    if (theWorld == nullptr) {
        return 0;
    }

    if (minecraft.options != nullptr && minecraft.options->renderDistance != renderDistance) {
        loadRenderers();
    }

    if (pass == 0) {
        renderersLoaded = 0;
        renderersBeingClipped = 0;
        renderersBeingOccluded = 0;
        renderersBeingRendered = 0;
        renderersSkippingRenderPass = 0;
    }

    const double dx = player.posX - prevSortX;
    const double dy = player.posY - prevSortY;
    const double dz = player.posZ - prevSortZ;
    if (dx * dx + dy * dy + dz * dz > 16.0) {
        prevSortX = player.posX;
        prevSortY = player.posY;
        prevSortZ = player.posZ;
        markRenderersForNewPosition(MathHelper::floor_double(player.posX), MathHelper::floor_double(player.posY),
                                    MathHelper::floor_double(player.posZ));
        std::sort(sortedWorldRenderers.begin(), sortedWorldRenderers.end(),
                  [&player](const WorldRenderer *lhs, const WorldRenderer *rhs) {
                      return lhs->distanceToEntitySquared(player) < rhs->distanceToEntitySquared(player);
                  });
    }

    return renderSortedRenderers(0, static_cast<int_t>(sortedWorldRenderers.size()), pass, partialTicks);
}

int_t RenderGlobal::renderSortedRenderers(const int_t start, const int_t end, const int_t pass,
                                          const double partialTicks) {
    glRenderLists.clear();
    for (auto &list: allRenderLists) {
        list.reset();
    }

    EntityPlayer *player = minecraft.thePlayer.get();
    if (player == nullptr) {
        return 0;
    }

    const double playerRenderX = player->lastTickPosX + (player->posX - player->lastTickPosX) * partialTicks;
    const double playerRenderY = player->lastTickPosY + (player->posY - player->lastTickPosY) * partialTicks;
    const double playerRenderZ = player->lastTickPosZ + (player->posZ - player->lastTickPosZ) * partialTicks;

    int_t rendered = 0;
    const int_t clampedEnd = std::min(end, static_cast<int_t>(sortedWorldRenderers.size()));
    for (int_t i = start; i < clampedEnd; ++i) {
        WorldRenderer *renderer = sortedWorldRenderers[i];
        if (renderer == nullptr) {
            continue;
        }
        if (pass == 0) {
            ++renderersLoaded;
            if (renderer->skipRenderPass[pass]) {
                ++renderersSkippingRenderPass;
            } else if (!renderer->isInFrustum) {
                ++renderersBeingClipped;
            } else if (!renderer->isVisible) {
                ++renderersBeingOccluded;
            } else {
                ++renderersBeingRendered;
            }
        }

        if (!renderer->skipRenderPass[pass] && renderer->isInFrustum && renderer->isVisible) {
            const int_t glCallList = renderer->getGLCallListForPass(pass);
            if (glCallList >= 0) {
                glRenderLists.push_back(renderer);
                ++rendered;
            }
        }
    }

    int_t activeLists = 0;
    for (WorldRenderer *renderer: glRenderLists) {
        int_t listIndex = -1;
        for (int_t i = 0; i < activeLists; ++i) {
            if (allRenderLists[i].isRenderedAt(renderer->posXMinus, renderer->posYMinus, renderer->posZMinus)) {
                listIndex = i;
            }
        }
        if (listIndex < 0) {
            listIndex = activeLists++;
            allRenderLists[listIndex].setLocation(renderer->posXMinus, renderer->posYMinus, renderer->posZMinus,
                                                  playerRenderX, playerRenderY, playerRenderZ);
        }
        allRenderLists[listIndex].render(renderer->getGLCallListForPass(pass));
    }

    renderAllRenderLists(pass, partialTicks);
    return rendered;
}

void RenderGlobal::renderAllRenderLists(int_t pass, double partialTicks) {
    (void) pass;
    (void) partialTicks;
    for (auto &list: allRenderLists) {
        list.render();
    }
}

void RenderGlobal::updateClouds() {
    ++cloudTickCounter;
}

void RenderGlobal::renderSky(float) {
    if (theWorld == nullptr) {
        return;
    }

    const std::unique_ptr<Vec3D> skyColor = theWorld->getSkyColor(0.0f);
    float red = static_cast<float>(skyColor->xCoord);
    float green = static_cast<float>(skyColor->yCoord);
    float blue = static_cast<float>(skyColor->zCoord);

    if (minecraft.options != nullptr && minecraft.options->anaglyph) {
        const float gray = (red * 30.0f + green * 59.0f + blue * 11.0f) / 100.0f;
        const float anaglyphGreen = (red * 30.0f + green * 70.0f) / 100.0f;
        const float anaglyphBlue = (red * 30.0f + blue * 70.0f) / 100.0f;
        red = gray;
        green = anaglyphGreen;
        blue = anaglyphBlue;
    }

    glDisable(GL_TEXTURE_2D);
    glColor3f(red, green, blue);
    glDepthMask(false);
    glEnable(GL_FOG);
    glCallList(static_cast<GLuint>(glSkyList));
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glPushMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glRotatef(theWorld->getCelestialAngle(0.0f) * 360.0f, 1.0f, 0.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/terrain/sun.png"));
    Tessellator &tessellator = Tessellator::instance;
    constexpr float sunSize = 30.0f;
    tessellator.startDrawingQuads();
    tessellator.addVertexWithUV(-sunSize, 100.0, -sunSize, 0.0, 0.0);
    tessellator.addVertexWithUV(sunSize, 100.0, -sunSize, 1.0, 0.0);
    tessellator.addVertexWithUV(sunSize, 100.0, sunSize, 1.0, 1.0);
    tessellator.addVertexWithUV(-sunSize, 100.0, sunSize, 0.0, 1.0);
    tessellator.draw();

    constexpr float moonSize = 20.0f;
    glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/terrain/moon.png"));
    tessellator.startDrawingQuads();
    tessellator.addVertexWithUV(-moonSize, -100.0, moonSize, 1.0, 1.0);
    tessellator.addVertexWithUV(moonSize, -100.0, moonSize, 0.0, 1.0);
    tessellator.addVertexWithUV(moonSize, -100.0, -moonSize, 0.0, 0.0);
    tessellator.addVertexWithUV(-moonSize, -100.0, -moonSize, 1.0, 0.0);
    tessellator.draw();

    glDisable(GL_TEXTURE_2D);
    const float starBrightness = theWorld->getStarBrightness(0.0f);
    if (starBrightness > 0.0f) {
        glColor4f(starBrightness, starBrightness, starBrightness, starBrightness);
        glCallList(static_cast<GLuint>(starGLCallList));
    }

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_FOG);
    glPopMatrix();
    glColor3f(red * 0.2f + 0.04f, green * 0.2f + 0.04f, blue * 0.6f + 0.1f);
    glDisable(GL_TEXTURE_2D);
    glCallList(static_cast<GLuint>(glSkyList2));
    glEnable(GL_TEXTURE_2D);
    glDepthMask(true);
}

void RenderGlobal::renderClouds(float partialTicks) {
    if (minecraft.options != nullptr && minecraft.options->fancyGraphics) {
        renderCloudsFancy(partialTicks);
        return;
    }

    if (theWorld == nullptr || minecraft.thePlayer == nullptr) {
        return;
    }

    glDisable(GL_CULL_FACE);
    const float playerY = static_cast<float>(minecraft.thePlayer->lastTickPosY +
                                              (minecraft.thePlayer->posY - minecraft.thePlayer->lastTickPosY) *
                                              static_cast<double>(partialTicks));
    constexpr int_t cloudSize = 32;
    const int_t cloudRepeat = 256 / cloudSize;
    Tessellator &tessellator = Tessellator::instance;
    glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/clouds.png"));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::unique_ptr<Vec3D> cloudColor = theWorld->getCloudColor(partialTicks);
    float red = static_cast<float>(cloudColor->xCoord);
    float green = static_cast<float>(cloudColor->yCoord);
    float blue = static_cast<float>(cloudColor->zCoord);
    if (minecraft.options != nullptr && minecraft.options->anaglyph) {
        const float gray = (red * 30.0f + green * 59.0f + blue * 11.0f) / 100.0f;
        const float anaglyphGreen = (red * 30.0f + green * 70.0f) / 100.0f;
        const float anaglyphBlue = (red * 30.0f + blue * 70.0f) / 100.0f;
        red = gray;
        green = anaglyphGreen;
        blue = anaglyphBlue;
    }

    const float texScale = 0.5f / 1024.0f;
    double cloudX = minecraft.thePlayer->prevPosX +
                    (minecraft.thePlayer->posX - minecraft.thePlayer->prevPosX) *
                    static_cast<double>(partialTicks) +
                    static_cast<double>((static_cast<float>(cloudTickCounter) + partialTicks) * 0.03f);
    double cloudZ = minecraft.thePlayer->prevPosZ +
                    (minecraft.thePlayer->posZ - minecraft.thePlayer->prevPosZ) *
                    static_cast<double>(partialTicks);
    const int_t cloudTileX = MathHelper::floor_double(cloudX / 2048.0);
    const int_t cloudTileZ = MathHelper::floor_double(cloudZ / 2048.0);
    cloudX -= static_cast<double>(cloudTileX * 2048);
    cloudZ -= static_cast<double>(cloudTileZ * 2048);
    const float cloudY = 120.0f - playerY + 0.33f;
    const float cloudOffsetX = static_cast<float>(cloudX * static_cast<double>(texScale));
    const float cloudOffsetZ = static_cast<float>(cloudZ * static_cast<double>(texScale));
    tessellator.startDrawingQuads();
    tessellator.setColorRGBA_F(red, green, blue, 0.8f);
    for (int_t x = -cloudSize * cloudRepeat; x < cloudSize * cloudRepeat; x += cloudSize) {
        for (int_t z = -cloudSize * cloudRepeat; z < cloudSize * cloudRepeat; z += cloudSize) {
            tessellator.addVertexWithUV(static_cast<double>(x), static_cast<double>(cloudY),
                                        static_cast<double>(z + cloudSize),
                                        static_cast<double>(static_cast<float>(x) * texScale + cloudOffsetX),
                                        static_cast<double>(static_cast<float>(z + cloudSize) * texScale +
                                                            cloudOffsetZ));
            tessellator.addVertexWithUV(static_cast<double>(x + cloudSize), static_cast<double>(cloudY),
                                        static_cast<double>(z + cloudSize),
                                        static_cast<double>(static_cast<float>(x + cloudSize) * texScale +
                                                            cloudOffsetX),
                                        static_cast<double>(static_cast<float>(z + cloudSize) * texScale +
                                                            cloudOffsetZ));
            tessellator.addVertexWithUV(static_cast<double>(x + cloudSize), static_cast<double>(cloudY),
                                        static_cast<double>(z),
                                        static_cast<double>(static_cast<float>(x + cloudSize) * texScale +
                                                            cloudOffsetX),
                                        static_cast<double>(static_cast<float>(z) * texScale + cloudOffsetZ));
            tessellator.addVertexWithUV(static_cast<double>(x), static_cast<double>(cloudY), static_cast<double>(z),
                                        static_cast<double>(static_cast<float>(x) * texScale + cloudOffsetX),
                                        static_cast<double>(static_cast<float>(z) * texScale + cloudOffsetZ));
        }
    }
    tessellator.draw();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}

void RenderGlobal::renderCloudsFancy(float) {
    if (theWorld == nullptr || minecraft.thePlayer == nullptr) {
        return;
    }

    glDisable(GL_CULL_FACE);
    const float playerY = static_cast<float>(minecraft.thePlayer->lastTickPosY +
                                              (minecraft.thePlayer->posY - minecraft.thePlayer->lastTickPosY));
    Tessellator &tessellator = Tessellator::instance;
    glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/clouds.png"));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::unique_ptr<Vec3D> cloudColor = theWorld->getCloudColor(0.0f);
    float red = static_cast<float>(cloudColor->xCoord);
    float green = static_cast<float>(cloudColor->yCoord);
    float blue = static_cast<float>(cloudColor->zCoord);
    if (minecraft.options != nullptr && minecraft.options->anaglyph) {
        const float gray = (red * 30.0f + green * 59.0f + blue * 11.0f) / 100.0f;
        const float anaglyphGreen = (red * 30.0f + green * 70.0f) / 100.0f;
        const float anaglyphBlue = (red * 30.0f + blue * 70.0f) / 100.0f;
        red = gray;
        green = anaglyphGreen;
        blue = anaglyphBlue;
    }

    glScalef(12.0f, 1.0f, 12.0f);
    tessellator.startDrawingQuads();
    tessellator.setColorRGBA_F(red, green, blue, 0.8f);
    tessellator.addVertex(-10.0, 128.0 - playerY, -10.0);
    tessellator.addVertex(10.0, 128.0 - playerY, -10.0);
    tessellator.addVertex(10.0, 128.0 - playerY, 10.0);
    tessellator.addVertex(-10.0, 128.0 - playerY, 10.0);
    tessellator.draw();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}

void RenderGlobal::renderStars() {
    Tessellator &tessellator = Tessellator::instance;
    tessellator.startDrawingQuads();
    for (int_t i = 0; i < 1500; ++i) {
        const double x = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        const double y = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        const double z = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        const double scale = 0.25 + static_cast<double>(rand()) / RAND_MAX * 0.25;
        const double distSq = x * x + y * y + z * z;
        if (distSq < 1.0 && distSq > 0.01) {
            const double inv = 1.0 / std::sqrt(distSq);
            const double sx = x * inv * 100.0;
            const double sy = y * inv * 100.0;
            const double sz = z * inv * 100.0;
            tessellator.addVertex(sx - scale, sy - scale, sz);
            tessellator.addVertex(sx + scale, sy - scale, sz);
            tessellator.addVertex(sx + scale, sy + scale, sz);
            tessellator.addVertex(sx - scale, sy + scale, sz);
        }
    }
    tessellator.draw();
}

void RenderGlobal::renderEntities(const Vec3D &cameraPos, ICamera &camera, const float partialTicks) {
    if (theWorld == nullptr) {
        return;
    }

    if (minecraft.fontRenderer != nullptr && minecraft.thePlayer != nullptr && minecraft.options != nullptr) {
        RenderManager::instance.cacheActiveRenderInfo(*theWorld, renderEngine, *minecraft.fontRenderer,
                                                      *minecraft.thePlayer, *minecraft.options, partialTicks);
    }

    countEntitiesTotal = 0;
    countEntitiesRendered = 0;
    countEntitiesHidden = 0;

    RenderManager::renderPosX = cameraPos.xCoord;
    RenderManager::renderPosY = cameraPos.yCoord;
    RenderManager::renderPosZ = cameraPos.zCoord;

    const auto &entities = theWorld->getLoadedEntityList();
    countEntitiesTotal = static_cast<int_t>(entities.size());

    for (const auto &entityPtr: entities) {
        if (entityPtr == nullptr) {
            ++countEntitiesHidden;
            continue;
        }

        Entity &entity = *entityPtr;
        if (entity.isInRangeToRenderVec3D(cameraPos) && camera.isBoundingBoxInFrustum(entity.boundingBox) &&
            (&entity != minecraft.thePlayer.get() || minecraft.options->thirdPersonView)) {
            ++countEntitiesRendered;
            RenderManager::instance.renderEntity(entity, partialTicks);
        }
    }
}

void RenderGlobal::drawBlockBreaking(EntityPlayer &player, const MovingObjectPosition &hit, int_t mode,
                                     float partialTicks) {
    if (theWorld == nullptr || hit.typeOfHit != 0) {
        return;
    }

    Tessellator &tessellator = Tessellator::instance;
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 1.0f, 1.0f,
              (MathHelper::sin(static_cast<float>(System::currentTimeMillis()) / 100.0f) * 0.2f + 0.4f) * 0.5f);

    if (mode == 0 && damagePartialTime > 0.0f) {
        glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
        glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/terrain.png"));
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        glPushMatrix();
        const int_t blockId = theWorld->getBlockId(hit.blockX, hit.blockY, hit.blockZ);
        Block *block = blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                           ? Block::blocksList[blockId]
                           : nullptr;
        glDisable(GL_ALPHA_TEST);
        glPolygonOffset(-3.0f, -3.0f);
        glEnable(GL_POLYGON_OFFSET_FILL);
        tessellator.startDrawingQuads();
        const double playerX = player.lastTickPosX + (player.posX - player.lastTickPosX) * partialTicks;
        const double playerY = player.lastTickPosY + (player.posY - player.lastTickPosY) * partialTicks;
        const double playerZ = player.lastTickPosZ + (player.posZ - player.lastTickPosZ) * partialTicks;
        tessellator.setTranslationD(-playerX, -playerY, -playerZ);
        tessellator.disableColor();
        if (block == nullptr) {
            block = Block::stone;
        }

        globalRenderBlocks->renderBlockUsingTexture(block, hit.blockX, hit.blockY, hit.blockZ,
                                                    240 + static_cast<int_t>(damagePartialTime * 10.0f));
        tessellator.draw();
        tessellator.setTranslationD(0.0, 0.0, 0.0);
        glPolygonOffset(0.0f, 0.0f);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glEnable(GL_ALPHA_TEST);
        glDepthMask(true);
        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
}

void RenderGlobal::drawSelectionBox(EntityPlayer &player, const MovingObjectPosition &hit, const int_t mode,
                                    const float partialTicks) {
    if (theWorld == nullptr || mode != 0 || hit.typeOfHit != 0) {
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
    glLineWidth(2.0f);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDepthMask(false);

    constexpr float expandAmount = 0.002f;
    const int_t blockId = theWorld->getBlockId(hit.blockX, hit.blockY, hit.blockZ);
    Block *block = blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    if (block != nullptr) {
        block->setBlockBoundsBasedOnState(*theWorld, hit.blockX, hit.blockY, hit.blockZ);
        const double playerX = player.lastTickPosX + (player.posX - player.lastTickPosX) * partialTicks;
        const double playerY = player.lastTickPosY + (player.posY - player.lastTickPosY) * partialTicks;
        const double playerZ = player.lastTickPosZ + (player.posZ - player.lastTickPosZ) * partialTicks;
        const AxisAlignedBB box = block->getSelectedBoundingBoxFromPool(*theWorld, hit.blockX, hit.blockY, hit.blockZ)
                .expand(static_cast<double>(expandAmount), static_cast<double>(expandAmount),
                        static_cast<double>(expandAmount))
                .getOffsetBoundingBox(-playerX, -playerY, -playerZ);
        drawOutlinedBoundingBox(box);
    }

    glDepthMask(true);
    glEnable(GL_FOG);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void RenderGlobal::drawOutlinedBoundingBox(const AxisAlignedBB &box) {
    Tessellator &tessellator = Tessellator::instance;
    tessellator.startDrawing(GL_LINE_STRIP);
    tessellator.addVertex(box.minX, box.minY, box.minZ);
    tessellator.addVertex(box.maxX, box.minY, box.minZ);
    tessellator.addVertex(box.maxX, box.minY, box.maxZ);
    tessellator.addVertex(box.minX, box.minY, box.maxZ);
    tessellator.addVertex(box.minX, box.minY, box.minZ);
    tessellator.draw();

    tessellator.startDrawing(GL_LINE_STRIP);
    tessellator.addVertex(box.minX, box.maxY, box.minZ);
    tessellator.addVertex(box.maxX, box.maxY, box.minZ);
    tessellator.addVertex(box.maxX, box.maxY, box.maxZ);
    tessellator.addVertex(box.minX, box.maxY, box.maxZ);
    tessellator.addVertex(box.minX, box.maxY, box.minZ);
    tessellator.draw();

    tessellator.startDrawing(GL_LINES);
    tessellator.addVertex(box.minX, box.minY, box.minZ);
    tessellator.addVertex(box.minX, box.maxY, box.minZ);
    tessellator.addVertex(box.maxX, box.minY, box.minZ);
    tessellator.addVertex(box.maxX, box.maxY, box.minZ);
    tessellator.addVertex(box.maxX, box.minY, box.maxZ);
    tessellator.addVertex(box.maxX, box.maxY, box.maxZ);
    tessellator.addVertex(box.minX, box.minY, box.maxZ);
    tessellator.addVertex(box.minX, box.maxY, box.maxZ);
    tessellator.draw();
}

void RenderGlobal::markBlockRangeNeedsUpdate(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1) {
    if (renderChunksWide <= 0 || renderChunksTall <= 0 || renderChunksDeep <= 0) {
        return;
    }

    x0 -= 1;
    y0 -= 1;
    z0 -= 1;
    x1 += 1;
    y1 += 1;
    z1 += 1;

    const int_t minChunkX = MathHelper::bucketInt(x0, 16);
    const int_t minChunkY = MathHelper::bucketInt(y0, 16);
    const int_t minChunkZ = MathHelper::bucketInt(z0, 16);
    const int_t maxChunkX = MathHelper::bucketInt(x1, 16);
    const int_t maxChunkY = MathHelper::bucketInt(y1, 16);
    const int_t maxChunkZ = MathHelper::bucketInt(z1, 16);

    for (int_t chunkX = minChunkX; chunkX <= maxChunkX; ++chunkX) {
        int_t rendererX = chunkX % renderChunksWide;
        if (rendererX < 0) {
            rendererX += renderChunksWide;
        }

        for (int_t chunkY = minChunkY; chunkY <= maxChunkY; ++chunkY) {
            int_t rendererY = chunkY % renderChunksTall;
            if (rendererY < 0) {
                rendererY += renderChunksTall;
            }

            for (int_t chunkZ = minChunkZ; chunkZ <= maxChunkZ; ++chunkZ) {
                int_t rendererZ = chunkZ % renderChunksDeep;
                if (rendererZ < 0) {
                    rendererZ += renderChunksDeep;
                }

                WorldRenderer *renderer = getWorldRenderer(rendererX, rendererY, rendererZ);
                if (renderer == nullptr) {
                    continue;
                }
                if (!renderer->needsUpdate) {
                    worldRenderersToUpdate.push_back(renderer);
                }
                renderer->markDirty();
            }
        }
    }
}

void RenderGlobal::updateAllRenderers() {
    for (auto &renderer: worldRenderers) {
        if (renderer != nullptr && renderer->isChunkLit) {
            if (!renderer->needsUpdate) {
                worldRenderersToUpdate.push_back(renderer.get());
            }
            renderer->markDirty();
        }
    }
}

jstring RenderGlobal::getDebugInfoRenders() {
    return u"C: " + String::toString(renderersBeingRendered) + u"/" + String::toString(renderersLoaded) + u". F: " +
           String::toString(renderersBeingClipped) + u", O: " +
           String::toString(renderersBeingOccluded) + u", E: " + String::toString(renderersSkippingRenderPass);
}

jstring RenderGlobal::getDebugInfoEntities() {
    return u"E: " + String::toString(countEntitiesRendered) + u"/" + String::toString(countEntitiesTotal) +
           u". B: " + String::toString(countEntitiesHidden) + u", I: " +
           String::toString(countEntitiesTotal - countEntitiesHidden - countEntitiesRendered);
}
