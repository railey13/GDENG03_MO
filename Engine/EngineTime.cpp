#include "EngineTime.h"
#include <Windows.h>
#include <iostream>
#include <thread>
#include "Debug.h"

EngineTime* EngineTime::sharedInstance = NULL;

void EngineTime::initialize() {
    sharedInstance = new EngineTime();
}

d64 EngineTime::getDeltaTime() {

    return sharedInstance->deltaTime;
}

d64 EngineTime::getTotalTime() {
    return sharedInstance->totalTime;
}

void EngineTime::LimitFPS(d64 frameRateLimit) {
    if (frameRateLimit <= 0.0f) return;

    d64 timePerFrame = 1.0f / frameRateLimit;

    if (sharedInstance->deltaTime < timePerFrame) {
        d64 sleepMS = (timePerFrame - sharedInstance->deltaTime) * 1000.0f;
        Sleep(static_cast<DWORD>(sleepMS));
    }
    LogFrameEnd();
}

void EngineTime::UpdateFPSCounter() {
    sharedInstance->fpsAccumulatedTime += sharedInstance->deltaTime;
    sharedInstance->fpsFrameCount++;

    if (sharedInstance->fpsAccumulatedTime >= 1.0) {
        sharedInstance->currentFPS = sharedInstance->fpsFrameCount / sharedInstance->fpsAccumulatedTime;

        Debug::Log("FPS: " + std::to_string(sharedInstance->currentFPS));

        sharedInstance->fpsFrameCount = 0;
        sharedInstance->fpsAccumulatedTime = 0.0;
    }
}

d64 EngineTime::getFPS() {
    return sharedInstance->currentFPS;
}

EngineTime::EngineTime() {

}

EngineTime::~EngineTime() {

}

void EngineTime::LogFrameStart() {
    sharedInstance->start = std::chrono::steady_clock::now();
}

void EngineTime::LogFrameEnd() {
    sharedInstance->end = std::chrono::steady_clock::now();

    std::chrono::duration<d64> elapsed_seconds = sharedInstance->end - sharedInstance->start;

    sharedInstance->deltaTime = elapsed_seconds.count();

    sharedInstance->totalTime += sharedInstance->deltaTime;
}
