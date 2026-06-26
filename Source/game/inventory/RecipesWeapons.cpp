//
// Created by Keir Davis on 26/06/2026.
//

#include "RecipesWeapons.h"

#include "CraftingManager.h"
#include "game/block/Block.h"
#include "game/item/Item.h"
#include "game/item/ItemStack.h"

void RecipesWeapons::addRecipes(CraftingManager &manager) {
    const int_t materialIds[5] = {
        Block::planks->blockID,
        Block::cobblestone->blockID,
        Item::ingotIron->shiftedIndex,
        Item::diamond->shiftedIndex,
        Item::ingotGold->shiftedIndex
    };

    Item *swords[5] = {Item::swordWood, Item::swordStone, Item::swordSteel, Item::swordDiamond, Item::swordGold};

    for (int_t mat = 0; mat < 5; ++mat) {
        manager.addRecipe(
            ItemStack(*swords[mat]),
            {"X", "X", "#"},
            {{'#', Item::stick->shiftedIndex}, {'X', materialIds[mat]}}
        );
    }

    manager.addRecipe(
        ItemStack(*Item::bow, 1),
        {" #X", "# X", " #X"},
        {{'X', Item::silk->shiftedIndex}, {'#', Item::stick->shiftedIndex}}
    );

    manager.addRecipe(
        ItemStack(*Item::arrow, 4),
        {"X", "#", "Y"},
        {{'Y', Item::feather->shiftedIndex}, {'X', Item::flint->shiftedIndex}, {'#', Item::stick->shiftedIndex}}
    );
}
