/*
 * MainMenuScene.h
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#ifndef __MAINMENUSCENE_H__
#define __MAINMENUSCENE_H__

#include "cocos2d.h"


using namespace cocos2d;

class MainMenu: public cocos2d::Layer{
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;

private:
	Sprite* _background;
	void createMenu();
	void play(Ref* pSender);
	void exit(Ref* pSender);

	CREATE_FUNC(MainMenu);
};


#endif /* __MAINMENUSCENE_H__ */
