#pragma once
#include "actionsprite.h"

class Robot : public ActionSprite
{
public:
	Robot(void);
	~Robot(void);

	CREATE_FUNC(Robot);
	bool init();
	void knockout();

	CC_SYNTHESIZE(float, _nextDecisionTime, NextDecisionTime);
};
