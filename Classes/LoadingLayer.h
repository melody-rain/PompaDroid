#pragma once

#include "cocos2d.h"

USING_NS_CC;

class LoadingLayer : public Layer
{
public:
    LoadingLayer()
    {
    }

    virtual ~LoadingLayer()
    {
    }

    CREATE_FUNC(LoadingLayer);

    bool init();
    static Scene *scene();
    void onEnterTransitionDidFinish();
};