#ifndef CHARACTER_H
#define CHARACTER_H

#include "cocos2d.h"
#include <vector>

class Character
{
public:
    Character( cocos2d::Layer *layer );
    
    void setPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 getPosition();
	cocos2d::Size getSize();
	int getForm();

	void setForm(int max);
	void stand();
	void wingLeft();
    void wingRight();

private:
	int form;
	cocos2d::Sprite *characterSprite;
	std::vector<cocos2d::Action*> standAction;
	std::vector<cocos2d::Action*> leftWingAction;
	std::vector<cocos2d::Action*> rightWingAction;
	cocos2d::Action *getAction(int id, int begin,int end, float dt);
};

#endif