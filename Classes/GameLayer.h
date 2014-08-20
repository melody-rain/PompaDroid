#pragma once
#include "cocos2d.h"
#include "Hero.h"
#include "SimpleDPad.h"
#include "HudLayer.h"

class GameLayer : public cocos2d::Layer, public SimpleDPadDelegate
{
public:
    GameLayer(void);
    ~GameLayer(void);

    CREATE_FUNC(GameLayer);
    bool init();
    void initTileMap();
    void initHero();
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

    virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::Point direction);
    virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::Point direction);
    virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad);

    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::SpriteBatchNode *_actors;
    Hero *_hero;
    CC_SYNTHESIZE(HudLayer*, _hud, Hud);
};
