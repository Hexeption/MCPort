cmake_minimum_required(VERSION 4.2)
project(MCPort)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(MCPORT_BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif ()

option(MCPORT_USE_LOCAL_MCDATA_DIR "Use the process working directory as Minecraft data directory" ON)

add_subdirectory(External)

add_executable(MCPort
        Source/game/client/Minecraft.cpp
        Source/game/client/Minecraft.h
        Source/port/Main.cpp
        Source/port/java/Math.cpp
        Source/port/java/Math.h
        Source/port/java/Random.cpp
        Source/port/java/Random.h
        Source/port/java/BufferedImage.cpp
        Source/port/java/BufferedImage.h
        Source/game/client/renderer/ImageBuffer.h
        Source/port/java/String.cpp
        Source/port/java/String.h
        Source/port/java/System.cpp
        Source/port/java/System.h
        Source/port/java/Type.h
        Source/port/java/DataInput.cpp
        Source/port/java/DataInput.h
        Source/port/java/DataOutput.cpp
        Source/port/java/DataOutput.h
        Source/port/lwjgl/BufferUtils.cpp
        Source/port/lwjgl/BufferUtils.h
        Source/port/lwjgl/Display.cpp
        Source/port/lwjgl/Display.h
        Source/port/lwjgl/GLContext.cpp
        Source/port/lwjgl/GLContext.h
        Source/port/lwjgl/Keyboard.cpp
        Source/port/lwjgl/Keyboard.h
        Source/port/lwjgl/Mouse.cpp
        Source/port/lwjgl/Mouse.h
        Source/port/utils/GLU.cpp
        Source/port/utils/GLU.h
        Source/port/utils/Networking.cpp
        Source/port/utils/Networking.h
        Source/port/utils/SDLException.h
        Source/game/client/Timer.cpp
        Source/game/client/Timer.h
        Source/game/client/MathHelper.cpp
        Source/game/client/MathHelper.h
        Source/game/block/Block.cpp
        Source/game/block/Block.h
        Source/game/block/Material.cpp
        Source/game/block/Material.h
        Source/game/phys/AxisAlignedBB.cpp
        Source/game/phys/AxisAlignedBB.h
        Source/game/phys/MovingObjectPosition.cpp
        Source/game/phys/MovingObjectPosition.h
        Source/game/entity/MovementInput.cpp
        Source/game/entity/MovementInput.h
        Source/game/entity/MovementInputFromOptions.cpp
        Source/game/entity/MovementInputFromOptions.h
        Source/game/entity/Entity.cpp
        Source/game/entity/Entity.h
        Source/game/entity/EntityLiving.cpp
        Source/game/entity/EntityLiving.h
        Source/game/entity/EntityPlayer.cpp
        Source/game/entity/EntityPlayer.h
        Source/game/entity/EntityPlayerSP.cpp
        Source/game/entity/EntityPlayerSP.h
        Source/game/world/chunk/Chunk.cpp
        Source/game/world/chunk/Chunk.h
        Source/game/world/chunk/ChunkProviderGenerate.cpp
        Source/game/world/chunk/ChunkProviderGenerate.h
        Source/game/world/chunk/ChunkCache.cpp
        Source/game/world/chunk/ChunkCache.h
        Source/game/world/gen/NoiseGenerator.h
        Source/game/world/gen/NoiseGeneratorPerlin.cpp
        Source/game/world/gen/NoiseGeneratorPerlin.h
        Source/game/world/gen/NoiseGeneratorOctaves.cpp
        Source/game/world/gen/NoiseGeneratorOctaves.h
        Source/port/java/File.cpp
        Source/port/java/File.h
        Source/port/os/FileSystem.h
        Source/game/client/options/GameSettings.cpp
        Source/game/client/options/GameSettings.h
        Source/game/client/options/Keybinding.h
        Source/game/client/renderer/RenderEngine.cpp
        Source/game/client/renderer/RenderEngine.h
        Source/game/client/renderer/EntityRenderer.cpp
        Source/game/client/renderer/EntityRenderer.h
        Source/game/client/renderer/FontRenderer.cpp
        Source/game/client/renderer/FontRenderer.h
        Source/game/client/renderer/ScaledResolution.cpp
        Source/game/client/renderer/ScaledResolution.h
        Source/game/client/renderer/Tessellator.cpp
        Source/game/client/renderer/Tessellator.h
        Source/game/client/renderer/RenderBlocks.cpp
        Source/game/client/renderer/RenderBlocks.h
        Source/game/client/renderer/WorldRenderer.cpp
        Source/game/client/renderer/WorldRenderer.h
        Source/game/client/renderer/ThreadDownloadImage.cpp
        Source/game/client/renderer/ThreadDownloadImage.h
        Source/game/client/renderer/ThreadDownloadImageData.cpp
        Source/game/client/renderer/ThreadDownloadImageData.h
        Source/game/client/renderer/TextureFX.cpp
        Source/game/client/renderer/TextureFX.h
        Source/game/client/MouseHelper.cpp
        Source/game/client/MouseHelper.h
        Source/game/client/OpenGlCapsChecker.cpp
        Source/game/client/OpenGlCapsChecker.h
        Source/game/client/player/PlayerController.cpp
        Source/game/client/player/PlayerController.h
        Source/game/client/player/PlayerControllerSP.cpp
        Source/game/client/player/PlayerControllerSP.h
        Source/game/client/renderer/RenderGlobal.cpp
        Source/game/client/renderer/RenderGlobal.h
        Source/game/client/gui/Gui.cpp
        Source/game/client/gui/Gui.h
        Source/game/client/gui/GuiButton.cpp
        Source/game/client/gui/GuiButton.h
        Source/game/client/gui/GuiSmallButton.cpp
        Source/game/client/gui/GuiSmallButton.h
        Source/game/client/gui/GuiScreen.cpp
        Source/game/client/gui/GuiScreen.h
        Source/game/client/gui/GuiMainMenu.cpp
        Source/game/client/gui/GuiMainMenu.h
        Source/game/client/gui/GuiIngameMenu.cpp
        Source/game/client/gui/GuiIngameMenu.h
        Source/game/client/gui/GuiDeleteWorld.cpp
        Source/game/client/gui/GuiDeleteWorld.h
        Source/game/client/gui/GuiYesNo.cpp
        Source/game/client/gui/GuiYesNo.h
        Source/game/client/gui/LogoEffectRandomizer.cpp
        Source/game/client/gui/LogoEffectRandomizer.h
        Source/game/client/gui/GuiOptions.cpp
        Source/game/client/gui/GuiOptions.h
        Source/game/client/gui/GuiControls.cpp
        Source/game/client/gui/GuiControls.h
        Source/game/client/gui/GuiSelectWorld.cpp
        Source/game/client/gui/GuiSelectWorld.h
        Source/game/nbt/NBTBase.cpp
        Source/game/nbt/NBTBase.h
        Source/game/nbt/NBTTagByte.cpp
        Source/game/nbt/NBTTagByte.h
        Source/game/nbt/NBTTagByteArray.cpp
        Source/game/nbt/NBTTagByteArray.h
        Source/game/nbt/NBTTagCompound.cpp
        Source/game/nbt/NBTTagCompound.h
        Source/game/nbt/NBTTagDouble.cpp
        Source/game/nbt/NBTTagDouble.h
        Source/game/nbt/NBTTagEnd.cpp
        Source/game/nbt/NBTTagEnd.h
        Source/game/nbt/NBTTagFloat.cpp
        Source/game/nbt/NBTTagFloat.h
        Source/game/nbt/NBTTagInt.cpp
        Source/game/nbt/NBTTagInt.h
        Source/game/nbt/NBTTagList.cpp
        Source/game/nbt/NBTTagList.h
        Source/game/nbt/NBTTagLong.cpp
        Source/game/nbt/NBTTagLong.h
        Source/game/nbt/NBTTagShort.cpp
        Source/game/nbt/NBTTagShort.h
        Source/game/nbt/NBTTagString.cpp
        Source/game/nbt/NBTTagString.h
        Source/game/nbt/CompressedStreamTools.cpp
        Source/game/nbt/CompressedStreamTools.h
        Source/game/util/Vec3D.cpp
        Source/game/util/Vec3D.h
        Source/game/world/NextTickListEntry.cpp
        Source/game/world/NextTickListEntry.h
        Source/game/world/MetadataChunkBlock.cpp
        Source/game/world/MetadataChunkBlock.h
        Source/game/world/World.cpp
        Source/game/world/World.h
        Source/game/world/EnumSkyBlock.h
        Source/game/world/IBlockAccess.h
)

if (APPLE)
    target_sources(MCPort PRIVATE Source/port/os/osx/FileSystem.cpp)
elseif (WIN32)
    target_sources(MCPort PRIVATE Source/port/os/windows/FileSystem.cpp)
else ()
    target_sources(MCPort PRIVATE Source/port/os/linux/FileSystem.cpp)
endif ()

set_target_properties(MCPort PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${MCPORT_BIN_DIR}/$<LOWER_CASE:$<CONFIG>>"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MCPORT_BIN_DIR}/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MCPORT_BIN_DIR}/release"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MCPORT_BIN_DIR}/relwithdebinfo"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${MCPORT_BIN_DIR}/minsizerel"
)

target_include_directories(MCPort PRIVATE "Source")
target_include_directories(MCPort PRIVATE "Source/port")

target_compile_definitions(MCPort PRIVATE
        MCPORT_USE_LOCAL_MCDATA_DIR=$<BOOL:${MCPORT_USE_LOCAL_MCDATA_DIR}>
)

target_link_libraries(MCPort PRIVATE SDL3::SDL3 glad stb ZLIB::ZLIB)

if (WIN32)
    target_link_libraries(MCPort PRIVATE ws2_32)
endif ()

add_custom_command(
        TARGET MCPort POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:MCPort> $<TARGET_RUNTIME_DLLS:MCPort>
        COMMAND_EXPAND_LISTS
)

add_custom_command(
        TARGET MCPort POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:MCPort>/Assets
)
