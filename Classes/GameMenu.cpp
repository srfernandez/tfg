/*
 * MainMenuScene.cpp
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#include "TagGameScene.h"
#include "GameMenu.h"
#include "MainMenuScene.h"
#include <stdio.h>

USING_NS_CC;

Scene* GameMenu::createScene(){
	auto scene= Scene::create();

	auto layer = GameMenu::create();

	scene->addChild(layer);

	return scene;
}

bool GameMenu::init() {
	if(!cocos2d::Layer::init()){ return false;}

	origin=Director::getInstance() -> getVisibleOrigin();
	visibleSize= Director::getInstance()-> getVisibleSize();

	_background= Sprite::create("background.png");
	_background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	_background->setOpacity(240);

	this->addChild(_background,10);

	auto paused= cocos2d::Label::createWithTTF("Paused", "fonts/ArcadeClassic.TTF", 250);

		paused->setPosition(Vec2(origin.x + visibleSize.width/2,
											origin.y + visibleSize.height/2));
		this->addChild(paused, 30);
	auto menu_item_1 = MenuItemImage::create("Resume.png", "Resume-pressed.png", CC_CALLBACK_1(GameMenu::resume,this));
	auto menu_item_2 = MenuItemImage::create("Quit.png", "Quit-pressed.png", CC_CALLBACK_1(GameMenu::exit,this));


	auto *menu = Menu::create(menu_item_1, menu_item_2, NULL);
	menu->alignItemsHorizontallyWithPadding(40);
	this->addChild(menu,20);
	menu->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/3));


	return true;
}


void GameMenu::resume(Ref* pSender) {
	Director::getInstance()->popScene();
	Director::getInstance()->resume();
}


void GameMenu::exit(Ref* pSender) {
	Director::getInstance()->resume();
	Director::getInstance()->popScene();
	auto menu = MainMenu::createScene();
	Director::getInstance()->replaceScene(menu);

}


