#pragma once
#include "cocos2d.h"

class SimpleDPad;

class SimpleDPadDelegate
{
public:
    virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::Point direction) = 0;
    virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::Point direction) = 0;
    virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad) = 0;
};

class SimpleDPad : public cocos2d::Sprite
{
public:
    SimpleDPad(void);
    ~SimpleDPad(void);

    static SimpleDPad* dPadWithFile(cocos2d::__String *fileName, float radius);
    bool initWithFile(cocos2d::__String *filename, float radius);

    void onEnterTransitionDidFinish();
    void onExit();
    void update(float dt);

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    void updateDirectionForTouchLocation(cocos2d::Point location);

    CC_SYNTHESIZE(SimpleDPadDelegate*, _delegate, Delegate);
    CC_SYNTHESIZE(bool, _isHeld, IsHeld);

protected:
    float _radius;
    cocos2d::Point _direction;
};
