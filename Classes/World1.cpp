#include "World1.h"
#include "Definitions.h"
USING_NS_CC;

World1::World1(cocos2d::Layer *layer)
{
	visibleSize= Director::getInstance()->getVisibleSize();
	   origin= Director::getInstance()->getVisibleOrigin();


	   auto jumping = SpriteFrame::create("jumping.png", Rect(0,0,239,239));

		for(int i = 1; i <= 8; i++)
			{
				jumpFrames.pushBack(jumping);

			}

		auto _foreground=Sprite::createWithSpriteFrameName("foreground.png");
		auto edgeBody= PhysicsBody::createEdgeBox(_foreground->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 3);
		edgeBody->setCollisionBitmask(GROUND_COLLISION_BITMASK);
		edgeBody->setContactTestBitmask(true);
		edgeBody->setDynamic(false);

		auto edgeNode= Node::create();
		edgeNode->setPosition(Point(visibleSize.width/2 + origin.x, _foreground->getContentSize().height/2));

		edgeNode->setPhysicsBody(edgeBody);

		layer->addChild(edgeNode);
	  protag=Sprite::createWithSpriteFrameName("0001.png");
	   protag->setAnchorPoint(Vec2(0.5,0));
	   protag->setPosition(visibleSize.width/2 + origin.x, 0);

	   frames = getAnimation("%04d.png", 16);

		protag = Sprite::createWithSpriteFrame(frames.front());

		 //protag=Sprite::create("Obs.png");
	   protag->setAnchorPoint(Vec2(0.5,0));
	   protag->setPosition(visibleSize.width/2 + origin.x, _foreground->getContentSize().height);

	   auto protagBody= PhysicsBody::createBox(protag->getContentSize());
	   protagBody->setDynamic(true);
	   protagBody->setCollisionBitmask(PROTAG_COLLISION_BITMASK);
	   protagBody ->setContactTestBitmask(true);
	   protagBody ->getShape(0)->setRestitution(0);
	   protagBody->setGravityEnable(false);
	   protag->setPhysicsBody(protagBody);
	   layer->addChild(protag,15);
		//_animation = Animation::createWithSpriteFrames(frames, 1.0f/8);

	   auto a1= Animation::createWithSpriteFrames(frames, 0.1f);
		  a1->retain();
	   auto a2 = Animation::createWithSpriteFrames(jumpFrames,1.0f/8);
			a2->retain();

		animationArray[0] = a1;
		animationArray[1] = a2;
		protag->runAction(RepeatForever::create(Animate::create(animationArray[0])));
		//protag->runAction(RepeatForever::create(Animate::create(_animation)));


}

Vec2 World1::getCurrentPos(){
	return protag->getPosition();
}

void World1::spawnObstacles(cocos2d::Layer *layer){

	auto random=cocos2d::random(1,3);

	char obs[50];
	sprintf(obs, "obstacleW1_%d.png", random);
	std::string obsTxt (obs);
	auto obstacle=Sprite::createWithSpriteFrameName(obsTxt);
	auto obstacleBody = PhysicsBody::createBox(obstacle->getContentSize());

	auto _foreground= Sprite::createWithSpriteFrameName("foreground.png");

   obstacleBody->setDynamic(false);
   obstacleBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
   obstacleBody->setContactTestBitmask(true);

   obstacle->setPhysicsBody(obstacleBody);
   obstacle->setAnchorPoint(Vec2(0.5,0));
   obstacle->setPosition(visibleSize.width, _foreground->getContentSize().height);
   layer->addChild(obstacle,15);

   auto controller=Sprite::createWithSpriteFrameName("controller.png");
   	controller->setAnchorPoint(Vec2(0.5,0));
   	controller->setPosition(visibleSize.width, visibleSize.height/2);
   	auto controllerBody= PhysicsBody::createBox(controller->getContentSize());
   	controllerBody->setDynamic(true);
   	controllerBody->setCollisionBitmask(CONTROLLER_COLLISION_BITMASK);
   	controllerBody ->setContactTestBitmask(true);
   	controllerBody ->getShape(0)->setRestitution(0);
   	controller->setPhysicsBody(controllerBody);
   	layer->addChild(controller);


   auto r = cocos2d::random(0,2);
   if(r==1){
   	   			auto highGround=Sprite::create("bonus.png");
   	   			highGround->setAnchorPoint(Vec2(0.5,0));
   	   			highGround->setPosition(visibleSize.width, visibleSize.height/2);

   	   			auto groundBody = PhysicsBody::createBox(highGround->getContentSize());
   	   			groundBody->setCollisionBitmask(BONUS_COLLISION_BITMASK);
   	   			groundBody->setContactTestBitmask(true);
   	   			groundBody->setDynamic(false);
   	   			highGround->setPhysicsBody(groundBody);
   	   			layer->addChild(highGround,15);
   	   			auto action = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
   	   			highGround->runAction(action);
   	   		}
   else if(r==2){
   				auto health=Sprite::create("bonusHealth.png");
   				health->setAnchorPoint(Vec2(0.5,0));
   				health->setPosition(visibleSize.width,visibleSize.height/2);

   				auto healthBody = PhysicsBody::createBox(health->getContentSize());
   				healthBody->setCollisionBitmask(BONUSHEALTH_COLLISION_BITMASK);
   				healthBody->setContactTestBitmask(true);
   				healthBody->setDynamic(false);
   				health->setPhysicsBody(healthBody);
   				layer->addChild(health,15);
   				auto action2 = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
   				health->runAction(action2);
      			}
   auto obstacleAction = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
   auto controllerAction = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
   obstacle->runAction(obstacleAction);
   controller->runAction(controllerAction);
}

int World1::Jump(int a, int b){
	if(a==1){
		b=b+1;
		protag->stopAllActions();
		protag->runAction(RepeatForever::create(Animate::create(animationArray[1])));
	}
	auto jumping=cocos2d::JumpBy::create(1.3,Vec2(0,0),300,1);
	protag->runAction(jumping);
	return b;
}

void World1::getRunningAnimation(){
	protag->stopAllActions();
	protag->runAction(RepeatForever::create(Animate::create(animationArray[0])));
}

void World1::dashLeft(){
	if(protag->getPosition().x-250 < protag->getContentSize().width/2){

		auto action=MoveTo::create(0.7,Vec2(origin.x + protag->getContentSize().width/2, protag->getPosition().y));
		protag->runAction(action);

	}
	else{

		auto action=MoveTo::create(0.7,Vec2(protag->getPosition().x-250, protag->getPosition().y));
		protag->runAction(action);
	}
}

void World1::dashRight(){
	if(protag->getPosition().x+200 > visibleSize.width - protag->getContentSize().width/2){

			auto action=MoveTo::create(0.7,Vec2(visibleSize.width - protag->getContentSize().width/2, protag->getPosition().y));
			protag->runAction(action);
	}
	else{
			auto action=MoveTo::create(0.7,Vec2(protag->getPosition().x+200, protag->getPosition().y));
			protag->runAction(action);
	}
}
Vector<SpriteFrame *> World1::getAnimation(const char *format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame *> animFrames(16);
    char str[100];
    for(int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
   return animFrames;

}
