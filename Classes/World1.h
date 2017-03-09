#ifndef __WORLD1_H__
#define __WORLD1_H__

#include "cocos2d.h"

using namespace cocos2d;
class World1
{
public:
	World1( cocos2d::Layer *layer );
	 int Jump(int a,int b );
	 void dashLeft();
	 void dashRight();
	 void spawnObstacles(cocos2d::Layer *layer);
	 void getRunningAnimation();
	 Vec2 getCurrentPos();
	Vector<SpriteFrame*> getAnimation(const char *format, int count);


private:
    Size visibleSize;
    Vec2 origin;
    Sprite *_bg1;
    Sprite *_bg2;
    Sprite *_foreground;
    Sprite *_foreground2;
    cocos2d::Sprite *protag;
    Vector<SpriteFrame *> frames;
	Vector<SpriteFrame *> jumpFrames;
	Vector <SpriteFrame *> dashFrames;
    Animation *animationArray[3];

};

#endif // __WORLD1_H__
