//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_WORLD_H
#define MCPORT_WORLD_H

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include "EnumSkyBlock.h"
#include "IBlockAccess.h"
#include "game/block/Material.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/phys/AxisAlignedBB.h"
#include "game/phys/MovingObjectPosition.h"
#include "game/util/Vec3D.h"
#include "game/world/MetadataChunkBlock.h"
#include "game/world/NextTickListEntry.h"
#include "game/world/chunk/Chunk.h"
#include "game/world/chunk/ChunkProviderGenerate.h"
#include "java/File.h"
#include "java/Random.h"
#include "java/Type.h"

class Entity;
class EntityPlayer;
class RenderGlobal;

class World : public IBlockAccess {
public:
    static std::array<float, 16> lightBrightnessTable;

    long_t worldTime = 0;
    bool snowCovered = false;
    long_t skyColor = 8961023L;
    long_t fogColor = 12638463L;
    long_t cloudColor = 16777215L;
    int_t skylightSubtracted = 0;
    int_t updateLCG = Random().nextInt();
    int_t DIST_HASH_MAGIC = 1013904223;
    bool editingBlocks = false;
    int_t autosavePeriod = 40;
    int_t difficultySetting = 0;
    Random rand;
    int_t spawnX = 0;
    int_t spawnY = 64;
    int_t spawnZ = 0;
    bool isNewWorld = false;
    File saveDirectory;
    long_t randomSeed = 0;
    long_t sizeOnDisk = 0;
    jstring levelName;
    bool worldChunkLoadOverride = false;
    bool multiplayerWorld = false;
    RenderGlobal *renderGlobal = nullptr;
    std::vector<std::unique_ptr<Entity> > loadedEntityList;
    std::vector<Entity *> unloadedEntityList;
    std::vector<EntityPlayer *> playerEntities;
    EntityPlayer *playerToSave = nullptr;
    std::unique_ptr<NBTTagCompound> savedPlayerData;

    const std::vector<std::unique_ptr<Entity> > &getLoadedEntityList() const;

    static std::unique_ptr<NBTTagCompound> getLevelData(const File &minecraftDir, const jstring &levelName);

    static void deleteWorld(const File &minecraftDir, const jstring &levelName);

    World(const File &savesDirectory, const jstring &levelName);

    World(const File &savesDirectory, const jstring &levelName, long_t seed);

    int_t getBlockId(int_t x, int_t y, int_t z) const override;

    bool blockExists(int_t x, int_t y, int_t z) const;

    bool checkChunksExist(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1) const;

    bool setBlockAndMetadata(int_t x, int_t y, int_t z, int_t blockId, int_t metadata);

    bool setBlock(int_t x, int_t y, int_t z, int_t blockId);

    Material *getBlockMaterial(int_t x, int_t y, int_t z) const override;

    int_t getBlockMetadata(int_t x, int_t y, int_t z) const override;

    void setBlockMetadataWithNotify(int_t x, int_t y, int_t z, int_t metadata);

    bool setBlockMetadata(int_t x, int_t y, int_t z, int_t metadata);

    bool setBlockWithNotify(int_t x, int_t y, int_t z, int_t blockId);

    bool setBlockAndMetadataWithNotify(int_t x, int_t y, int_t z, int_t blockId, int_t metadata);

    void markBlockNeedsUpdate(int_t x, int_t y, int_t z);

    void notifyBlockChange(int_t x, int_t y, int_t z, int_t blockId);

    void markBlockAsNeedsUpdate(int_t x, int_t z, int_t y0, int_t y1);

    void markBlocksDirtyVertical(int_t x, int_t z, int_t y0, int_t y1);

