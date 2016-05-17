/*
 * MainMenuScene.cpp
 *
 *  Created on: 12/05/2016
 *      Author: Susana
 */
#include "TagGameScene.h"
#include "MainMenuScene.h"

#include <stdio.h>

USING_NS_CC;

Scene* MainMenu::createScene(){
	auto scene= Scene::create();

	auto layer = MainMenu::create();

	scene->addChild(layer);

	return scene;
}

bool MainMenu::init() {
	if(!cocos2d::Layer::init()){ return false;}

	origin=Director::getInstance() -> getVisibleOrigin();
	visibleSize= Director::getInstance()-> getVisibleSize();

	_background= Sprite::create("menuBG.png");
	_background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	this->addChild(_background);


	MenuItemFont::setFontSize(100);

	auto menu_item_1 = MenuItemFont::create("New Game", CC_CALLBACK_1(MainMenu::play,this));
	auto menu_item_2= MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenu::exit,this));
	auto *menu = Menu::create(menu_item_1, menu_item_2, NULL);
	menu->alignItemsVertically();
	this->addChild(menu);


	    return true;
}

void MainMenu::createMenu(){
	auto playButton = MenuItemImage::create("play_button.png",
				"play_button_pressed.png", CC_CALLBACK_1(MainMenu::exit, this));

		playButton->setAnchorPoint(Vec2(0.5,0.5));
		playButton->setPosition(Vec2(origin.x + visibleSize.width/2,
						origin.y + visibleSize.height/2));

	auto exitButton = MenuItemImage::create("exit_button.png",
						"exit_button_pressed.png", CC_CALLBACK_1(MainMenu::exit, this));

				exitButton->setAnchorPoint(Vec2(0,0));
				exitButton->setPosition(Vec2(origin.x + visibleSize.width/2,
                        origin.y + visibleSize.height - playButton->getContentSize().height));


	auto menu = Menu::create(playButton, exitButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF("HIIIIII", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(origin.x + visibleSize.width/2,
	                            origin.y + visibleSize.height - label->getContentSize().height));

	    // add the label as a child to this layer
	    this->addChild(label, 1);
}

void MainMenu::play(Ref* pSender) {
	auto Newgame = TagGame::createScene();
	Director::getInstance()->replaceScene(Newgame);
}
void MainMenu::exit(Ref* pSender) {
	Director::getInstance()->end();
}


