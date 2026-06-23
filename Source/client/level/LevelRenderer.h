//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_LEVELRENDERER_H
#define MCPORT_LEVELRENDERER_H

#include <memory>
#include <vector>

#include "LevelListener.h"
#include "Chunk.h"
#include "java/Type.h"

class Level;
class Player;

class LevelRenderer : public LevelListener {
public:
    static constexpr int_t MAX_REBUILDS_PER_FRAME = 8;
    static constexpr int_t CHUNK_SIZE = 16;

private:
    Level *level;
    std::vector<std::unique_ptr<Chunk>> chunks;
    int_t xChunks;
    int_t yChunks;
    int_t zChunks;

public:
    explicit LevelRenderer(Level &level);

    std::vector<Chunk *> getAllDirtyChunks();

    void render(Player &player, int_t layer);

    void updateDirtyChunks(Player &player);

    void setDirty(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    void tileChanged(int x, int y, int z) override;

    void lightColumnChanged(int var1, int var2, int var3, int var4) override;

    void allChanged() override;
};


#endif //MCPORT_LEVELRENDERER_H
