#include "GameScene.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics( );
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	scene->getPhysicsWorld()->setGravity( Vect(0.0f, 0.0f) );

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->SetPhysicsWorld( scene->getPhysicsWorld() );

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	gameState = 0;

	createSprites();
	createDynamicSprites();
	createScore();
	createBumpAction();
	createChooseButton();
	createListeners();

	scheduleUpdate();

	return true;
}
void GameScene::createSprites()
{
	backgroundSprite = Sprite::create( "Background.png" );
	backgroundSprite->getTexture()->setAliasTexParameters();
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	bottomSprite = Sprite::create( "Bottom.png" );
	bottomSprite->getTexture()->setAliasTexParameters();
	bottomSprite->setPosition(Point(visibleSize.width / 2 + origin.x, bottomSprite->getContentSize().height / 2 + origin.y));

	forestSprite = Sprite::create( "Forest.png" );
	forestSprite->getTexture()->setAliasTexParameters();
	forestSprite->setPosition(Point(visibleSize.width / 2 + origin.x, forestSprite->getContentSize().height / 2 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2));

	instructionSprite = Sprite::create( "Instruction.png" );
	instructionSprite->getTexture()->setAliasTexParameters();
	instructionSprite->setPosition(Point(visibleSize.width / 2 + origin.x, forestSprite->getContentSize().height * 2 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2));

	getreadySprite = Sprite::create( "GetReady.png" );
	getreadySprite->getTexture()->setAliasTexParameters();
	getreadySprite->setPosition(Point(visibleSize.width / 2 + origin.x,getreadySprite->getContentSize().height + instructionSprite->getContentSize().height + instructionSprite->getPosition().y));

	this->addChild(backgroundSprite);
	this->addChild(bottomSprite);
	this->addChild(forestSprite);
	this->addChild(instructionSprite,100000);
	this->addChild(getreadySprite,100000);
}
void GameScene::createDynamicSprites()
{
	character = new Character(this);
	character->setPosition(Vect(visibleSize.width / 2 + origin.x, character->getSize().height / 2 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2));
	character->stand();

	barrier = 0;
	barriers.push_back(new Barrier(this));
	barriers.at(0)->setPositionY(character->getSize().height * 12 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2);
	barriers.at(0)->middleDownBarrierHide();

	cloud = 0;
	clouds.push_back(new Cloud(this));
	clouds.at(0)->setPosition(Vect(visibleSize.width / 2 + origin.x + visibleSize.width / 4, clouds.at(0)->getSize().height * 2 + visibleSize.height / 2 + origin.y));
}
void GameScene::createScore()
{
	score = 0;
	highScore = UserDefault::getInstance()->getIntegerForKey( "HIGHSCORE", 0 );
	form = UserDefault::getInstance()->getIntegerForKey( "FORM", 0 );

	if(highScore > pow(10,SCORE_DIGITS_AMOUNT)-1)
	{
		highScore = 0;
	}

	if(form > 4)
	{
		form = 0;
	}

	for(int i=0; i<form; i++)
	{
		character->setForm(form);
	}

	number = new Number(Vec2(origin.x + visibleSize.width - BIG_NUMBERS_WIDTH / 2 - SCORE_SHIFT, origin.y + visibleSize.height - BIG_NUMBERS_HEIGHT / 2 - SCORE_SHIFT), BIG_NUMBERS,  this);
	number->setNumber(score);
}
void GameScene::createBumpAction()
{
	auto moveUp = MoveBy::create(0.05f,Vec2(0,12));
	auto moveDown = MoveBy::create(0.1f,Vec2(0,-12));
	auto moveEaseDown = EaseBounceOut::create(moveDown->clone());
	bumpAction = Sequence::create(moveUp,moveEaseDown,NULL);
	bumpAction->retain();
}
void GameScene::createChooseButton()
{
	auto chooseButton = MenuItemImage::create("ChooseButton.png", "ChooseButtonClicked.png", CC_CALLBACK_1( GameScene::choose, this ));
	chooseButton->setPosition(Point(chooseButton->getContentSize().width / 2 + CHOOSE_BUTTON_RIGHT_SHIFT, bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2 - (chooseButton->getContentSize().height / 2 + CHOOSE_BUTTON_DOWN_SHIFT)));

	chooseMenu = Menu::create(chooseButton, NULL);
	chooseMenu->setPosition(Point::ZERO);

	this->addChild(chooseMenu,10000);

	if(highScore < 5)
	{
		chooseMenu->setEnabled(false);
		chooseMenu->setVisible(false);
	}
}
void GameScene::createListeners()
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1( GameScene::onContactBegin, this );
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches( true );
	touchListener->onTouchBegan = CC_CALLBACK_2( GameScene::onTouchBegan, this );
	touchListener->onTouchEnded = CC_CALLBACK_2( GameScene::onTouchEnded, this );
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touchListener, this );

	auto keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	this->setKeypadEnabled( true );
}
void GameScene::addBarrier()
{
	if(barriers.at(barriers.size()-1)->getPositionY() < visibleSize.height)
	{
		barriers.push_back(new Barrier(this));
		barriers.at(barriers.size()-1)->setPositionY(barriers.at(barriers.size()-2)->getPositionY() + character->getSize().height * 12);
	}
}
void GameScene::addCloud()
{
	if(clouds.at(clouds.size()-1)->getPosition().y < visibleSize.height)
	{
		cloud++;
		clouds.push_back(new Cloud(this));
		clouds.at(clouds.size()-1)->setPosition(Vect(pow(-1,cloud) * (visibleSize.width / 4 - CLOUDS_SHIFT) + visibleSize.width / 2 + origin.x,character->getSize().height * 6 + clouds.at(clouds.size()-2)->getPosition().y));
	}
}
void GameScene::addScore()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "point.ogg" );
	number->setNumber(++score);
}
void GameScene::choose( cocos2d::Ref *sender )
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "button.ogg" );

	if(highScore >= 50)
	{
		form = 4;
	}
	else if(highScore >= 20)
	{
		form = 3;
	}
	else if(highScore >= 10)
	{
		form = 2;
	}
	else if(highScore >= 5)
	{
		form = 1;
	}

	character->setForm(form);
}
void GameScene::hit()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "hit.ogg" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "die.ogg" );

	for(int i=0; i<barriers.size(); i++)
	{
		barriers.at(i)->runAction(bumpAction);
	}

	gameState = 2;

	speed.x = 0;
	speed.y = 0;
}
void GameScene::GameOver()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "swooshing.ogg" );

	gameState = 3;

	auto scene = GameOverScene::createScene(score);
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameScene::update( float dt )
{
	addBarrier();
	addCloud();

	position = character->getPosition();

	if(gameState == 1)
	{
		dy = position.y + speed.y - visibleSize.height/2;

		if(dy > 0 && speed.y > 0)
		{
			bottomSprite->setPosition(bottomSprite->getPositionX(),bottomSprite->getPositionY()-dy);
			forestSprite->setPosition(forestSprite->getPositionX(),forestSprite->getPositionY()-dy);

			for(int i=0; i<barriers.size(); i++)
			{
				if(barriers.at(i)->isVisible())
				{
					barriers.at(i)->setPositionY(barriers.at(i)->getPositionY()-dy);
				}
				else
				{
					delete barriers.at(i);
					barriers.erase(barriers.begin()+i);
					barrier++;
				}
			}

			for(int i=0; i<clouds.size(); i++)
			{
				if(clouds.at(i)->isVisible())
				{
					clouds.at(i)->setPosition(Vect(clouds.at(i)->getPosition().x,clouds.at(i)->getPosition().y-dy));
				}
				else
				{
					delete clouds.at(i);
					clouds.erase(clouds.begin()+i);
				}
			}

			if(position.x + speed.x > visibleSize.width)
			{
				character->setPosition(Vect(visibleSize.width,visibleSize.height/2));
			}
			else if(position.x + speed.x < 0)
			{
				character->setPosition(Vect(0,visibleSize.height/2));
			}
			else
			{
				character->setPosition(Vect(position.x+speed.x,visibleSize.height/2));
			}
		}
		else
		{
			if(position.y + speed.y < character->getSize().height / 2 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2)
			{
				speed.x = 0;
				speed.y = 0;
				character->setPosition(Vect(position.x,character->getSize().height / 2 + bottomSprite->getPositionY() + bottomSprite->getContentSize().height / 2));
			}
			else
			{
				if(position.x + speed.x > visibleSize.width)
				{
					character->setPosition(Vect(visibleSize.width,position.y+speed.y));
				}
				else if(position.x + speed.x < 0)
				{
					character->setPosition(Vect(0,position.y+speed.y));
				}
				else
				{
					character->setPosition(Vect(position.x+speed.x,position.y+speed.y));
				}
			}
		}

		if(position.y - character->getSize().height / 2 < 0)
		{
			GameOver();
		}

		if(position.y > barriers.at(score-barrier)->getPositionY())
		{
			addScore();
		}

		speed.y = speed.y - 1;
		
	}
	else if(gameState == 2)
	{
		if(position.y - character->getSize().height / 2 < 0)
		{
			GameOver();
		}

		character->setPosition(Vect(position.x,position.y+speed.y));
		speed.y = speed.y - 1;
	}
}
bool GameScene::onContactBegin( cocos2d::PhysicsContact &contact )
{
	if(gameState == 1)
	{
		PhysicsBody *a = contact.getShapeA()->getBody();
		PhysicsBody *b = contact.getShapeB()->getBody();

		if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask()) || (BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
		{
			hit();
		}
	}

	return true;
}

bool GameScene::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
	auto touchLocation = touch->getLocation();
	auto nodePosition = convertToNodeSpace( touchLocation );

	if(gameState == 0)
	{
		UserDefault::getInstance()->setIntegerForKey("FORM", character->getForm());
		UserDefault::getInstance()->flush();

		this->removeChild(instructionSprite);
		this->removeChild(getreadySprite);
		this->removeChild(chooseMenu);

		gameState = 1;
	}
	if(gameState == 1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("wing.ogg");

		if(nodePosition.x < visibleSize.width / 2 + origin.x)
		{
			if(speed.x >= 0)
			{
				character->wingLeft();
			}

			speed.x = SPEED_LEFT;
			speed.y = SPEED_UP;
		}
		else
		{
			if(speed.x <= 0)
			{
				character->wingRight();
			}

			speed.x = SPEED_RIGHT;
			speed.y = SPEED_UP;
		}
	}

	return true;
}
void GameScene::onTouchEnded( cocos2d::Touch *touch, cocos2d::Event *event )
{

}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if(gameState == 0)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "swooshing.ogg" );

			auto scene = MainMenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
		}
	}
}








