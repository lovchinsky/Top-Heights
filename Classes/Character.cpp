#include "Character.h"
#include "Definitions.h"

USING_NS_CC;

Character::Character( cocos2d::Layer *layer )
{
	characterSprite = Sprite::create();
	characterSprite->retain();

	form = 0;

	for(int i=0; i<CHARACTER_FORMS; i++)
	{
		standAction.push_back(getAction(i,0,3,1.4f));
		rightWingAction.push_back(getAction(i,3,6,0.1f));
		leftWingAction.push_back(getAction(i,6,9,0.1f));

		standAction.at(i)->retain();
		rightWingAction.at(i)->retain();
		leftWingAction.at(i)->retain();
	}

	auto characterBody = PhysicsBody::createCircle(CHARACTER_SIZE/2-1);
	characterBody->setCollisionBitmask(BIRD_COLLISION_BITMASK);
	characterBody->setContactTestBitmask(true);

	characterSprite->setPhysicsBody(characterBody);

	layer->addChild(characterSprite, 10000);
}
Action* Character::getAction(int id, int begin, int end, float dt)
{
	std::string filemame;

	switch (id)
	{
	case 0:
		filemame = "CharacterRed.png";
		break;
	case 1:
		filemame = "CharacterYellow.png";
		break;
	case 2:
		filemame = "CharacterBlue.png";
		break;
	case 3:
		filemame = "CharacterGhost.png";
		break;
	case 4:
		filemame = "CharacterDark.png";
		break;
	default:
		break;
	}

	Vector<SpriteFrame*> animationFrames;

	for(int i = begin; i < end; i++)
	{
		auto frame = SpriteFrame::create(filemame,Rect((CHARACTER_SIZE+2)*i + 1,1,CHARACTER_SIZE,CHARACTER_SIZE));
		frame->getTexture()->setAliasTexParameters();
		animationFrames.pushBack(frame);

		if(i+1 == end)
		{
			frame = SpriteFrame::create(filemame,Rect((CHARACTER_SIZE+2)*(i-1) + 1,1,CHARACTER_SIZE,CHARACTER_SIZE));
			frame->getTexture()->setAliasTexParameters();
			animationFrames.pushBack(frame);
		}
	}

	auto animation = Animation::createWithSpriteFrames(animationFrames, dt);
	auto animate = Animate::create(animation);
	auto action = RepeatForever::create(animate);

	return action;
}
void Character::setPosition(Vect position)
{
	characterSprite->setPosition( position );
}
Vec2 Character::getPosition()
{
	return characterSprite->getPosition();
}
Size Character::getSize()
{
	return Size(CHARACTER_SIZE,CHARACTER_SIZE);
}
int Character::getForm()
{
	return form;
}
void Character::setForm(int max)
{
	form++;

	if(form > max)
	{
		form = 0;
	}

	characterSprite->stopAllActions();
	characterSprite->runAction(standAction.at(form));
}
void Character::stand()
{
	characterSprite->stopAllActions();
	characterSprite->runAction(standAction.at(form));
}
void Character::wingLeft()
{
	characterSprite->getPhysicsBody()->setPositionOffset(Vec2(-3,0));
	characterSprite->stopAllActions();
	characterSprite->runAction(leftWingAction.at(form));
}
void Character::wingRight()
{
	characterSprite->getPhysicsBody()->setPositionOffset(Vec2(3,0));
	characterSprite->stopAllActions();
	characterSprite->runAction(rightWingAction.at(form));
}



