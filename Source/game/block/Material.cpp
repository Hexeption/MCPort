//
// Created by Keir Davis on 24/06/2026.
//

#include "Material.h"

Material *Material::air = nullptr;
Material *Material::grass = nullptr;
Material *Material::wood = nullptr;
Material *Material::rock = nullptr;
Material *Material::iron = nullptr;
Material *Material::water = nullptr;
Material *Material::lava = nullptr;
Material *Material::leaves = nullptr;
Material *Material::plants = nullptr;
Material *Material::sponge = nullptr;
Material *Material::cloth = nullptr;
Material *Material::fire = nullptr;
Material *Material::sand = nullptr;
Material *Material::circuits = nullptr;
Material *Material::glass = nullptr;
Material *Material::tnt = nullptr;
Material *Material::unused = nullptr;
Material *Material::ice = nullptr;
Material *Material::snow = nullptr;
Material *Material::craftedSnow = nullptr;
Material *Material::cactus = nullptr;
Material *Material::clay = nullptr;

void Material::init() {
    if (air != nullptr) {
        return;
    }

    air = new MaterialTransparent();
    grass = new Material();
    wood = (new Material())->setBurning();
    rock = new Material();
    iron = new Material();
    water = new MaterialLiquid();
    lava = new MaterialLiquid();
    leaves = (new Material())->setBurning();
    plants = new MaterialLogic();
    sponge = new Material();
    cloth = (new Material())->setBurning();
    fire = new MaterialTransparent();
    sand = new Material();
    circuits = new MaterialLogic();
    glass = new Material();
    tnt = (new Material())->setBurning();
    unused = new Material();
    ice = new Material();
    snow = new MaterialLogic();
    craftedSnow = new Material();
    cactus = new Material();
    clay = new Material();
}

bool Material::getIsLiquid() const {
    return false;
}

bool Material::isSolid() const {
    return true;
}

bool Material::getCanBlockGrass() const {
    return true;
}

bool Material::getIsSolid() const {
    return true;
}

Material *Material::setBurning() {
    canBurn = true;
    return this;
}

bool Material::getCanBurn() const {
    return canBurn;
}

bool MaterialLiquid::getIsLiquid() const {
    return true;
}

bool MaterialLiquid::isSolid() const {
    return false;
}

bool MaterialLiquid::getIsSolid() const {
    return false;
}

bool MaterialLogic::isSolid() const {
    return false;
}

bool MaterialLogic::getCanBlockGrass() const {
    return false;
}

bool MaterialLogic::getIsSolid() const {
    return false;
}

bool MaterialTransparent::isSolid() const {
    return false;
}

bool MaterialTransparent::getCanBlockGrass() const {
    return false;
}

bool MaterialTransparent::getIsSolid() const {
    return false;
}
