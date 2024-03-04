#include "TimeManager.h"

#include "External/mmgr/mmgr.h"

Timer TimeManager::graphicsTimer;
Timer TimeManager::gameTimer;

float TimeManager::DeltaTime;
int TimeManager::FrameCount;