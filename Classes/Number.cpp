#include "Number.h"
#include "Definitions.h"

USING_NS_CC;



Number::Number(Vec2 pos, NumberSize nSize, cocos2d::Layer *layer )
{
	initSprites(layer);
	setNumberSize(nSize, pos);
}

void Number::setNumber(int n)
{
	number = n;
	std::vector<int> digits = convertIntToVector(n);
	for(int i = 0; i < SCORE_DIGITS_AMOUNT; ++i)
	{
		if(i < digits.size())
		{
			numberSprites.at(i)->setSpriteFrame(numberFrames.at(digits.at(i)));
		}
	}
}

std::vector<int> Number::convertIntToVector(int n)
{
	std::vector<int> digits;
	if (n == 0)
	{
		digits.push_back(0);
		return digits;
	}
	while(n > 0)
	{
		digits.push_back(n % 10);
		n /= 10;
	}
	return digits;
}

void Number::incNumber()
{
	setNumber(number + 1);
}

void Number::setNumberSize(NumberSize nSize, Vec2 pos)
{
	switch(nSize)
	{
	case SMALL_NUMBERS:
		setNumberParameters("SmallNumbers.png", SMALL_NUMBERS_WIDTH, SMALL_NUMBERS_HEIGHT, SMALL_NUMBERS_SPACING, pos);
		break;
	case BIG_NUMBERS:
		setNumberParameters("BigNumbers.png", BIG_NUMBERS_WIDTH, BIG_NUMBERS_HEIGHT, BIG_NUMBERS_SPACING, pos);
		break;
	}
}

void Number::setNumberParameters(std::string atlasFileName, int numberWidth, int numberHeight, int numberSpacing, Vec2 pos)
{
	numberFrames.clear();
	for(int i = 0; i < 10; i++)
	{
		auto frame = SpriteFrame::create(atlasFileName,Rect((numberWidth+2)*i + 1,1,numberWidth, numberHeight));
		frame->getTexture()->setAliasTexParameters();
		numberFrames.pushBack(frame);
	}
	for (int i = 0; i < SCORE_DIGITS_AMOUNT; ++i)
	{
		numberSprites.at(i)->setPosition(pos.x - i * (numberWidth + numberSpacing), pos.y);
	}
}

void Number::initSprites(Layer* layer)
{
	for (int i = 0; i < SCORE_DIGITS_AMOUNT; ++i)
	{
		numberSprites.pushBack(Sprite::create());
		layer->addChild(numberSprites.at(i),10000);
	}
}