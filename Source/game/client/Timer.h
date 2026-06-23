//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_TIMER_H
#define MCPORT_TIMER_H
#include "java/Type.h"


class Timer {
public:
    float ticksPerSecond;
    int_t elapsedTicks;
    float renderPartialTicks;
    float timerSpeed = 1.0f;
    float elapsedPartialTicks = 0.0f;

    Timer(float ticksPerSecond);

    void updateTimer();

private:
    double lastHRTime;
    long_t lastSyncSysClock;
    long_t lastSyncHRClock;
    double timeSyncAdjustment = 1.0;
};


#endif //MCPORT_TIMER_H
