#include "Hero.h"
using namespace cocos2d;

Hero::Hero(void)
{
}

Hero::~Hero(void)
{
}

bool Hero::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("hero_idle_00.png"));

        int i;
        //idle animation
        Vector<SpriteFrame*> idleFrames(6);
        for (i = 0; i < 6; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(__String::createWithFormat("hero_idle_%02d.png", i)->getCString());
            idleFrames.pushBack(frame);
        }
        Animation *idleAnimation = Animation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
        this->setIdleAction(CCRepeatForever::create(CCAnimate::create(idleAnimation)));

        //attack animation
        Vector<SpriteFrame*> attackFrames(3);
        for (i = 0; i < 3; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(__String::createWithFormat("hero_attack_00_%02d.png", i)->getCString());
            attackFrames.pushBack(frame);
        }
        Animation *attackAnimation = Animation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
        this->setAttackAction(Sequence::create(Animate::create(attackAnimation), CallFunc::create(this, callfunc_selector(Hero::idle)), NULL));

        this->setCenterToBottom(39.0);
        this->setCenterToSides(29.0);
        this->setHitPoints(100.0);
        this->setDamage(20.0);
        this->setWalkSpeed(80.0);

        bRet = true;
    } while (0);

    return bRet;
}
