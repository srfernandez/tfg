#include "TagGameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* TagGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene ->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = TagGame::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TagGame::init()
{
    //////////////////////////////
    // 1. super init first
	if(!cocos2d::Layer::init()){ return false;}
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //background creation

    bk1 = Sprite::create("menuBG.png");
    bk1->setAnchorPoint(Vec2(0,0));
    bk1->setPosition(Vec2(0,0));

    bk2 = Sprite::create("menuBG.png");
    bk2->setAnchorPoint(Vec2(0,0));
    bk2->setPosition( Vec2(visibleSize.width, 0));

    // add the sprite as a child to this layer
    this->addChild(bk1, 0);
    this->addChild(bk2, 0);

    auto bg_action = cocos2d::RepeatForever::create(cocos2d::MoveBy::create(2.0, Point(-visibleSize.width,0)));
    bk1->runAction(bg_action);

    auto bg_action_2 = cocos2d::RepeatForever::create(cocos2d::MoveBy::create(2.0, Point(-visibleSize.width,0)));
    bk1->runAction(bg_action_2);

    this->schedule(schedule_selector(TagGame::ScrollBG ),OBSTACLE_SPAWN_FREQUENCY);
    /*auto _background= Sprite::create("menuBG.png");
	_background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	this->addChild(_background);*/

	auto edgeBody= PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeBody->setCollisionBitmask(GROUND_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(false);

	auto edgeNode= Node::create();
	edgeNode->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);
	this->schedule(schedule_selector(TagGame::spawnFoes ), OBSTACLE_SPAWN_FREQUENCY*visibleSize.width*2);


	//contact listener for protagonist
	protag = new Protag( this );
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(TagGame::onContactBegin, this);
	//contactListener->onContactPreSolve = CC_CALLBACK_2(TagGame::onContactPreSolve, this);
	//contactListener->onContactPostSolve = CC_CALLBACK_2(TagGame::onContactPostSolve,this);
	//contactListener->onContactSeparate = CC_CALLBACK_1(TagGame::onContactSeparate,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);

	//Swipe listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(TagGame::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TagGame::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TagGame::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TagGame::onTouchCancelled, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    isTouchDown = false;

    initialTouchPos[0] = 0;
    initialTouchPos[1] = 0;

    this->scheduleUpdate();
    
    return true;
}

void TagGame::spawnFoes(float dt){
	obstacle.spawnObstacles(this);
}

void TagGame::ScrollBG(float dt){
	if(bk1->getPosition().x <= visibleSize.width +1){
			bk1->setPosition(Vec2(visibleSize.width,0));
		}
		if(bk2->getPosition().x <= visibleSize.width +1){
			bk2->setPosition(Vec2(visibleSize.width,0));
		}
}

bool TagGame::onContactBegin(cocos2d::PhysicsContact &contact){
	PhysicsBody *a=contact.getShapeA()->getBody();
	PhysicsBody *b=contact.getShapeB()->getBody();

	//Character touches an obstacle
	if((PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())
			|| (PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask())){

		auto scene = MainMenu::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	//Character touches the ground
	if((PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())
				|| (PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask())){

			protag->setTouchGround(true);
		}
	return true;
}


void TagGame::update(float dt)
{
    if (true == isTouchDown)
    {
        if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05)
        {
            CCLOG("SWIPED DOWN");

            isTouchDown = false;
            auto MENU = MainMenu::createScene();
            Director::getInstance()->replaceScene(MENU);
        }
        else if (initialTouchPos[1] - currentTouchPos[1] < - visibleSize.width * 0.05)
        {
            CCLOG("SWIPED UP");

            isTouchDown = false;
            protag->Jump();
            this->scheduleOnce( schedule_selector( TagGame::StopJumping ), PROTAG_JUMP_DURATION );
        }
    }

    protag->Fall();
}

void TagGame::StopJumping(float dt){
	protag->StopJumping();
}

bool TagGame::onTouchBegan(Touch *touch, Event *event)
{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    isTouchDown = true;
    
    return true;
}

void TagGame::onTouchMoved(Touch *touch, Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
}

void TagGame::onTouchEnded(Touch *touch, Event *event)
{
    isTouchDown = false;
}

void TagGame::onTouchCancelled(Touch *touch, Event *event)
{
    onTouchEnded(touch, event);
}

void TagGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
