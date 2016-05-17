#include "Protag.h"
#include "Definitions.h"
USING_NS_CC;

Protag::Protag(cocos2d::Layer *layer)
{
   visibleSize= Director::getInstance()->getVisibleSize();
   origin= Director::getInstance()->getVisibleOrigin();

   protag=Sprite::create("Obs.png");
   protag->setAnchorPoint(Vec2(0.5,0));
   protag->setPosition(visibleSize.width/2 + origin.x, 0);

   auto protagBody= PhysicsBody::createBox(protag->getContentSize());
   protagBody->setCollisionBitmask(PROTAG_COLLISION_BITMASK);
   protagBody ->setContactTestBitmask(true);
   protagBody ->getShape(0)->setRestitution(0);

   protag->setPhysicsBody(protagBody);

   layer->addChild(protag);

   isFalling=true;
   touchGround=true;
}

void Protag::Jump(){
	isFalling=false;
	touchGround=false;

	auto jumpAction= MoveBy::create(1.0, Point(visibleSize.width/2,visibleSize.height/3));
	protag->runAction(jumpAction);

}
void Protag::Fall( )
{
    if ( true == isFalling )
    {
    	protag->setPositionX( visibleSize.width / 2 + origin.x );
    	protag->setPositionY( protag->getPositionY() - ( PROTAG_FALLING_SPEED * visibleSize.height ) );
    }
    else
    {
    	protag->setPositionX( visibleSize.width / 2 + origin.x );
    	protag->setPositionY( protag->getPositionY() + ( PROTAG_FALLING_SPEED * visibleSize.height ) );
    }
}




