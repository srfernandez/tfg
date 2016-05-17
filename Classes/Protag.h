#ifndef __PROTAG_H__
#define __PROTAG_H__

#include "cocos2d.h"

using namespace cocos2d;
class Protag
{
public:
	Protag( cocos2d::Layer *layer );
	 void Fall( );
	 void Jump( );
	 void StopJumping( ) { isFalling = true; }
	 void setTouchGround (bool m){ touchGround=m;}
	 bool isTouchingGround(){ return touchGround;}

private:
    Size visibleSize;
    Vec2 origin;
    cocos2d::Sprite *protag;
    bool isFalling;
    bool touchGround;


};

#endif // __PROTAG_H__
