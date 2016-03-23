#ifndef NUMBER_H
#define NUMBER_H

#include "cocos2d.h"
#include <vector>
#include <string>

enum NumberSize {SMALL_NUMBERS, BIG_NUMBERS};

class Number
{
public:
    Number(cocos2d::Vec2 pos, NumberSize nSize, cocos2d::Layer *layer );

	void setNumber(int number);
	void incNumber();
	void setNumberSize(NumberSize nSize, cocos2d::Vec2 pos);

private:
	cocos2d::Vector<cocos2d::SpriteFrame*> numberFrames;
	cocos2d::Vector<cocos2d::Sprite*> numberSprites;
	int number;

	std::vector<int> convertIntToVector(int number);
	void initSprites(cocos2d::Layer* layer);
	void setNumberParameters(std::string atlasFileName, int numberWidth, int numberHeight, int numberSpacing, cocos2d::Vec2 pos);
};

#endif