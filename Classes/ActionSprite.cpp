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

void ActionSprite::updateHP()
{

}

void ActionSprite::makeHPBar()
{

}