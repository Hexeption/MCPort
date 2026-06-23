//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_LEVELLISTENER_H
#define MCPORT_LEVELLISTENER_H

class LevelListener {
public:
    virtual ~LevelListener() = default;

    virtual void tileChanged(int x, int y, int z) {
    }

    virtual void lightColumnChanged(int var1, int var2, int var3, int var4) {
    }

    virtual void allChanged() {
    }
};

#endif //MCPORT_LEVELLISTENER_H
