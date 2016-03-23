#include "Barrier.h"
#include "Definitions.h"

USING_NS_CC;

Barrier::Barrier( cocos2d::Layer *layer )
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	leftBarrierSprite = Sprite::create("Barrier.png");
	rightBarrierSprite = Sprite::create("Barrier.png");
	middleUpBarrierSprite = Sprite::create("Block.png");
	middleDownBarrierSprite = Sprite::create("Block.png");

	leftBarrierSprite->getTexture()->setAliasTexParameters();
	rightBarrierSprite->getTexture()->setAliasTexParameters();
	middleUpBarrierSprite->getTexture()->setAliasTexParameters();
	middleDownBarrierSprite->getTexture()->setAliasTexParameters();

	auto leftBarrierBody = PhysicsBody::createBox(Size(leftBarrierSprite->getContentSize().width,leftBarrierSprite->getContentSize().height));
	leftBarrierBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	leftBarrierBody->setContactTestBitmask(true);
	leftBarrierBody->setDynamic(false);

	auto rightBarrierBody = PhysicsBody::createBox(Size(rightBarrierSprite->getContentSize().width,rightBarrierSprite->getContentSize().height));
	rightBarrierBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	rightBarrierBody->setContactTestBitmask(true);
	rightBarrierBody->setDynamic(false);

	auto middleUpBarrierBody = PhysicsBody::createBox(Size(middleUpBarrierSprite->getContentSize().width,middleUpBarrierSprite->getContentSize().height));
	middleUpBarrierBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	middleUpBarrierBody->setContactTestBitmask(true);
	middleUpBarrierBody->setDynamic(false);

	auto middleDownBarrierBody = PhysicsBody::createBox(Size(middleDownBarrierSprite->getContentSize().width,middleDownBarrierSprite->getContentSize().height));
	middleDownBarrierBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	middleDownBarrierBody->setContactTestBitmask(true);
	middleDownBarrierBody->setDynamic(false);

	leftBarrierSprite->setPhysicsBody(leftBarrierBody);
	rightBarrierSprite->setPhysicsBody(rightBarrierBody);
	middleUpBarrierSprite->setPhysicsBody(middleUpBarrierBody);
	middleDownBarrierSprite->setPhysicsBody(middleDownBarrierBody);

	int random = CCRANDOM_MINUS1_1() * floor((CCRANDOM_0_1() * (LEFT_RIGHT_BARRIER_SHIFT - LEFT_RIGHT_BARRIER_INDENT) + LEFT_RIGHT_BARRIER_INDENT) / 6) * 3;

	leftBarrierSprite->setPositionX(visibleSize.width / 2 + origin.x + random - (leftBarrierSprite->getContentSize().width + LEFT_RIGHT_BARRIER_DISTANCE) / 2);
	rightBarrierSprite->setPositionX(visibleSize.width / 2 + origin.x + random + (rightBarrierSprite->getContentSize().width + LEFT_RIGHT_BARRIER_DISTANCE) / 2);

	middleUpBarrierSprite->setPositionX(leftBarrierSprite->getPosition().x + leftBarrierSprite->getContentSize().width / 2 + middleUpBarrierSprite->getContentSize().width / 2);
	middleDownBarrierSprite->setPositionX(rightBarrierSprite->getPosition().x - rightBarrierSprite->getContentSize().width / 2 - middleDownBarrierSprite->getContentSize().width / 2);

	random = floor(CCRANDOM_0_1() * (LEFT_RIGHT_BARRIER_DISTANCE - middleUpBarrierSprite->getContentSize().width) / 3) * 3;
	middleUpBarrierSprite->setPositionX(middleUpBarrierSprite->getPositionX() + random);

	random = floor(CCRANDOM_0_1() * (LEFT_RIGHT_BARRIER_DISTANCE - middleDownBarrierSprite->getContentSize().width) / 3) * 3;
	middleDownBarrierSprite->setPositionX(middleDownBarrierSprite->getPositionX() - random);

	layer->addChild(leftBarrierSprite,1000);
	layer->addChild(rightBarrierSprite,1000);
	layer->addChild(middleUpBarrierSprite,1000);
	layer->addChild(middleDownBarrierSprite,1000);
}
void Barrier::setPositionY(float y)
{
	leftBarrierSprite->setPositionY(y);
	rightBarrierSprite->setPositionY(y);
	middleUpBarrierSprite->setPositionY(y + MIDDLE_BARRIER_SHIFT);
	middleDownBarrierSprite->setPositionY(y - MIDDLE_BARRIER_SHIFT);
}
float Barrier::getPositionY()
{
	return leftBarrierSprite->getPositionY();
}
void Barrier::runAction(cocos2d::Action* action)
{
	leftBarrierSprite->runAction(action->clone());
	rightBarrierSprite->runAction(action->clone());
	middleUpBarrierSprite->runAction(action->clone());
	middleDownBarrierSprite->runAction(action->clone());
}
void Barrier::middleDownBarrierHide()
{
	middleDownBarrierSprite->getPhysicsBody()->setEnable(false);
	middleDownBarrierSprite->setVisible(false);
}
bool Barrier::isVisible()
{
	if(middleUpBarrierSprite->getPositionY() + BARRIERS_HEIGHT < 0)
	{
		return false;
	}

	return true;
}