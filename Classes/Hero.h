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

    void updateHP();
 
    void makeHPBar();

    CC_SYNTHESIZE(cocos2d::ProgressTimer*, heroHPBar, heroHPBar);
};
