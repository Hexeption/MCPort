//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CRAFTINGRECIPE_H
#define MCPORT_CRAFTINGRECIPE_H

#include <vector>
#include "game/item/ItemStack.h"

class CraftingRecipe {
private:
    int_t width;
    int_t height;
    std::vector<int_t> ingredientMap;
    ItemStack resultStack;

public:
    int_t resultId;

    CraftingRecipe(int_t width, int_t height, std::vector<int_t> ingredients, ItemStack result);

    bool matches(const std::vector<int_t> &grid) const;

    ItemStack getCraftingResult(const std::vector<int_t> &grid) const;

    int_t getRecipeSize() const;

private:
    bool checkMatch(const std::vector<int_t> &grid, int_t offX, int_t offY, bool mirror) const;
};

#endif //MCPORT_CRAFTINGRECIPE_H
