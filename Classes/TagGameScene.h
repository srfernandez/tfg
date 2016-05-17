#ifndef __TAGGAME_H__
#define __TAGGAME_H__

#include "cocos2d.h"
#include "Obstacles.h"
#include "Protag.h"
using namespace cocos2d;
class TagGame: public cocos2d::Layer
{
public:
	Sprite* bk1;
	Sprite* bk2;
    static Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    Size visibleSize;
    // touch events
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

	bool isTouchDown;

	float initialTouchPos[2];
	float currentTouchPos[2];

	 void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(TagGame);

private:

    void setPhysicsWorld(cocos2d::PhysicsWorld *world){ sceneWorld=world;}
    void ScrollBG(float dt);
    void spawnFoes(float dt);
    void StopJumping(float dt);

    cocos2d::PhysicsWorld *sceneWorld;

    Obstacles obstacle;
    Protag *protag;


    //Physics Contact Listener
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    bool onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve& solve);
    void onContactPostSolve(PhysicsContact &contact,
    		const PhysicsContactPostSolve &solve);
    void onContactSeperate(PhysicsContact &contact);

};

#endif // __TAGGAME_H__
