#ifndef CLOUD_H
#define CLOUD_H

#include "cocos2d.h"

class Cloud
{
public:
    Cloud( cocos2d::Layer *layer );
    
    void setPosition(cocos2d::Vect position);
	cocos2d::Vec2 getPosition();
	cocos2d::Size getSize();

	bool isVisible();

private:
	cocos2d::Sprite *cloudSprite;
};

#endif