/*
 * MainMenuScene.h
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#ifndef __GAMEMENUSCENE_H__
#define __GAMEMENUSCENE_H__

#include "cocos2d.h"


using namespace cocos2d;

class GameMenu: public cocos2d::Layer{
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;
	short _margin;


private:
	Sprite* _background;
	void resume(Ref* pSender);
	void exit(Ref* pSender);
	CREATE_FUNC(GameMenu);
};


#endif /* __GAMEMENUSCENE_H__ */
