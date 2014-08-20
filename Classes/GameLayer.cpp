#include "GameLayer.h"
using namespace cocos2d;

GameLayer::GameLayer(void)
{
    _tileMap = NULL;
    _hero = NULL;
}

GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!Layer::init());

        this->setTouchEnabled(true);

        SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pd_sprites.plist");
        _actors = SpriteBatchNode::create("pd_sprites.pvr.ccz");
        _actors->getTexture()->setAliasTexParameters();
        this->addChild(_actors, -5);

        this->initHero();
        this->initTileMap();

        bRet = true;
    } while (0);

    return bRet;
}

void GameLayer::initTileMap()
{
    _tileMap = TMXTiledMap::create("pd_tilemap.tmx");
    Ref *pObject = NULL;
    auto& children = _tileMap->getChildren();

    SpriteBatchNode* child = nullptr;

    for (const auto &node : children)
    {
        child = static_cast<SpriteBatchNode*>(node);
        child->getTexture()->setAliasTexParameters();
    }

    this->addChild(_tileMap, -6);
}

void GameLayer::initHero()
{
    _hero = Hero::create();
    _actors->addChild(_hero);
    _hero->setPosition(ccp(_hero->getCenterToSides(), 80));
    _hero->setDesiredPosition(_hero->getPosition());
    _hero->idle();
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    _hero->attack();
}

void GameLayer::didChangeDirectionTo(SimpleDPad *simpleDPad, Point direction)
{

}

void GameLayer::isHoldingDirection(SimpleDPad *simpleDPad, Point direction)
{

}

void GameLayer::simpleDPadTouchEnded(SimpleDPad *simpleDPad)
{

}
