#include "Robot.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

Robot::Robot(void)
{
}

Robot::~Robot(void)
{
}

bool Robot::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("robot_idle_00.png"));
        
        int i;
        //idle animation
        Vector<SpriteFrame*> idleFrames(5);
        for (i = 0; i < 5; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
                __String::createWithFormat("robot_idle_%02d.png", i)->getCString());
            idleFrames.pushBack(frame);
        }
        Animation *idleAnimation = Animation::createWithSpriteFrames(idleFrames, float(1.0 / 12.0));
        this->setIdleAction(RepeatForever::create(Animate::create(idleAnimation)));

        //attack animation
        Vector<SpriteFrame*> attackFrames(5);
        for (i = 0; i < 5; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
                __String::createWithFormat("robot_attack_%02d.png", i)->getCString());
            attackFrames.pushBack(frame);
        }
        Animation *attackAnimation = Animation::createWithSpriteFrames(attackFrames, float(1.0 / 24.0));
        this->setAttackAction(Sequence::create(Animate::create(attackAnimation), CallFunc::create(this, callfunc_selector(Robot::idle)), NULL));

        //walk animation
        Vector<SpriteFrame*> walkFrames(6);
        for (i = 0; i < 6; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(
                String::createWithFormat("robot_walk_%02d.png", i)->getCString());
            walkFrames.pushBack(frame);
        }
        Animation *walkAnimation = Animation::createWithSpriteFrames(walkFrames, float(1.0 / 12.0));
        this->setWalkAction(RepeatForever::create(Animate::create(walkAnimation)));

        //hurt animation
        Vector<SpriteFrame*> hurtFrames(3);
        for (i = 0; i < 3; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("robot_hurt_%02d.png", i)->getCString());
            hurtFrames.pushBack(frame);
        }
        Animation *hurtAnimation = Animation::createWithSpriteFrames(hurtFrames, float(1.0 / 12.0));
        this->setHurtAction(Sequence::create(Animate::create(hurtAnimation), CCCallFunc::create(this, callfunc_selector(Robot::idle)), NULL));

        //knocked out animation
        Vector<SpriteFrame*> knockedOutFrames(5);
        for (i = 0; i < 5; i++)
        {
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("robot_knockout_%02d.png", i)->getCString());
            knockedOutFrames.pushBack(frame);
        }
        Animation *knockedOutAnimation = Animation::createWithSpriteFrames(knockedOutFrames, float(1.0 / 12.0));
        this->setKnockedOutAction(Sequence::create(Animate::create(knockedOutAnimation), Blink::create(2.0, 10.0), 
            ActionSprite::createDeadCallbackFunc(), NULL));

        this->setWalkSpeed(80.0);
        this->setCenterToBottom(39.0);
        this->setCenterToSides(29.0);
        this->setHitPoints(100.0);
        this->setDamage(10.0);

        this->setHitbox(this->createBoundingBoxWithOrigin(Vec2(-this->getCenterToSides(), -this->getCenterToBottom()),
            Size(this->getCenterToSides() * 2, this->getCenterToBottom() * 2)));
        this->setAttackBox(this->createBoundingBoxWithOrigin(Vec2(this->getCenterToSides(), -5), Size(25, 20)));

        _nextDecisionTime = 0;
        makeHPBar();
        bRet = true;
    } while (0);

    return bRet;
}

void Robot::knockout()
{
    ActionSprite::knockout();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pd_botdeath.wav");
}