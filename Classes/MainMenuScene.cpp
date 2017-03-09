/*
 * MainMenuScene.cpp
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#include "TagGameScene.h"
#include "World2.h"
#include "MainMenuScene.h"
#include <stdio.h>
#include "external/json/document.h"
#include "json/prettywriter.h"
#include "json/filestream.h"
#include "json/stringbuffer.h"

USING_NS_CC;

using namespace rapidjson;
Scene* MainMenu::createScene(){
	auto scene= Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;

}

bool MainMenu::init() {
	if(!cocos2d::Layer::init()){ return false;}
	auto path = FileUtils::getInstance()->getWritablePath();
	//CCLog("path = %s", path.c_str());


	origin=Director::getInstance() -> getVisibleOrigin();
		visibleSize= Director::getInstance()-> getVisibleSize();

		_background= Sprite::create("background.png");
		_background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		_background->setOpacity(240);
		_difficulty=0;


		this->addChild(_background);


	if(false == FileUtils::getInstance()->isFileExist("/data/data/tfg.videojuego.taggame/files/things.json")){
		Document d;
		Document::AllocatorType& allocator = d.GetAllocator();
		d.SetObject();

		StringBuffer  buffer;
		Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);
		FILE* fileWrite = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "w");
		if (fileWrite)
			{
				fputs(buffer.GetString(),fileWrite);
				fclose(fileWrite);
			}
		this->FirstGame();

	}


	this->cargarJson();

	this->createMenu();

	auto logo = Sprite::createWithSpriteFrameName("logo.png");
	logo->setPosition(Vec2(visibleSize.width/2+origin.x, visibleSize.height- visibleSize.height/3));
	this->addChild(logo,20);
	logo->setOpacity(0);



	auto fade = FadeIn::create(3);
	logo->runAction(fade);
	//menu->runAction(fade);

	return true;
}

void MainMenu::cargarJson(){
	FILE* fileRead = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "r");

	FileStream fs(fileRead);
	this->saveFile.ParseStream<0>(fs);
}


void MainMenu::FirstGame(){
	rapidjson::Document d;
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
		d.SetObject();

		rapidjson::Value Partida(rapidjson::kObjectType);
		Partida.AddMember("World",0,allocator);
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
}

void MainMenu::createMenu(){
	auto menu_item_1 = MenuItemImage::create("NewGame.png", "NewGame-pressed.png", CC_CALLBACK_1(MainMenu::play,this));
		//auto menu_item_2 = MenuItemImage::create("Mode.png", "Mode-pressed.png", CC_CALLBACK_1(MainMenu::setDifficulty,this));
		item=cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(MainMenu::setDifficulty,this),
				MenuItemImage::create("easy.png", "easy-pressed.png"),
				MenuItemImage::create("normal.png", "normal-pressed.png"),
				MenuItemImage::create("hard.png", "hard-pressed.png"),nullptr);
		MenuItemImage* menu_item_3 = MenuItemImage::create("continue.png", "continue-pressed.png", CC_CALLBACK_1(MainMenu::Continue,this));
		auto menu_item_4 = MenuItemImage::create("Exit.png", "Exit-pressed.png", CC_CALLBACK_1(MainMenu::exit,this));

		menu = Menu::create(menu_item_1, item,menu_item_3,menu_item_4, NULL);
		menu->alignItemsHorizontallyWithPadding(40);
		menu->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/3));
		this->addChild(menu,20);

		auto aux = saveFile["Partida"]["World"].GetInt();
		if(aux == 0){
			menu_item_3->setOpacity(150);
			menu_item_3->setEnabled(false);
		}
}

void MainMenu::play(Ref* pSender) {

		rapidjson::Document d;
		FILE* fileRead = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "r");

		FileStream fs(fileRead);
		d.ParseStream<0>(fs);
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

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
		Partida.AddMember("World",1,allocator);
		Partida.AddMember("Difficulty",_difficulty,allocator);
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
		auto Newgame = TagGame::createScene();
		Director::getInstance()->replaceScene(Newgame);
	/*rapidjson::Document d;
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	d.SetObject();

	rapidjson::Value Partida(rapidjson::kObjectType);
	Partida.AddMember("World",1,allocator);
	Partida.AddMember("Difficulty",_difficulty,allocator);
	d.AddMember("Partida",Partida,allocator);

	rapidjson::StringBuffer  buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	FILE* fileWrite = fopen("/data/data/tfg.videojuego.taggame/files/savedFiles.json", "w");
	if (fileWrite)
	{
			fputs(buffer.GetString(),fileWrite);
			fclose(fileWrite);
	}

	auto Newgame = TagGame::createScene();
	Director::getInstance()->replaceScene(Newgame);*/
}

void MainMenu::setDifficulty(Ref* pSender) {
	_difficulty++;
		if(_difficulty==3){
			_difficulty=0;
		}
}

void MainMenu::Continue(Ref* pSender) {
		auto Newgame = TagGame::createScene();
		Director::getInstance()->replaceScene(Newgame);

}
void MainMenu::exit(Ref* pSender) {
	Director::getInstance()->end();
}



