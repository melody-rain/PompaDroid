#include "ActionSprite.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

ActionSprite::ActionSprite(void)
{
    _idleAction = NULL;
    _attackAction = NULL;
    _walkAction = NULL;
    _hurtAction = NULL;
    _knockedOutAction = NULL;
}

ActionSprite::~ActionSprite(void)
{
}

void ActionSprite::idle()
{
    if (_actionState != kActionStateIdle)
    {
        this->stopAllActions();
        this->runAction(_idleAction);
        _actionState = kActionStateIdle;
        _velocity = Vec2::ZERO;
    }
}

void ActionSprite::attack()
{
    if (_actionState == kActionStateIdle || _actionState == kActionStateAttack || _actionState == kActionStateWalk)
    {
        this->stopAllActions();
        this->runAction(_attackAction);
        _actionState = kActionStateAttack;
    }
}

void ActionSprite::hurtWithDamage(float damage)
{
    if (_actionState != kActionStateKnockedOut)
    {
        int randomSound = random_range(0, 1);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("pd_hit%d.wav", randomSound)->getCString());
        this->stopAllActions();
        this->runAction(_hurtAction);
        _actionState = kActionStateHurt;
        _hitPoints -= damage;

        if (_hitPoints <= 0)
        {
            this->knockout();
        }
    }
}

void ActionSprite::knockout()
{
    this->stopAllActions();
    this->runAction(_knockedOutAction);
    _hitPoints = 0;
    _actionState = kActionStateKnockedOut;
}

void ActionSprite::walkWithDirection(Point direction)
{
    if (_actionState == kActionStateIdle)
    {
        this->stopAllActions();
        this->runAction(_walkAction);
        _actionState = kActionStateWalk;
    }
    if (_actionState == kActionStateWalk)
    {
        _velocity = Vec2(direction.x * _walkSpeed, direction.y * _walkSpeed);
        if (_velocity.x >= 0)
        {
            this->setScaleX(1.0);
        } 
        else
        {
            this->setScaleX(-1.0);
        }
    }
}

void ActionSprite::update(float dt)
{
    if (_actionState == kActionStateWalk)
    {
        _desiredPosition = this->getPosition() + ccpMult(_velocity, dt);
    }
}

BoundingBox ActionSprite::createBoundingBoxWithOrigin(Point origin, Size size)
{
    BoundingBox boundingBox;
    boundingBox.original.origin = origin;
    boundingBox.original.size = size;
    boundingBox.actual.origin = this->getPosition() + Vec2(boundingBox.original.origin.x, boundingBox.original.origin.y);
    boundingBox.actual.size = size;
    return boundingBox;
}

void ActionSprite::transformBoxes()
{
    _hitBox.actual.origin = this->getPosition() + Vec2(_hitBox.original.origin.x, _hitBox.original.origin.y);
    _attackBox.actual.origin = this->getPosition() + Vec2(_attackBox.original.origin.x +
        (this->getScaleX() == -1 ? (- _attackBox.original.size.width - _hitBox.original.size.width) : 0),
        _attackBox.original.origin.y);
}

void ActionSprite::setPosition(Point position)
{
    CCSprite::setPosition(position);
    this->transformBoxes();
}

void ActionSprite::updateHP(float hitPoints)
{
    heroHPBar->setPercentage(hitPoints / 100 * 100);
}

void ActionSprite::makeHPBar()
{
    Sprite *heroHPBarSprite = Sprite::create("blood.png");
    heroHPBar = ProgressTimer::create(heroHPBarSprite);
    heroHPBar->setType(ProgressTimer::Type::BAR);
    heroHPBar->setMidpoint(Vec2(0, 0.5));
    heroHPBar->setBarChangeRate(Vec2(1, 0));
    heroHPBar->setPercentage(getHitPoints());
    heroHPBar->setAnchorPoint(Vec2(0, 0.5));
    heroHPBar->setScaleX(0.3);

    Sprite *heroHPBarSpriteBg = Sprite::create("bloodbg.png");
    heroHPBarBg = ProgressTimer::create(heroHPBarSpriteBg);
    heroHPBarBg->setType(ProgressTimer::Type::BAR);
    heroHPBarBg->setMidpoint(Vec2(0, 0));
    heroHPBarBg->setBarChangeRate(Vec2(1, 0));
    heroHPBarBg->setPercentage(getHitPoints());
    heroHPBarBg->setAnchorPoint(Vec2(0, 0.5));
    heroHPBarBg->setScaleX(0.3);
}

cocos2d::CallFunc *ActionSprite::createDeadCallbackFunc()
{
    return CallFunc::create(CC_CALLBACK_0(ActionSprite::onDead, this));
}

void ActionSprite::onDead()
{
    this->onDeadCallback();
}

void ActionSprite::removeSprite()
{
    _actionState = kActionStateDisapper;
}