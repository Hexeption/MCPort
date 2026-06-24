//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERENGINE_H
#define MCPORT_RENDERENGINE_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "java/BufferedImage.h"
#include "ImageBuffer.h"
#include "java/String.h"
#include "java/Type.h"

class GameSettings;
class TextureFX;
class ThreadDownloadImageData;

class RenderEngine {
public:
    static bool useMipmaps;

    explicit RenderEngine(GameSettings *options);

    ~RenderEngine();

    int_t getTexture(const jstring &path);

    int_t allocateAndSetupTexture(const BufferedImage &image);

    void setupTexture(const BufferedImage &image, int_t textureName);

    void deleteTexture(int_t textureName);

    int_t getTextureForDownloadableImage(const jstring &url, const jstring &fallbackTexture);

    ThreadDownloadImageData *obtainImageData(const jstring &url, ImageBuffer *buffer);

    void releaseImageData(const jstring &url);

    void registerTextureFX(TextureFX *textureFX);

    void updateDynamicTextures();

    void refreshTextures();

    void bindTexture(int_t textureName);

private:
    std::unordered_map<jstring, int_t> textureMap;
    std::unordered_map<int_t, BufferedImage> textureContentsMap;
    std::vector<ubyte_t> imageData;
    std::vector<TextureFX *> textureList;
    std::unordered_map<jstring, std::unique_ptr<ThreadDownloadImageData> > urlToImageDataMap;
    GameSettings *options;
    bool clampTexture = false;

    BufferedImage loadImageResource(const jstring &path) const;

    BufferedImage unwrapImageByColumns(const BufferedImage &image) const;

    static int_t averageColor(int_t first, int_t second);

    static int_t alphaBlend(int_t first, int_t second);

    static int_t readInt(const std::vector<ubyte_t> &data, std::size_t offset);

    static void writeInt(std::vector<ubyte_t> &data, std::size_t offset, int_t value);
};

#endif //MCPORT_RENDERENGINE_H
