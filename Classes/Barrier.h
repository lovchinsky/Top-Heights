#ifndef BARRIER_H
#define BARRIER_H

#include "cocos2d.h"

class Barrier
{
public:
    Barrier( cocos2d::Layer *layer );
    
    void setPositionY(float y);
	float getPositionY();
	void runAction(cocos2d::Action* action);

	void middleDownBarrierHide();
	bool isVisible();

private:
	cocos2d::Sprite *leftBarrierSprite;
	cocos2d::Sprite *rightBarrierSprite;
	cocos2d::Sprite *middleUpBarrierSprite;
	cocos2d::Sprite *middleDownBarrierSprite;
};

#endif