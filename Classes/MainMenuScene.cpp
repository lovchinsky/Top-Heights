#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

#include "NativeUtils.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
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

    auto playItem = MenuItemImage::create( "PlayButton.png", "PlayButtonClicked.png", CC_CALLBACK_1( MainMenuScene::GoToGameScene, this ));
    playItem->setPosition(Point(visibleSize.width / 2 + origin.x - BUTTON_SHIFT, visibleSize.height / 2 + origin.y - playItem->getContentSize().height / 2 ));

	auto ratingItem = MenuItemImage::create( "RatingButton.png", "RatingButtonClicked.png", CC_CALLBACK_1( MainMenuScene::GoToRating, this ));
    ratingItem->setPosition(Point(visibleSize.width / 2 + origin.x + BUTTON_SHIFT, visibleSize.height / 2 + origin.y - ratingItem->getContentSize().height / 2 ));

	auto rateItem = MenuItemImage::create( "RateButton.png", "RateButtonClicked.png", CC_CALLBACK_1( MainMenuScene::GoToRate, this ));
	rateItem->setPosition(Point(visibleSize.width / 2 + origin.x, rateItem->getContentSize().height / 2 + playItem->getPosition().y + playItem->getContentSize().height / 2 + BUTTON_SHIFT_RATE));

	auto moreItem = MenuItemImage::create( "MoreButton.png", "MoreButtonClicked.png", CC_CALLBACK_1( MainMenuScene::GoToMore, this ));
	moreItem->setPosition(Point(visibleSize.width / 2 + origin.x,bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2 - moreItem->getContentSize().height / 2 - 54));

	auto titleSprite = Sprite::create( "Title.png" );
	titleSprite->getTexture()->setAliasTexParameters();
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x,(visibleSize.height + origin.y - (rateItem->getPosition().y + rateItem->getContentSize().height / 2)) / 2 + rateItem->getPosition().y + rateItem->getContentSize().height / 2));
    
    auto menu = Menu::create(playItem,ratingItem,rateItem,moreItem, NULL);
    menu->setPosition(Point::ZERO);

	this->setKeypadEnabled( true );
    
	this->addChild(backgroundSprite);
	this->addChild(bottomSprite);
	this->addChild(forestSprite);
	this->addChild(titleSprite);
    this->addChild(menu);
    
    return true;
}
void MainMenuScene::GoToGameScene( cocos2d::Ref *sender )
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "swooshing.ogg" );

    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}
void MainMenuScene::GoToRate( cocos2d::Ref *sender )
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );

	NativeUtils::goRate();
}
void MainMenuScene::GoToMore( cocos2d::Ref *sender )
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );

	NativeUtils::goMore();
}
void MainMenuScene::GoToRating( cocos2d::Ref *sender )
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );

	if(!NativeUtils::isSignedIn())
	{
		NativeUtils::signIn();
	}
	else
	{
		int highScore = UserDefault::getInstance()->getIntegerForKey( "HIGHSCORE", 0 );

		if(highScore > 0)
		{
			NativeUtils::submitScore(LEADERBOARD,highScore);
		}

		NativeUtils::showLeaderboard(LEADERBOARD);
	}
}




