//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_MATERIAL_H
#define MCPORT_MATERIAL_H

class Material {
public:
    static Material *air;
    static Material *grass;
    static Material *wood;
    static Material *rock;
    static Material *iron;
    static Material *water;
    static Material *lava;
    static Material *leaves;
    static Material *plants;
    static Material *sponge;
    static Material *cloth;
    static Material *fire;
    static Material *sand;
    static Material *circuits;
    static Material *glass;
    static Material *tnt;
    static Material *unused;
    static Material *ice;
    static Material *snow;
    static Material *craftedSnow;
    static Material *cactus;
    static Material *clay;

    static void init();

    virtual ~Material() = default;

    virtual bool getIsLiquid() const;

    virtual bool isSolid() const;

    virtual bool getCanBlockGrass() const;

    virtual bool getIsSolid() const;

    Material *setBurning();

    bool getCanBurn() const;

private:
    bool canBurn = false;
};

class MaterialLiquid : public Material {
public:
    bool getIsLiquid() const override;

    bool isSolid() const override;

    bool getIsSolid() const override;
};

class MaterialLogic : public Material {
public:
    bool isSolid() const override;

    bool getCanBlockGrass() const override;

    bool getIsSolid() const override;
};

class MaterialTransparent : public Material {
public:
    bool isSolid() const override;

    bool getCanBlockGrass() const override;

    bool getIsSolid() const override;
};

#endif //MCPORT_MATERIAL_H
