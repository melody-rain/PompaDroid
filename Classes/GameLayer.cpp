#include "GameLayer.h"
#include "Robot.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

GameLayer::GameLayer(void)
{
    _tileMap = NULL;
    _hero = NULL;
	_robots = NULL;
}

GameLayer::~GameLayer(void)
{
	this->unscheduleUpdate();
}

bool GameLayer::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!Layer::init());

		// Load audio
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("latin_industries.aifc");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("latin_industries.aifc");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pd_hit0.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pd_hit1.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pd_herodeath.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pd_botdeath.wav");

		this->setTouchEnabled(true);

        SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pd_sprites.plist");
        _actors = SpriteBatchNode::create("pd_sprites.pvr.ccz");
        _actors->getTexture()->setAliasTexParameters();
        this->addChild(_actors, -5);

        this->initHero();
        this->initTileMap();
		this->initRobots();
		
		this->scheduleUpdate();

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

	if (_hero->getActionState() == kActionStateAttack)
	{
		CCObject *pObject = NULL;
		CCARRAY_FOREACH(_robots, pObject)
		{
			Robot *robot = (Robot*)pObject;
			if (robot->getActionState() != kActionStateKnockedOut)
			{
				if (fabsf(_hero->getPosition().y - robot->getPosition().y) < 10)
				{
					if (_hero->getAttackBox().actual.intersectsRect(robot->getHitbox().actual))
					{
						robot->hurtWithDamage(_hero->getDamage());
					}
				}
			}
		}		
	}
}

void GameLayer::didChangeDirectionTo(SimpleDPad *simpleDPad, Point direction)
{
	_hero->walkWithDirection(direction);
}

void GameLayer::isHoldingDirection(SimpleDPad *simpleDPad, Point direction)
{
	_hero->walkWithDirection(direction);
}

void GameLayer::simpleDPadTouchEnded(SimpleDPad *simpleDPad)
{
	if (_hero->getActionState() == kActionStateWalk)
	{
		_hero->idle();
	}
}

void GameLayer::update(float dt)
{
	_hero->update(dt);
	this->updateRobots(dt);
	this->updatePositions();
	this->reorderActors();
	this->setViewpointCenter(_hero->getPosition());
}

void GameLayer::updatePositions()
{
	float posX = MIN(_tileMap->getMapSize().width * _tileMap->getTileSize().width - _hero->getCenterToSides(),
		MAX(_hero->getCenterToSides(), _hero->getDesiredPosition().x));
	float posY = MIN(3 * _tileMap->getTileSize().height + _hero->getCenterToBottom(),
		MAX(_hero->getCenterToBottom(), _hero->getDesiredPosition().y));
	_hero->setPosition(ccp(posX, posY));

	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_robots, pObject)
	{
		Robot *robot = (Robot*)pObject;
		posX = MIN(_tileMap->getMapSize().width * _tileMap->getTileSize().width - robot->getCenterToSides(),
			MAX(robot->getCenterToSides(), robot->getDesiredPosition().x));
		posY = MIN(3 * _tileMap->getTileSize().height + robot->getCenterToBottom(),
			MAX(robot->getCenterToBottom(), robot->getDesiredPosition().y));
		robot->setPosition(ccp(posX, posY));
	}
}

void GameLayer::setViewpointCenter(CCPoint position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
}

void GameLayer::initRobots()
{
	int robotCount = 50;
	this->setRobots(CCArray::createWithCapacity(robotCount));

	for (int i = 0; i < robotCount; i++)
	{
		Robot *robot = Robot::create();
		_actors->addChild(robot);
		_robots->addObject(robot);

		int minX = SCREEN.width + robot->getCenterToSides();
		int maxX = _tileMap->getMapSize().width * _tileMap->getTileSize().width - robot->getCenterToSides();
		int minY = robot->getCenterToBottom();
		int maxY = 3 * _tileMap->getTileSize().height + robot->getCenterToBottom();
		robot->setScaleX(-1);
		robot->setPosition(ccp(random_range(minX, maxX), random_range(minY, maxY)));
		robot->setDesiredPosition(robot->getPosition());
		robot->idle();
	}
}

void GameLayer::reorderActors()
{
	CCObject *pObject = NULL;
    for (const auto &node : _actors->getChildren())
    {
        ActionSprite *sprite = (ActionSprite*)node;
        _actors->reorderChild(sprite, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - sprite->getPosition().y);
    }
}

void GameLayer::updateRobots(float dt)
{
	int alive = 0;
	float distanceSQ;
	int randomChoice = 0;
	CCObject *pObject = NULL;
	CCARRAY_FOREACH(_robots, pObject)
	{
		Robot *robot = (Robot*)pObject;
		robot->update(dt);
		if (robot->getActionState() != kActionStateKnockedOut)
		{
			//1
			alive++;
			
			//2
			if (CURTIME > robot->getNextDecisionTime())
			{
				distanceSQ = ccpDistanceSQ(robot->getPosition(), _hero->getPosition());

				//3
				if (distanceSQ <= 50 * 50)
				{
					robot->setNextDecisionTime(CURTIME + frandom_range(0.1, 0.5) * 1000);
					randomChoice = random_range(0, 1);

					if (randomChoice == 0)
					{
						if (_hero->getPosition().x > robot->getPosition().x)
						{
							robot->setScaleX(1.0);
						} 
						else
						{
							robot->setScaleX(-1.0);
						}

						//4
						robot->setNextDecisionTime(robot->getNextDecisionTime() + frandom_range(0.1, 0.5) * 2000);
						robot->attack();						
						if (robot->getActionState() == kActionStateAttack)
						{
							if (fabsf(_hero->getPosition().y - robot->getPosition().y) < 10)
							{
								if (_hero->getHitbox().actual.intersectsRect(robot->getAttackBox().actual))
								{
									_hero->hurtWithDamage(robot->getDamage());

									//end game checker here
									if (_hero->getActionState() == kActionStateKnockedOut && _hud->getChildByTag(5) == NULL)
									{
										this->endGame();
									}
								}
							}
						}
					}
					else
					{
						robot->idle();
					}
				}
				else if (distanceSQ <= SCREEN.width * SCREEN.width)
				{
					//5
					robot->setNextDecisionTime(CURTIME + frandom_range(0.5, 1.0) * 1000);
					randomChoice = random_range(0, 2);
					if (randomChoice == 0)
					{
						CCPoint moveDirection = ccpNormalize(ccpSub(_hero->getPosition(), robot->getPosition()));
						robot->walkWithDirection(moveDirection);
					} 
					else
					{
						robot->idle();
					}
				}
			}
		}
	}

	//end game checker here
	if (alive == 0 && _hud->getChildByTag(5) == NULL)
	{
		this->endGame();
	}
}

void GameLayer::endGame()
{
	CCLabelTTF *restartLabel = CCLabelTTF::create("RESTART", "Arial", 30);
	CCMenuItemLabel *restartItem = CCMenuItemLabel::create(restartLabel, this, menu_selector(GameLayer::restartGame));
	CCMenu *menu = CCMenu::create(restartItem, NULL);
	menu->setPosition(CENTER);
	menu->setTag(5);
	_hud->addChild(menu, 5);
}

void GameLayer::restartGame(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameScene::create());
}
