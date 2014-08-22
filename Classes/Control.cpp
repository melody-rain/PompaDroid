#include "Control.h"
#include "GameResource.h"

enum
{
    eTagPause
};

Control::Control()
{

}

Control::~Control()
{

}

bool Control::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        Size tSize = Director::getInstance()->getWinSize();

        cache = SpriteFrameCache::getInstance();
        cache->addSpriteFramesWithFile(s_cache);

        pauseMeneItemImage = MenuItemImage::create(s_pause_file, s_play_file, CC_CALLBACK_1(Control::pauseMenuCallback, this));
        Size cSize = pauseMeneItemImage->getContentSize();
        pauseMeneItemImage->setScale(0.3f);

        Menu *pauseMenu = Menu::create(pauseMeneItemImage, NULL);
        pauseMenu->setPosition(Vec2(tSize.width - cSize.width * 0.3, tSize.height - cSize.height * 0.3));

        this->addChild(pauseMenu, 0, eTagPause);
        bRet = true;

    } while (0);
    return true;
}

void Control::pauseMenuCallback(Ref *pSender)
{
    if (!Director::getInstance()->isPaused())
    {
        pauseMeneItemImage->setNormalSpriteFrame(cache->getSpriteFrameByName(s_play));
        pauseMeneItemImage->setSelectedSpriteFrame(cache->getSpriteFrameByName(s_pause));
        Director::getInstance()->pause();
    }
    else
    {
        pauseMeneItemImage->setNormalSpriteFrame(cache->getSpriteFrameByName(s_pause));
        pauseMeneItemImage->setSelectedSpriteFrame(cache->getSpriteFrameByName(s_play));
        Director::getInstance()->resume();
    }
}