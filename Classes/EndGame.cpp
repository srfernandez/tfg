/*
 * MainMenuScene.cpp
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#include "TagGameScene.h"
#include "EndGame.h"
#include "MainMenuScene.h"
#include <stdio.h>
#include "json/document.h"
#include "json/filestream.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"

USING_NS_CC;
using namespace rapidjson;
Scene* EndGame::createScene(){
	auto scene= Scene::create();

	auto layer = EndGame::create();

	scene->addChild(layer);

	return scene;
}

bool EndGame::init() {
	if(!cocos2d::Layer::init()){ return false;}

	origin=Director::getInstance() -> getVisibleOrigin();
	visibleSize= Director::getInstance()-> getVisibleSize();

	_background= Sprite::create("background.png");
	_background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	_background->setOpacity(240);

	this->addChild(_background, 20);
	_isWon=UserDefault::getInstance()->getBoolForKey("Won");
	_points = UserDefault::getInstance()->getIntegerForKey("points");

	rapidjson::Document d;
		FILE* fileRead = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "r");

		FileStream fs(fileRead);
		d.ParseStream<0>(fs);
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

		auto world=d["Partida"]["World"].GetInt();
		_difficulty=d["Partida"]["Difficulty"].GetInt();

	auto menu_item_1 = MenuItemImage::create("Menu.png", "Menu-pressed.png", CC_CALLBACK_1(EndGame::Back,this));
	auto menu_item_2 = MenuItemImage::create("Retry.png", "Retry-pressed.png", CC_CALLBACK_1(EndGame::Retry,this));
	auto menu_item_3 = MenuItemImage::create("continue.png", "continue-pressed.png", CC_CALLBACK_1(EndGame::Continue,this));
	auto menu_item_4 = MenuItemImage::create("NewGame.png", "NewGame-pressed.png", CC_CALLBACK_1(EndGame::Play,this));

	if(_isWon==true ){
		_endGame= cocos2d::Label::createWithTTF(" ", "fonts/ArcadeClassic.TTF", 250);
		char points[50];
		sprintf(points, "You   won   %d P", _points);
		std::string pointsTxt (points);
		_endGame->setString(pointsTxt);
		if(world==3){
			menu = Menu::create(menu_item_1, menu_item_2, menu_item_4,  NULL);
		}else{
			menu = Menu::create(menu_item_1, menu_item_2,menu_item_3,  NULL);
		}
	}
	else{
	_endGame= cocos2d::Label::createWithTTF(" ", "fonts/ArcadeClassic.TTF", 250);
	char points[50];
	sprintf(points, "You   lost  %d P", _points);
		std::string pointsTxt (points);
		_endGame->setString(pointsTxt);
		menu = Menu::create(menu_item_1, menu_item_2,  NULL);

	}
	_endGame->setPosition(Vec2(origin.x + visibleSize.width/2,
										origin.y + visibleSize.height/2));
	this->addChild(_endGame, 50);

	menu->alignItemsHorizontallyWithPadding(40);
	menu->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/3));
	this->addChild(menu,40);
	return true;


}


void EndGame::Back(Ref* pSender) {
	updateSaves();
	auto menu = MainMenu::createScene();
	Director::getInstance()->replaceScene(menu);
}

void EndGame::Retry(Ref* pSender) {
	auto retry = TagGame::createScene();
	Director::getInstance()->replaceScene(retry);
}
void EndGame::Play(Ref* pSender) {
		rapidjson::Document d;
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
		d.SetObject();

		rapidjson::Value Partida(rapidjson::kObjectType);
		Partida.AddMember("World",1,allocator);
		Partida.AddMember("Difficulty",_difficulty,allocator);
		d.AddMember("Partida",Partida,allocator);

		rapidjson::StringBuffer  buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);
		FILE* fileWrite = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "w");
		if (fileWrite)
		{
				fputs(buffer.GetString(),fileWrite);
				fclose(fileWrite);
		}

		auto Newgame = TagGame::createScene();
		Director::getInstance()->replaceScene(Newgame);
}
void EndGame::updateSaves(){
	rapidjson::Document d;
	FILE* fileRead = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "r");

	FileStream fs(fileRead);
	d.ParseStream<0>(fs);
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

	auto world=d["Partida"]["World"].GetInt();
	auto difficulty=d["Partida"]["Difficulty"].GetInt();
	if (world<3){
		world = world+1;}
	d.RemoveAllMembers();

	rapidjson::StringBuffer  buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	FILE* fileWrite = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "w");
	if (fileWrite)
		{
			fputs(buffer.GetString(),fileWrite);
			fclose(fileWrite);
		}
	rapidjson::Value Partida(rapidjson::kObjectType);
	Partida.AddMember("World",world,allocator);
	Partida.AddMember("Difficulty",difficulty,allocator);
	d.AddMember("Partida",Partida,allocator);

	rapidjson::StringBuffer  buffer2;
	rapidjson::Writer<rapidjson::StringBuffer> writer2(buffer2);
	d.Accept(writer2);
	FILE* fileWrite2 = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "w");
	if (fileWrite2)
	{
		fputs(buffer2.GetString(),fileWrite2);
		fclose(fileWrite2);
	}
}
void EndGame::Continue(Ref* pSender) {
	if(_isWon==true){
		updateSaves();
	}
	auto retry = TagGame::createScene();
	Director::getInstance()->replaceScene(retry);
}

