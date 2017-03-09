/*
 * MainMenuScene.h
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#ifndef __WORLD2SCENE_H__
#define __WORLD2SCENE_H__

#include "cocos2d.h"


using namespace cocos2d;

class World2 {
public:
	World2( cocos2d::Layer *layer );
	void spawnObstacles(cocos2d::Layer *layer);
    void stopSwimming();
    void swim();
    void Fall();
    Vector<SpriteFrame*> getAnimation(const char *format, int count);
	Vec2 origin;
	Size visibleSize;
	Vec2 getCurrentPos();




private:
	Sprite* protag;
	bool isFloating;
	Vector<SpriteFrame *> frames;
	Vector<SpriteFrame *> obsFrames;
	Animation *animationArray[3];


};


#endif /* __WORLD2SCENE_H__ */
