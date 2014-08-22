#pragma once
#include "cocos2d.h"
#include "GameLayer.h"
#include "HudLayer.h"
#include "Control.h"

class GameScene : public cocos2d::Scene
{
public:
    GameScene(void);
    ~GameScene(void);

    virtual bool init();
    CREATE_FUNC(GameScene);

    CC_SYNTHESIZE(GameLayer*, _gameLayer, GameLayer);
    CC_SYNTHESIZE(HudLayer*, _hudLayer, HudLayer);
    CC_SYNTHESIZE(Control*, _controller, DController);
};
