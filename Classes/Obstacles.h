#ifndef __OBSTACLES_H__
#define __OBSTACLES_H__

#include "cocos2d.h"

using namespace cocos2d;
class Obstacles
{
public:
	Obstacles();

    void spawnObstacles(cocos2d::Layer* layer);

private:
    Size visibleSize;
    Vec2 origin;

};

#endif // __OBSTACLES_H__
