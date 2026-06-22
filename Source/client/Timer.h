//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_TIMER_H
#define MCPORT_TIMER_H
#include "java/Type.h"


class Timer {
private:
    float ticksPerSecond;
    long_t lastTime;

public:
    int_t ticks{};
    float a{};
    float timeScale = 1.0f;
    float fps = 0.0f;
    float passedTime = 0.0f;

public:
    Timer(float ticksPerSecond);

    void advanceTime();
};


#endif //MCPORT_TIMER_H
