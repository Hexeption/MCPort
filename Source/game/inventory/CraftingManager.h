//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CRAFTINGMANAGER_H
#define MCPORT_CRAFTINGMANAGER_H

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "CraftingRecipe.h"
#include "game/item/ItemStack.h"

class CraftingManager {
private:
    std::vector<CraftingRecipe> recipes;

    CraftingManager();

public:
    static CraftingManager &getInstance();

    void addRecipe(ItemStack result, std::vector<std::string> rows, std::unordered_map<char, int_t> ingredients);

    std::optional<ItemStack> findMatchingRecipe(const std::vector<int_t> &grid);
};

#endif //MCPORT_CRAFTINGMANAGER_H
