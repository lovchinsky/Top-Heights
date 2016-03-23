#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene( unsigned int s );

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);
    
private:
    void GoToGameScene( cocos2d::Ref *sender );
	void GoToMainMenuScene( cocos2d::Ref *sender );
	bool setHighScore();

	int highScore;
	int unlocked;
    
};

#endif // __GAME_OVER_SCENE_H__
