/*
 * MainMenuScene.h
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#ifndef __MAINMENUSCENE_H__
#define __MAINMENUSCENE_H__

#include "cocos2d.h"
#include "json/document.h"
#include "json/filestream.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"

using namespace cocos2d;

class MainMenu: public cocos2d::Layer{
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;
	short _margin;


private:
	Sprite* _background;
	Menu* menu;
	 rapidjson::Document saveFile;
	short _difficulty;
	cocos2d::MenuItemToggle* item;
	void play(Ref* pSender);
	void exit(Ref* pSender);
	void Continue(Ref* pSender);
	void setDifficulty(Ref* pSender);
	void cargarJson();
	void createMenu();
	void FirstGame();
	CREATE_FUNC(MainMenu);
};


#endif /* __MAINMENUSCENE_H__ */
