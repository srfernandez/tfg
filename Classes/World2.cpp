/*
 * MainMenuScene.cpp
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#include "TagGameScene.h"
#include "World2.h"
#include "Definitions.h"
#include "GameMenu.h"
#include "EndGame.h"
#include <stdio.h>

USING_NS_CC;


World2::World2(cocos2d::Layer *layer) {

	origin=Director::getInstance() -> getVisibleOrigin();
	visibleSize= Director::getInstance()-> getVisibleSize();


	isFloating=true;
	//character creation
	frames = getAnimation("swimming_%04d.png", 4);
	protag=Sprite::create("Obs.png");


	protag=Sprite::createWithSpriteFrame(frames.front());
	protag->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

	auto pBody = PhysicsBody::createBox( protag->getContentSize());
	pBody->setDynamic(true);
	pBody->setCollisionBitmask(PROTAG_COLLISION_BITMASK);
	pBody ->setContactTestBitmask(true);
	//pBody ->getShape(0)->setRestitution(0);
	pBody->setGravityEnable(false);

	protag->setPhysicsBody( pBody );

	layer->addChild( protag, 100 );

	 auto a1= Animation::createWithSpriteFrames(frames, 0.2f);
	 protag->runAction(RepeatForever::create(Animate::create(a1)));

	 //obsFrames = getAnimation("shark_04%d.png",13);
	//auto obstacle = Sprite::create("obstacleW2.png");


}


void World2::stopSwimming(){
	isFloating=true;
}
void World2::swim(){
	isFloating=false;
}
void World2::Fall(){
	 if ( true == isFloating )
		    {
		        protag->setPositionX( visibleSize.width / 2 + origin.x );
		        protag->setPositionY( protag->getPositionY() - ( PROTAG_FALLING_SPEED * visibleSize.height  ) );
		    }
		    else
		    {
		        protag->setPositionX( visibleSize.width / 2 + origin.x );
		        protag->setPositionY( protag->getPositionY() + ( PROTAG_FALLING_SPEED * visibleSize.height * 2 ) );
		    }
}


void World2::spawnObstacles(cocos2d::Layer *layer){
	auto random=cocos2d::random(0,1);

	auto controller=Sprite::createWithSpriteFrameName("controller.png");


	auto controllerBody= PhysicsBody::createBox(controller->getContentSize());
	controllerBody->setDynamic(true);
	controllerBody->setCollisionBitmask(CONTROLLER_COLLISION_BITMASK);
	controllerBody ->setContactTestBitmask(true);
	controllerBody ->getShape(0)->setRestitution(0);
	controller->setPhysicsBody(controllerBody);
	layer->addChild(controller,0);

	auto obstacle= Sprite::createWithSpriteFrameName("shark_0010.png");
	obsFrames=getAnimation("shark_%04d.png", 13);
	obstacle=Sprite::createWithSpriteFrame(obsFrames.front());
	auto obsBody = PhysicsBody::createBox(obstacle->getContentSize());

	obsBody->setDynamic(false);
	obsBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
	obsBody->setContactTestBitmask( true );
	obstacle->setPhysicsBody( obsBody );

	obstacle->setPosition(visibleSize.width/2, visibleSize.height/2);

	auto r = cocos2d::random(0,1080);

	if(r<visibleSize.height/2){
		obstacle->setAnchorPoint(Vec2(0.5,0));
	}
	else{
		obstacle->setAnchorPoint(Vec2(0.5,1));
	}
	obstacle->setPosition(visibleSize.width + obstacle->getContentSize( ).width + origin.x, r );
	controller->setPosition(visibleSize.width + obstacle->getContentSize( ).width + origin.x, visibleSize.height/2 );
	layer->addChild(controller,0);
	layer->addChild(obstacle,90);

	auto randomBonus = cocos2d::random(0,2);
   if(randomBonus==1){
				auto bonus=Sprite::create("bonus.png");
				bonus->setAnchorPoint(Vec2(0.5,0));
				if ((r<visibleSize.height/2)){

					bonus->setPosition(visibleSize.width + obstacle->getContentSize( ).width , obstacle->getPositionY() + visibleSize.height/4);
				}
				else{
					bonus->setPosition(visibleSize.width + obstacle->getContentSize( ).width , obstacle->getPositionY() - visibleSize.height/4);
				}

				auto groundBody = PhysicsBody::createBox(bonus->getContentSize());
				groundBody->setCollisionBitmask(BONUS_COLLISION_BITMASK);
				groundBody->setContactTestBitmask(true);
				groundBody->setDynamic(false);
				bonus->setPhysicsBody(groundBody);
				layer->addChild(bonus,75);
				auto action = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
				bonus->runAction(action);
			}
   else if(randomBonus==2){
				auto health=Sprite::create("bonusHealth.png");
				health->setAnchorPoint(Vec2(0.5,0));
				if ((r<visibleSize.height/2)){
					health->setPosition(visibleSize.width + obstacle->getContentSize( ).width , obstacle->getPositionY() + visibleSize.height/4);
				}
				else{
					health->setPosition(visibleSize.width + obstacle->getContentSize( ).width , obstacle->getPositionY() - visibleSize.height/4);
				}

				auto healthBody = PhysicsBody::createBox(health->getContentSize());
				healthBody->setCollisionBitmask(BONUSHEALTH_COLLISION_BITMASK);
				healthBody->setContactTestBitmask(true);
				healthBody->setDynamic(false);
				health->setPhysicsBody(healthBody);
				layer->addChild(health,15);
				auto action2 = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
				health->runAction(action2);
				}
	auto a= Animation::createWithSpriteFrames(obsFrames, 0.2f);
	obstacle->runAction(RepeatForever::create(Animate::create(a)));
	auto controllerAction = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
	auto obstacleAction = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
	obstacle->runAction(obstacleAction);
	controller->runAction(controllerAction);

}
Vec2 World2::getCurrentPos(){
	return protag->getPosition();
}

Vector<SpriteFrame *> World2::getAnimation(const char *format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame *> animFrames;
    char str[100];
    for(int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
   return animFrames;

}
