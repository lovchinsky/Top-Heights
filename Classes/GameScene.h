#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "Barrier.h"
#include "Cloud.h"
#include "Number.h"
#include <thread>

#include <vector>

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };

	void createSprites();
	void createDynamicSprites();
	void createScore();
	void createBumpAction();
	void createChooseButton();
	void createListeners();

	void addBarrier();
	void addCloud();
	void addScore();

	void hit();
	void choose( cocos2d::Ref *sender );

	void GameOver();

    void update(float dt);

	bool onContactBegin(cocos2d::PhysicsContact &contact);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    Character *character;
	std::vector<Barrier*> barriers;
	std::vector<Cloud*> clouds;

	Number* number;
	
	unsigned int cloud;
    unsigned int barrier;
    unsigned int score;
	unsigned int highScore;
	unsigned int form;

	int gameState;
	int dy;
    
	cocos2d::Vec2 speed;
	cocos2d::Vec2 position;
	cocos2d::PhysicsWorld *sceneWorld;
    cocos2d::Label *scoreLabel;
	cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
	cocos2d::Sprite *backgroundSprite;
	cocos2d::Sprite *instructionSprite;
	cocos2d::Sprite *getreadySprite;
	cocos2d::Sprite *bottomSprite;
	cocos2d::Sprite *forestSprite;
	cocos2d::Action *bumpAction;
	cocos2d::Menu *chooseMenu;
};

#endif
