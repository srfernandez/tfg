#ifndef __TAGGAME_H__
#define __TAGGAME_H__

#include "cocos2d.h"

#include "World1.h"
#include "World2.h"
#include "World3.h"
#include "SimpleAudioEngine.h"
#include "SimpleAudioEngine.h"
#include "json/document.h"
#include "json/filestream.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
using namespace cocos2d;
class TagGame: public cocos2d::Layer
{
public:
    static Scene* createScene();
    static Scene* createSceneW2();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    Size visibleSize;
    Vec2 origin;

   CocosDenshion::SimpleAudioEngine* audio;
   void update(float dt); // default schedule method

    // implement the "static create()" method manually
    CREATE_FUNC(TagGame);

private:
    World1 *world1= NULL;
    World2 *world2= NULL;
    World3 *world3= NULL;
    rapidjson::Document saveFile;
    Sprite* _bg1;
    Sprite* _bg2;
    Sprite* _foreground;
    Sprite* _foreground2;
    Animation* _animation;
    SpriteFrame* _jumpFrame;
    Sprite* skull;
    Vector<SpriteFrame *> frames;
    Vector<SpriteFrame *> jumpFrames;
    Vector <SpriteFrame *> dashFrames;
    Animation *animationArray[3];
    Label* _health;
    Label* _points;
    short _currentHealth;
    short _currentPoints;
    short _world;
    cocos2d::PhysicsWorld *sceneWorld;

    //Protag *protag;
    Sprite* protag;
    bool isTouchingGround;


    float initialTouchPos[2];
    float currentTouchPos[2];
    bool isTouchDown=false;
    short _toFinish;
    short _difficulty;
    short auxFinish, auxCombo;
    bool _isWon=true;
    bool isHit;
    short _jumpNumber;
    Label* finish;

    void initW1();
	void initW2();
	void initW3();
    void setPhysicsWorld(cocos2d::PhysicsWorld *world){ sceneWorld=world;}
    Vector<SpriteFrame*> getAnimation(const char *format, int count);

    void jump();

    //schedule methods
    void scheduler();
    void PhysicsStep(float dt);

    void ScrollBG(float dt);
    void spawnObstacles(float dt);
    void spawnObsW2(float dt);
    void checkFinish();

    //hardware buttons listener
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // touch events
   	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
   	bool onTouchBeganW2(cocos2d::Touch *touch, cocos2d::Event *event);
   	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
   	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
   	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);


   	//Physics Contact Listener
   	bool onContactBegin(cocos2d::PhysicsContact &contact);
   	bool onContactBeginW2(cocos2d::PhysicsContact &contact);
   	//ingame menus
   	void createMenu();
   	void showMenu(Ref* pSender);
   	void updateHealth();
   	void updatePoints();


   	bool isFloating;
   	void swim(){isFloating=false;};
   	void stopSwimming(float dt);
   	void Float();



};

#endif // __TAGGAME_H__
