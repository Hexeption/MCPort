//
// Created by Keir Davis on 22/06/2026.
//

#include "Timer.h"

#include "java/System.h"

Timer::Timer(float ticksPerSecond) {
    this->ticksPerSecond = ticksPerSecond;
    this->lastTime = System::nanoTime();
}

void Timer::advanceTime() {
    long_t now = System::nanoTime();
    long_t passedNs = now - this->lastTime;
    this->lastTime = now;
    if (passedNs < 0L) {
        passedNs = 0L;
    }

    if (passedNs > 1000000000L) {
        passedNs = 1000000000L;
    }

    this->fps = 1000000000L / passedNs;
    this->passedTime += passedNs * this->timeScale * this->ticksPerSecond / 1.0E9F;
    this->ticks = this->passedTime;
    if (this->ticks > 100) {
        this->ticks = 100;
    }

    this->passedTime -= this->ticks;
    this->a = this->passedTime;
}
