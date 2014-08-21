#pragma once
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <sys/timeb.h>
#endif

// 1 - convenience measurements
#define SCREEN Director::getInstance()->getWinSize()
#define CENTER Vec2(SCREEN.width / 2, SCREEN.height / 2)
#define CURTIME GetCurTime()

// 2 - convenience functions
#ifndef UINT64_C
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#define UINT64_C(val) val##ui64
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define UINT64_C(val) val##LLU
#endif
#endif
#define random_range(low, high) (rand() % (high - low + 1)) + low
#define frandom (float)rand() / UINT64_C(0x100000000)
#define frandom_range(low, high) ((high - low) * frandom) + low

// 3 - enumerations
typedef enum _ActionState {
    kActionStateNone = 0,
    kActionStateIdle,
    kActionStateAttack,
    kActionStateWalk,
    kActionStateHurt,
    kActionStateKnockedOut
} ActionState;

// 4 - structures
typedef struct _BoundingBox {
    cocos2d::Rect actual;
    cocos2d::Rect original;
} BoundingBox;

inline float GetCurTime(){
    timeval time;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    cocos2d::gettimeofday(&time, NULL);    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    gettimeofday(&time, NULL);
#endif
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return (float)millisecs;
};