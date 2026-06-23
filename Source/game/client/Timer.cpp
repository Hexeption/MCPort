//
// Created by Keir Davis on 23/06/2026.
//

#include "Timer.h"

#include "java/System.h"

Timer::Timer(float ticksPerSecond) : ticksPerSecond(ticksPerSecond) {
    lastSyncSysClock = System::currentTimeMillis();
    lastSyncHRClock = System::nanoTime() / 1000000L;
}

void Timer::updateTimer() {
    long_t var1 = System::currentTimeMillis();
    long_t var3 = var1 - lastSyncSysClock;
    long_t var5 = System::nanoTime() / 1000000L;
    double var9;
    if (var3 > 1000L) {
        long var7 = var5 - lastSyncHRClock;
        var9 = static_cast<double>(var3) / static_cast<double>(var7);
        timeSyncAdjustment += (var9 - timeSyncAdjustment) * static_cast<double>(0.2F);
        lastSyncSysClock = var1;
        lastSyncHRClock = var5;
    }

    if (var3 < 0L) {
        lastSyncSysClock = var1;
        lastSyncHRClock = var5;
    }

    double var11 = static_cast<double>(var5) / 1000.0;
    var9 = (var11 - lastHRTime) * timeSyncAdjustment;
    lastHRTime = var11;
    if (var9 < 0.0) {
        var9 = 0.0;
    }

    if (var9 > 1.0) {
        var9 = 1.0;
    }

    elapsedPartialTicks = static_cast<float>(static_cast<double>(elapsedPartialTicks) + var9 * static_cast<double>(
                                                 timerSpeed) * static_cast<double>(ticksPerSecond));
    elapsedTicks = static_cast<int_t>(elapsedPartialTicks);
    elapsedPartialTicks -= static_cast<float>(elapsedTicks);
    if (elapsedTicks > 10) {
        elapsedTicks = 10;
    }

    renderPartialTicks = elapsedPartialTicks;
}
