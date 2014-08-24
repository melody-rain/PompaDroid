#include "Hero.h"
#include "SimpleAudioEngine.h"
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
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("hero_idle_%02d.png", i)->getCString());
            idleFrames.pushBack(frame);
        }
        Animation *idleAnimation = Animation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
        this->setIdleAction(CCRepeatForever::create(Animate::create(idleAnimation)));

        //attack animation
        Vector<SpriteFrame*> attackFrames(3);
        for (i = 0; i < 3; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("hero_attack_00_%02d.png", i)->getCString());
            attackFrames.pushBack(frame);
        }
        Animation *attackAnimation = Animation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
        this->setAttackAction(Sequence::create(Animate::create(attackAnimation), CallFunc::create(this, callfunc_selector(Hero::idle)), NULL));

        //walk animation
        Vector<SpriteFrame*> walkFrames(8);
        for (i = 0; i < 8; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("hero_walk_%02d.png", i)->getCString());
            walkFrames.pushBack(frame);
        }
        Animation *walkAnimation = Animation::createWithSpriteFrames(walkFrames, float(1.0 / 12.0));
        this->setWalkAction(CCRepeatForever::create(Animate::create(walkAnimation)));

        //hurt animation
        Vector<SpriteFrame*> hurtFrames(3);
        for (i = 0; i < 3; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("hero_hurt_%02d.png", i)->getCString());
            hurtFrames.pushBack(frame);
        }
        Animation *hurtAnimation = Animation::createWithSpriteFrames(hurtFrames, float(1.0 / 12.0));
        this->setHurtAction(CCSequence::create(Animate::create(hurtAnimation), CallFunc::create(this, callfunc_selector(Hero::idle)), NULL));

        //knocked out animation
        Vector<SpriteFrame*> knockedOutFrames(5);
        for (i = 0; i < 5; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("hero_knockout_%02d.png", i)->getCString());
            knockedOutFrames.pushBack(frame);
        }
        Animation *knockedOutAnimation = Animation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
        this->setKnockedOutAction(Sequence::create(Animate::create(knockedOutAnimation), Blink::create(2.0, 10.0), 
            ActionSprite::createDeadCallbackFunc(), NULL));

        this->setCenterToBottom(39.0);
        this->setCenterToSides(29.0);
        this->setHitPoints(100.0);
        this->setDamage(20.0);
        this->setWalkSpeed(80.0);

        this->setHitbox(this->createBoundingBoxWithOrigin(Vec2(-this->getCenterToSides(), -this->getCenterToBottom()),
            Size(this->getCenterToSides() * 2, this->getCenterToBottom() * 2)));
        this->setAttackBox(this->createBoundingBoxWithOrigin(Vec2(this->getCenterToSides(), -10), Size(20, 20)));

        makeHPBar();

        bRet = true;
    } while (0);

    return bRet;
}

void Hero::knockout()
{
    ActionSprite::knockout();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pd_herodeath.wav");
}