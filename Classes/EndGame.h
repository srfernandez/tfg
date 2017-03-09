/*
 * MainMenuScene.h
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#ifndef __ENDGAMESCENE_H__
#define __ENDGAMESCENE_H__

#include "cocos2d.h"
#include "json/document.h"
#include "json/filestream.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"

using namespace cocos2d;

class EndGame: public cocos2d::Layer{
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;
	short _margin;


private:
	short _difficulty;
	rapidjson::Document saveFile;
	Menu* menu;
	Sprite* _background;
	Label* _endGame;
	void Back(Ref* pSender);
	void Retry(Ref* pSender);
	void Play(Ref* pSender);
	void Continue(Ref* pSender);
	void updateSaves();
	CREATE_FUNC(EndGame);
	bool _isWon;
	short _points;
};


#endif /* __ENDGAMESCENE_H__ */
