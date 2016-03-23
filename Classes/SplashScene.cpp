#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"

#include "NativeUtils.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "button.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "die.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "hit.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "point.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "swooshing.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "wing.ogg" );
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->scheduleOnce( schedule_selector( SplashScene::GoToMainMenuScene ), DISPLAY_TIME_SPLASH_SCENE );

    auto splashSprite = Sprite::create( "Splash.png" );
	splashSprite->getTexture()->setAliasTexParameters();
    splashSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    
    this->addChild( splashSprite );
    
    return true;
}

void SplashScene::GoToMainMenuScene( float dt )
{
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}



