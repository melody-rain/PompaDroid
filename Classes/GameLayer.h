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
    void initRobots();
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

    virtual void didChangeDirectionTo(SimpleDPad *simpleDPad, cocos2d::Point direction);
    virtual void isHoldingDirection(SimpleDPad *simpleDPad, cocos2d::Point direction);
    virtual void simpleDPadTouchEnded(SimpleDPad *simpleDPad);

    void update(float dt);
    void updatePositions();
    void setViewpointCenter(cocos2d::Point position);
    void reorderActors();
    void updateRobots(float dt);

    void endGame();
    void restartGame(cocos2d::CCObject* pSender);
    void onEnterTransitionDidFinish();
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::SpriteBatchNode *_actors;
    Hero *_hero;
    CC_SYNTHESIZE(HudLayer*, _hud, Hud);
    CC_SYNTHESIZE_RETAIN(cocos2d::__Array*, _robots, Robots);
};
