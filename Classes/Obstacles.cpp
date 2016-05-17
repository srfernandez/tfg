#include "Obstacles.h"
#include "Definitions.h"
USING_NS_CC;

using namespace cocos2d;
Obstacles::Obstacles()
{
   visibleSize= Director::getInstance()->getVisibleSize();
   origin= Director::getInstance()->getVisibleOrigin();
}

void Obstacles::spawnObstacles(cocos2d::Layer *layer)
{
    CCLOG("HHHH");
    auto obstacle = Sprite::create("Obs.png");

    auto obstacleBody = PhysicsBody::createBox(obstacle->getContentSize());

    auto random=CCRANDOM_0_1();
    obstacleBody->setDynamic(false);
    obstacleBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    obstacleBody->setContactTestBitmask(true);

    obstacle->setPhysicsBody(obstacleBody);


    obstacle->setPosition(visibleSize.width, 0);
    layer->addChild(obstacle);

    auto obstacleAction = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width*1.5,0));

    obstacle->runAction(obstacleAction);
}


