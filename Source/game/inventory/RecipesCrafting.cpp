//
// Created by Keir Davis on 26/06/2026.
//

#include "RecipesCrafting.h"

#include "CraftingManager.h"
#include "game/block/Block.h"
#include "game/item/ItemStack.h"

void RecipesCrafting::addRecipes(CraftingManager &manager) {
    manager.addRecipe(
        ItemStack(*Block::chest),
        {"###", "# #", "###"},
        {{'#', Block::planks->blockID}}
    );
    manager.addRecipe(
        ItemStack(*Block::stoneOvenIdle),
        {"###", "# #", "###"},
        {{'#', Block::cobblestone->blockID}}
    );
    manager.addRecipe(
        ItemStack(*Block::workbench),
        {"##", "##"},
        {{'#', Block::planks->blockID}}
    );
}
