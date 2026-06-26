//
// Created by Keir Davis on 26/06/2026.
//

#include "CraftingManager.h"

#include <algorithm>
#include <iostream>

#include "RecipesCrafting.h"
#include "RecipesTools.h"
#include "RecipesWeapons.h"
#include "game/block/Block.h"
#include "game/item/Item.h"
#include "game/item/ItemStack.h"

CraftingManager::CraftingManager() {
    RecipesTools().addRecipes(*this);
    RecipesWeapons().addRecipes(*this);
    RecipesCrafting().addRecipes(*this);

    addRecipe(ItemStack(*Block::planks, 4), {"#"}, {{'#', Block::wood->blockID}});
    addRecipe(ItemStack(*Item::stick, 4),   {"#", "#"}, {{'#', Block::planks->blockID}});

    std::sort(recipes.begin(), recipes.end(), [](const CraftingRecipe &a, const CraftingRecipe &b) {
        return a.getRecipeSize() > b.getRecipeSize();
    });

    std::cout << recipes.size() << " recipes\n";
}

CraftingManager &CraftingManager::getInstance() {
    static CraftingManager instance;
    return instance;
}

void CraftingManager::addRecipe(ItemStack result, std::vector<std::string> rows,
                                 std::unordered_map<char, int_t> ingredients) {
    std::string pattern;
    int_t width = 0;
    int_t height = 0;

    for (const std::string &row : rows) {
        ++height;
        width = static_cast<int_t>(row.size());
        pattern += row;
    }

    std::vector<int_t> ingredientMap(static_cast<size_t>(width * height));
    for (int_t i = 0; i < width * height; ++i) {
        char c = pattern[static_cast<size_t>(i)];
        auto it = ingredients.find(c);
        ingredientMap[i] = (it != ingredients.end()) ? it->second : -1;
    }

    recipes.emplace_back(width, height, std::move(ingredientMap), result);
}

std::optional<ItemStack> CraftingManager::findMatchingRecipe(const std::vector<int_t> &grid) {
    for (CraftingRecipe &recipe : recipes) {
        if (recipe.matches(grid)) {
            return recipe.getCraftingResult(grid);
        }
    }
    return std::nullopt;
}
