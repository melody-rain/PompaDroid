#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Control : public Layer
{
public:
    Control();
    virtual ~Control();
    bool init();
    void pauseMenuCallback(Ref *pSender);
    CREATE_FUNC(Control);
    CC_SYNTHESIZE(MenuItemImage*, pauseMeneItemImage, pauseMeneItemImage);
    CC_SYNTHESIZE(SpriteFrameCache*, cache, cache);
    void onEnterTransitionDidFinish();
};