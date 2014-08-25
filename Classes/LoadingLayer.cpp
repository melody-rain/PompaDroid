#include "LoadingLayer.h"
#include "GameResource.h"
#include "GameScene.h"

bool LoadingLayer::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!Layer::init());

        Sprite *loadlingSprite = Sprite::create(s_loading);
        ProgressTimer *loadingBar = ProgressTimer::create(loadlingSprite);
        loadingBar->setType(ProgressTimer::Type::BAR);
        loadingBar->setMidpoint(Vec2(0, 0.5));
        loadingBar->setBarChangeRate(Vec2(1, 0));
        loadingBar->setPercentage(100);
        loadingBar->setScaleX(1.0);
        loadingBar->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 
            Director::getInstance()->getWinSize().height / 2));
        addChild(loadingBar);

        bRet = true;
    } while (0);

    return bRet;
}

Scene *LoadingLayer::scene()
{
    Scene *scene = Scene::create();
    LoadingLayer *loadingLayer = LoadingLayer::create();
    scene->addChild(loadingLayer);
    return scene;
}

void LoadingLayer::onEnterTransitionDidFinish()
{
    Director::getInstance()->replaceScene(GameScene::create());
}