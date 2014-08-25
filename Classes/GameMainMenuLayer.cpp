#include "GameMainMenuLayer.h"
#include "GameResource.h"
#include "GameScene.h"
#include "LoadingLayer.h"

USING_NS_CC;

bool GameMainMenuLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());

        initBackground();

        initMenus();

        bRet = true;

    } while (0);

    return bRet;
}

Scene *GameMainMenuLayer::scene()
{
    Scene *scene = Scene::create();
    GameMainMenuLayer *mainMenuLayer = GameMainMenuLayer::create();
    scene->addChild(mainMenuLayer);
    return scene;
}

void GameMainMenuLayer::initBackground()
{
    Size size = Director::getInstance()->getWinSize();
    Sprite *bgImg = Sprite::create(s_background);
    bgImg->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(bgImg);
}

void GameMainMenuLayer::initMenus()
{
    Size size = Director::getInstance()->getWinSize();
    MenuItemImage *newGameBt = MenuItemImage::create(s_newGameNormal, s_newGameSelected,
        CC_CALLBACK_1(GameMainMenuLayer::onNewGameCallback, this));
    newGameBt->setScale(size.width * 0.25 / newGameBt->getContentSize().width);
    newGameBt->setPosition(Vec2(size.width / 2, size.height / 2));
    newGameBt->setEnabled(true);

    mainMenu = Menu::create(newGameBt, NULL);
    mainMenu->setPosition(Vec2(0, 0));
    addChild(mainMenu);
}

void GameMainMenuLayer::onNewGameCallback(Ref* sender)
{
    Director::getInstance()->replaceScene(LoadingLayer::scene());
}

void GameMainMenuLayer::onEnter()
{
    Layer::onEnter();
    enableMenuItem();
}

void GameMainMenuLayer::enableMenuItem()
{
    auto &children = mainMenu->getChildren();
    for (const auto &child : children)
    {
        ((MenuItemImage*)child)->setEnabled(true);
    }
}

void GameMainMenuLayer::onEnterTransitionDidFinish()
{

}