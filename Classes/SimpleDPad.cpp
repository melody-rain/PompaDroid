#include "SimpleDPad.h"
using namespace cocos2d;

SimpleDPad::SimpleDPad(void)
{
    _delegate = NULL;
}

SimpleDPad::~SimpleDPad(void)
{
}

SimpleDPad* SimpleDPad::dPadWithFile(__String *fileName, float radius)
{
    SimpleDPad *pRet = new SimpleDPad();
    if (pRet && pRet->initWithFile(fileName, radius))
    {
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool SimpleDPad::initWithFile(__String *filename, float radius)
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!Sprite::initWithFile(filename->getCString()));
        
        _radius = radius;
        _direction = CCPointZero;
        _isHeld = false;
        this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void SimpleDPad::onEnterTransitionDidFinish()
{
    auto touchByOneListener = EventListenerTouchOneByOne::create();
    touchByOneListener->onTouchBegan = CC_CALLBACK_2(SimpleDPad::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchByOneListener, this);
}

void SimpleDPad::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}

void SimpleDPad::update(float dt)
{
    if (_isHeld)
    {
        _delegate->isHoldingDirection(this, _direction);
    }
}

bool SimpleDPad::onTouchBegan(Touch *pTouch, Event *unused_event)
{
    Point location = pTouch->getLocation();

    float distanceSQ = ccpDistanceSQ(location, this->getPosition());
    if (distanceSQ <= _radius * _radius)
    {
        this->updateDirectionForTouchLocation(location);
        _isHeld = true;
        return true;
    }
    return false;
}

void SimpleDPad::onTouchMoved(Touch* pTouch, Event *unused_event)
{
    CCPoint location = pTouch->getLocation();
    this->updateDirectionForTouchLocation(location);
}

void SimpleDPad::onTouchEnded(Touch* pTouch, Event *unused_event)
{
    _direction = CCPointZero;
    _isHeld = false;
    _delegate->simpleDPadTouchEnded(this);
}

void SimpleDPad::updateDirectionForTouchLocation(Point location)
{
    float radians = ccpToAngle(ccpSub(location, this->getPosition()));
    float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);

    if (degrees <= 22.5 && degrees >= -22.5) 
    {
        //right
        _direction = ccp(1.0, 0.0);
    }
    else if (degrees > 22.5 && degrees < 67.5)
    {
        //bottomright
        _direction = ccp(1.0, -1.0);
    }
    else if (degrees >= 67.5 && degrees <= 112.5)
    {
        //bottom
        _direction = ccp(0.0, -1.0);
    }
    else if (degrees > 112.5 && degrees < 157.5)
    {
        //bottomleft
        _direction = ccp(-1.0, -1.0);
    }
    else if (degrees >= 157.5 || degrees <= -157.5)
    {
        //left
        _direction = ccp(-1.0, 0.0);
    }
    else if (degrees < -22.5 && degrees > -67.5)
    {
        //topright
        _direction = ccp(1.0, 1.0);
    }
    else if (degrees <= -67.5 && degrees >= -112.5)
    {
        //top
        _direction = ccp(0.0, 1.0);
    }
    else if (degrees < -112.5 && degrees > -157.5)
    {
        //topleft
        _direction = ccp(-1.0, 1.0);
    }
    _delegate->didChangeDirectionTo(this, _direction);
}
