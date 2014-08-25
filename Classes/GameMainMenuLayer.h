#pragma once

#include "cocos2d.h"
USING_NS_CC;

class GameMainMenuLayer : public Layer
{
public:
    GameMainMenuLayer()
    {

    }

    virtual ~GameMainMenuLayer()
    {

    }

    CREATE_FUNC(GameMainMenuLayer);
    bool init();
    static Scene *scene();

    void onNewGameCallback(Ref *sender);

    void onEnter();

    CC_SYNTHESIZE(Menu*, mainMenu, MainMenu);

    void onEnterTransitionDidFinish();

private:
    void initBackground();
    void initMenus();
    void enableMenuItem();

};