#pragma once
#include "actionsprite.h"

class Hero : public ActionSprite
{
public:
    Hero(void);
    ~Hero(void);

    CREATE_FUNC(Hero);
    bool init();
    void knockout();

    void updateHP(float hitPoints);
 
    void makeHPBar();

    CC_SYNTHESIZE(cocos2d::ProgressTimer*, heroHPBar, heroHPBar);
    CC_SYNTHESIZE(cocos2d::ProgressTimer*, heroHPBarBg, heroHPBarBg);
};
