#pragma once
#include "cocos2d.h"
#include "SimpleDPad.h"
#include "Control.h"

class HudLayer : public cocos2d::Layer
{
public:
    HudLayer(void);
    ~HudLayer(void);

    bool init();
    CREATE_FUNC(HudLayer);
    CC_SYNTHESIZE(SimpleDPad*, _dPad, DPad);
    CC_SYNTHESIZE(Control*, _controller, DController);
};
