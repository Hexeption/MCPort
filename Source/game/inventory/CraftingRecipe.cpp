//
// Created by Keir Davis on 26/06/2026.
//

#include "CraftingRecipe.h"

CraftingRecipe::CraftingRecipe(int_t w, int_t h, std::vector<int_t> ingredients, ItemStack result)
    : width(w), height(h), ingredientMap(std::move(ingredients)), resultStack(result), resultId(result.itemID) {}

bool CraftingRecipe::matches(const std::vector<int_t> &grid) const {
    for (int_t x = 0; x <= 3 - width; ++x) {
        for (int_t y = 0; y <= 3 - height; ++y) {
            if (checkMatch(grid, x, y, true)) {
                return true;
            }
            if (checkMatch(grid, x, y, false)) {
                return true;
            }
        }
    }
    return false;
}

bool CraftingRecipe::checkMatch(const std::vector<int_t> &grid, int_t offX, int_t offY, bool mirror) const {
    for (int_t x = 0; x < 3; ++x) {
        for (int_t y = 0; y < 3; ++y) {
            int_t rx = x - offX;
            int_t ry = y - offY;
            int_t expected = -1;
            if (rx >= 0 && ry >= 0 && rx < width && ry < height) {
                int_t idx = mirror ? (width - rx - 1 + ry * width) : (rx + ry * width);
                expected = ingredientMap[idx];
            }
            if (grid[x + y * 3] != expected) {
                return false;
            }
        }
    }
    return true;
}

ItemStack CraftingRecipe::getCraftingResult(const std::vector<int_t> &) const {
    return ItemStack(resultStack.itemID, resultStack.stackSize);
}

int_t CraftingRecipe::getRecipeSize() const {
    return width * height;
}