    void markBlocksDirty(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    void notifyBlocksOfNeighborChange(int_t x, int_t y, int_t z, int_t blockId);

    bool canBlockSeeTheSky(int_t x, int_t y, int_t z) const;

    int_t getBlockLightValue(int_t x, int_t y, int_t z) const;

    int_t getBlockLightValue_do(int_t x, int_t y, int_t z, bool checkNeighbors) const;

    bool canExistingBlockSeeTheSky(int_t x, int_t y, int_t z) const;

    bool isAirBlock(int_t x, int_t y, int_t z) const;

    float getBrightness(int_t x, int_t y, int_t z) const override;

    bool isDayTime() const;

    bool isDaytime() const;

    int_t calculateSkylightSubtracted(float partialTicks) const;

    std::unique_ptr<Vec3D> getSkyColor(float partialTicks) const;

    float getCelestialAngle(float partialTicks) const;

    std::unique_ptr<Vec3D> getCloudColor(float partialTicks) const;

    std::unique_ptr<Vec3D> getFogColor(float partialTicks) const;

    int_t getTopSolidOrLiquidBlock(int_t x, int_t z) const;

    int_t getPrecipitationHeight(int_t x, int_t z) const;

    float getStarBrightness(float partialTicks) const;

    bool isBlockNormalCube(int_t x, int_t y, int_t z) const override;

    std::vector<AxisAlignedBB> getCollidingBoundingBoxes(const Entity &entity, const AxisAlignedBB &box) const;

    std::vector<Entity *> getEntitiesWithinAABBExcludingEntity(const Entity &entity, const AxisAlignedBB &box) const;

    bool getIsAnyLiquid(const AxisAlignedBB &box) const;

    bool handleMaterialAcceleration(const AxisAlignedBB &box, Material *material, Entity &entity);

    bool isMaterialInBB(const AxisAlignedBB &box, Material *material) const;

    void saveWorld(bool saveChunks, void *progressUpdate);

    bool saveWorld(int_t progressStep);

    void saveWorldIndirectly(void *progressUpdate);

    bool updatingLighting();

    void tick();

    int_t getHeightValue(int_t x, int_t z) const;

    void neighborLightPropagationChanged(EnumSkyBlock skyBlock, int_t x, int_t y, int_t z, int_t newLightValue);

    int_t getSavedLightValue(EnumSkyBlock skyBlock, int_t x, int_t y, int_t z) const;

    void setLightValue(EnumSkyBlock skyBlock, int_t x, int_t y, int_t z, int_t lightValue);

    void scheduleLightingUpdate(EnumSkyBlock skyBlock, int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    void scheduleLightingUpdate_do(EnumSkyBlock skyBlock, int_t x0, int_t y0, int_t z0, int_t x1, int_t y1,
                                   int_t z1, bool checkPrevious);

    void scheduleBlockUpdate(int_t x, int_t y, int_t z, int_t blockId);

    bool tickUpdates(bool runAllPending);

    void randomDisplayUpdates(int_t x, int_t y, int_t z);

    bool canBlockBePlacedAt(int_t blockId, int_t x, int_t y, int_t z, bool ignoreCollision);

    void setSpawnLocation();

    void sendQuittingDisconnectingPacket();

    void checkSessionLock() const;

    void setWorldTime(long_t time);

    void setRenderGlobal(RenderGlobal *renderer);

    MovingObjectPosition rayTraceBlocks(const Vec3D &start, const Vec3D &end);

    MovingObjectPosition rayTraceBlocks_do(const Vec3D &start, const Vec3D &end, bool ignoreLiquid);

    bool spawnEntityInWorld(std::unique_ptr<Entity> entity);

    bool spawnEntityInWorld(Entity *entity);

    void setEntityDead(Entity &entity);

    void updateEntities();

    void setPlayerToSave(EntityPlayer *player);

    bool loadPlayerData(EntityPlayer &player);

private:
    long_t lockTimestamp;
    std::unordered_map<long_t, int_t> blockOverrides;
    std::unordered_map<long_t, int_t> metadataOverrides;
    std::vector<NextTickListEntry> scheduledTickEntries;
    std::vector<MetadataChunkBlock> lightingToUpdate;
    mutable std::unordered_map<long_t, std::unique_ptr<Chunk> > loadedChunks;
    std::unique_ptr<ChunkProviderGenerate> chunkProvider;

    static void deleteWorldFiles(const std::vector<File> &files);

    static long_t getDirectorySize(const File &directory);

    static long_t blockKey(int_t x, int_t y, int_t z);

    static bool isValidBlockPosition(int_t x, int_t y, int_t z);

    static int_t blockToChunkCoord(int_t blockCoord);

    static int_t blockToLocalCoord(int_t blockCoord);

    static long_t chunkKey(int_t chunkX, int_t chunkZ);

    static std::array<float, 16> makeLightBrightnessTable();

    static int_t getDefaultLightValue(EnumSkyBlock skyBlock);

    static bool isSpawnBeach(int_t x, int_t z);

    static bool isEarlierScheduledTick(const NextTickListEntry &left, const NextTickListEntry &right);

    void createSessionLock() const;

    void updateEntity(Entity &entity);

    void saveLevel();

    void saveChunks();

    void saveChunk(const Chunk &chunk) const;

    std::unique_ptr<Chunk> loadChunkFromDisk(int_t chunkX, int_t chunkZ) const;

    void loadChunkEntities(NBTTagCompound &chunkData);

    File getChunksDirectory() const;

    File getChunkFile(int_t chunkX, int_t chunkZ) const;

    bool findSpawn(int_t x, int_t z) const;

    int_t getInitialSpawnLocation(int_t x, int_t z) const;

    int_t getFirstUncoveredBlock(int_t x, int_t z) const;

    int_t getGeneratedBlockId(int_t x, int_t y, int_t z) const;

    void calculateInitialSkylight();

    bool chunkExists(int_t chunkX, int_t chunkZ) const;

    Chunk &getChunkFromChunkCoords(int_t chunkX, int_t chunkZ) const;

    Chunk &getChunkFromBlockCoords(int_t x, int_t z) const;

protected:
    void obtainEntitySkin(Entity &entity);

    void releaseEntitySkin(Entity &entity);
};

#endif //MCPORT_WORLD_H
