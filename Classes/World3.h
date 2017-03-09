#ifndef __WORLD3_H__
#define __WORLD3_H__

#include "cocos2d.h"

using namespace cocos2d;
class World3
{
public:
	World3( cocos2d::Layer *layer );
	 int Jump(int a,int b );
	 void dashLeft();
	 void dashRight();
	 void spawnObstacles(cocos2d::Layer *layer);
	 void getRunningAnimation();
	Vector<SpriteFrame*> getAnimation(const char *format, int count);
	void setPosCharacter();
	Vec2 getCurrentPos();




private:
    Size visibleSize;
    Vec2 origin;
    cocos2d::Sprite *protag;
    Sprite *highGround;
    Vector<SpriteFrame *> frames;
	Vector<SpriteFrame *> jumpFrames;
	Vector <SpriteFrame *> dashFrames;
    Animation *animationArray[3];


};

#endif // __WORLD3_H__
