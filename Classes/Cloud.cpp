#include "Cloud.h"
#include "Definitions.h"

USING_NS_CC;

Cloud::Cloud( cocos2d::Layer *layer )
{
	cloudSprite = Sprite::create( "Cloud.png" );
	layer->addChild(cloudSprite);
}
void Cloud::setPosition(Vect position)
{
	cloudSprite->setPosition( position );
}
Vec2 Cloud::getPosition()
{
	return cloudSprite->getPosition();
}
Size Cloud::getSize()
{
	return cloudSprite->getContentSize();
}
bool Cloud::isVisible()
{
	if(cloudSprite->getPositionY() + CLOUDS_HEIGHT < 0)
	{
		return false;
	}

	return true;
}