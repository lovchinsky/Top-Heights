#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

#include "NativeUtils.h"

USING_NS_CC;

unsigned int score;

Scene* GameOverScene::createScene( unsigned int tempScore )
{
	score = tempScore;

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create( "Background.png" );
	backgroundSprite->getTexture()->setAliasTexParameters();
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto bottomSprite = Sprite::create( "Bottom.png" );
	bottomSprite->getTexture()->setAliasTexParameters();
	bottomSprite->setPosition(Point(visibleSize.width / 2 + origin.x, bottomSprite->getContentSize().height / 2 + origin.y));

	auto forestSprite = Sprite::create( "Forest.png" );
	forestSprite->getTexture()->setAliasTexParameters();
	forestSprite->setPosition(Point(visibleSize.width / 2 + origin.x, forestSprite->getContentSize().height / 2 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2));

	auto cloudSprite = Sprite::create( "Cloud.png" );
	cloudSprite->getTexture()->setAliasTexParameters();
	cloudSprite->setPosition(Point(visibleSize.width / 2 + origin.x + visibleSize.width / 4, cloudSprite->getContentSize().height * 2 + visibleSize.height / 2 + origin.y));

	auto retryItem = MenuItemImage::create( "PlayButton.png", "PlayButtonClicked.png", CC_CALLBACK_1( GameOverScene::GoToGameScene, this ));
	retryItem->setPosition(Point(visibleSize.width / 2 + origin.x - BUTTON_SHIFT, visibleSize.height / 2 + origin.y - retryItem->getContentSize().height / 2));

	auto homeItem = MenuItemImage::create( "HomeButton.png", "HomeButtonClicked.png", CC_CALLBACK_1( GameOverScene::GoToMainMenuScene, this ));
	homeItem->setPosition(Point(visibleSize.width / 2 + origin.x + BUTTON_SHIFT, visibleSize.height / 2 + origin.y - homeItem->getContentSize().height / 2));

	auto tableSprite = Sprite::create( "Table.png" );
	tableSprite->getTexture()->setAliasTexParameters();
	tableSprite->setPosition(Point(visibleSize.width / 2 + origin.x, retryItem->getPosition().y + retryItem->getContentSize().height / 2 + tableSprite->getContentSize().height / 2 + BUTTON_UP_SHIFT));

	auto overSprite = Sprite::create( "GameOver.png" );
	overSprite->getTexture()->setAliasTexParameters();
	overSprite->setPosition(Point(visibleSize.width / 2 + origin.x, tableSprite->getPosition().y+tableSprite->getContentSize().height / 2 + overSprite->getContentSize().height + BUTTON_UP_SHIFT));

	auto menu = Menu::create(retryItem, homeItem, NULL);
	menu->setPosition(Point::ZERO);

	auto newSprite = Sprite::create( "New.png" );
	newSprite->getTexture()->setAliasTexParameters();
	newSprite->setPosition(Point(tableSprite->getPosition().x + newSprite->getContentSize().width / 2 + 33, tableSprite->getPosition().y - newSprite->getContentSize().height / 2 - 6));
	newSprite->setVisible(false);

	if(setHighScore())
	{
		newSprite->setVisible(true);
	}

	auto scoreNumber = new Number(Vec2(tableSprite->getPosition().x + (tableSprite->getContentSize().width / 2.0) * 0.6724 + SMALL_NUMBERS_WIDTH / 2.0,
		tableSprite->getPosition().y + (tableSprite->getContentSize().height / 2.0) * (1 / 87.0) + SMALL_NUMBERS_HEIGHT / 2.0), SMALL_NUMBERS, this);
	auto highScoreNumber = new Number(Vec2(tableSprite->getPosition().x + (tableSprite->getContentSize().width / 2.0) * 0.6724 + SMALL_NUMBERS_WIDTH / 2.0,
		tableSprite->getPosition().y - ((tableSprite->getContentSize().height / 2.0) * (32 / 87.0) + SMALL_NUMBERS_HEIGHT / 2.0)), SMALL_NUMBERS, this);

	auto unlockedSprite = Sprite::create();
	auto unlockedFrame = SpriteFrame::create("Unlocked.png",Rect((CHARACTER_SIZE+2)*unlocked + 1,1,CHARACTER_SIZE, CHARACTER_SIZE + 3));
	unlockedFrame->getTexture()->setAliasTexParameters();
	unlockedSprite->setSpriteFrame(unlockedFrame);
	unlockedSprite->setPosition(Vec2(tableSprite->getPosition().x - 63 - CHARACTER_SIZE / 2, tableSprite->getPosition().y + 15 - CHARACTER_SIZE / 2));

	scoreNumber->setNumber(score);
	highScoreNumber->setNumber(highScore);

	auto keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameOverScene::onKeyReleased, this);

	this->setKeypadEnabled( true );

	this->addChild(backgroundSprite);
	this->addChild(bottomSprite);
	this->addChild(forestSprite);
	this->addChild(cloudSprite);
	this->addChild(tableSprite);
	this->addChild(newSprite);
	this->addChild(overSprite);
	this->addChild(menu);
	this->addChild(unlockedSprite);

	return true;
}
bool GameOverScene::setHighScore()
{
	auto userDefault = UserDefault::getInstance();

	highScore = userDefault->getIntegerForKey("HIGHSCORE", 0);
	unlocked = 0;

	if (score > highScore)
	{
		if(score >= 50 && highScore < 50)
		{
			unlocked = 4;
		}
		else if(score >= 20 && highScore < 20)
		{
			unlocked = 3;
		}
		else if(score >= 10 && highScore < 10)
		{
			unlocked = 2;
		}
		else if(score >= 5 && highScore < 5)
		{
			unlocked = 1;
		}

		highScore = score;
		userDefault->setIntegerForKey("HIGHSCORE", highScore);
		userDefault->flush();

		if(NativeUtils::isSignedIn())
		{
			NativeUtils::submitScore(LEADERBOARD,highScore);
		}

		return true;
	}

	return false;
}
void GameOverScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) 
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "swooshing.ogg" );

		auto scene = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
	}
}
void GameOverScene::GoToGameScene( cocos2d::Ref *sender )
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "swooshing.ogg" );

	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
}
void GameOverScene::GoToMainMenuScene( cocos2d::Ref *sender )
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "swooshing.ogg" );

	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
}








