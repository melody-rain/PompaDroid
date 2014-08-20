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
		_velocity = CCPointZero;
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
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(__String::createWithFormat("pd_hit%d.wav", randomSound)->getCString());
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
		_velocity = ccp(direction.x * _walkSpeed, direction.y * _walkSpeed);
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
		_desiredPosition = ccpAdd(this->getPosition(), ccpMult(_velocity, dt));
	}
}

BoundingBox ActionSprite::createBoundingBoxWithOrigin(Point origin, Size size)
{
	BoundingBox boundingBox;
	boundingBox.original.origin = origin;
	boundingBox.original.size = size;
	boundingBox.actual.origin = ccpAdd(this->getPosition(), ccp(boundingBox.original.origin.x, boundingBox.original.origin.y));
	boundingBox.actual.size = size;
	return boundingBox;
}

void ActionSprite::transformBoxes()
{
	_hitBox.actual.origin = ccpAdd(this->getPosition(), ccp(_hitBox.original.origin.x, _hitBox.original.origin.y));
	_attackBox.actual.origin = ccpAdd(this->getPosition(), ccp(_attackBox.original.origin.x + 
		(this->getScaleX() == -1 ? (- _attackBox.original.size.width - _hitBox.original.size.width) : 0),
		_attackBox.original.origin.y));
}

void ActionSprite::setPosition(Point position)
{
	CCSprite::setPosition(position);
	this->transformBoxes();
}
