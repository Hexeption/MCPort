//
// Created by Keir Davis on 26/06/2026.
//

#include "RecipesTools.h"

#include "CraftingManager.h"
#include "game/block/Block.h"
#include "game/item/Item.h"
#include "game/item/ItemStack.h"

void RecipesTools::addRecipes(CraftingManager &manager) {
    const std::string recipePatterns[4][3] = {
        {"XXX", " # ", " # "},
        {"X", "#", "#"},
        {"XX", "X#", " #"},
        {"XX", " #", " #"}
    };

    const int_t materialIds[5] = {
        Block::planks->blockID,
        Block::cobblestone->blockID,
        Item::ingotIron->shiftedIndex,
        Item::diamond->shiftedIndex,
        Item::ingotGold->shiftedIndex
    };

    Item *pickaxes[5] = {Item::pickaxeWood, Item::pickaxeStone, Item::pickaxeSteel, Item::pickaxeDiamond, Item::pickaxeGold};
    Item *shovels[5]  = {Item::shovelWood,  Item::shovelStone,  Item::shovel,        Item::shovelDiamond,  Item::shovelGold};
    Item *axes[5]     = {Item::axeWood,     Item::axeStone,     Item::axeSteel,      Item::axeDiamond,     Item::axeGold};
    Item *hoes[5]     = {Item::hoeWood,     Item::hoeStone,     Item::hoeSteel,      Item::hoeDiamond,     Item::hoeGold};

    Item **toolSets[4] = {pickaxes, shovels, axes, hoes};

    for (int_t mat = 0; mat < 5; ++mat) {
        for (int_t toolType = 0; toolType < 4; ++toolType) {
            std::vector<std::string> rows(recipePatterns[toolType], recipePatterns[toolType] + 3);
            manager.addRecipe(
                ItemStack(*toolSets[toolType][mat]),
                rows,
                {{'#', Item::stick->shiftedIndex}, {'X', materialIds[mat]}}
            );
        }
    }
}
