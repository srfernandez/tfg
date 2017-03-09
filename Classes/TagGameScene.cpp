#include "TagGameScene.h"
#include "MainMenuScene.h"
#include "GameMenu.h"
#include "EndGame.h"
#include "Definitions.h"
#include "json/document.h"
#include "json/filestream.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"


USING_NS_CC;

using namespace rapidjson;
Scene* TagGame::createScene()
{
    // 'scene' is an autorelease object

    auto scene = Scene::createWithPhysics();
    scene ->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    //scene->getPhysicsWorld()->setGravity(Vect(0,0));

    //scene->getPhysicsWorld()->setGravity(Vec2(0,-250.0f));
    //PhysicsWorld::DEBUGDRAW_NONE

    // 'layer' is an autorelease object
    auto layer = TagGame::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool TagGame::init()
{
	if(!cocos2d::Layer::init()){ return false;}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	FILE* fileRead = fopen("/data/data/tfg.videojuego.taggame/files/things.json", "r");

	FileStream fs(fileRead);
	this->saveFile.ParseStream<0>(fs);


	_world=saveFile["Partida"]["World"].GetInt();

	_difficulty=saveFile["Partida"]["Difficulty"].GetInt();
	//if(_world==2){ Director::getInstance()->getRunningScene()->getPhysicsWorld()->setGravity(Vect(0,0));}

	_currentHealth=3;
	_currentPoints=0;
	auxCombo=1;
	_toFinish=0;
	auxFinish=0;

	isHit=true;
	_jumpNumber=0;
	//_difficulty=UserDefault::getInstance()->getIntegerForKey("difficulty");
	_isWon=true;


	switch(_world){
		case 1:
			world1 = new World1( this );
			_bg1 = Sprite::create("bgW1.png");
			_bg2 = Sprite::create("bgW1.png");
			_foreground=Sprite::createWithSpriteFrameName("foreground.png");
			_foreground2 = Sprite::createWithSpriteFrameName("foreground.png");
			break;
		case 2:
			world2 = new World2( this);
			_bg1 = Sprite::create("bgW2.png");
			_bg2 = Sprite::create("bgW2.png");
			_foreground=Sprite::createWithSpriteFrameName("foreground2.png");
			_foreground2 = Sprite::createWithSpriteFrameName("foreground2.png");
			break;
		case 3:
			world3 = new World3(this);
			auto background= Sprite::create("background.png");
			background->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
			this->addChild(background,1);

			_bg1 = Sprite::createWithSpriteFrameName("BG.png");
			_bg2 = Sprite::createWithSpriteFrameName("BG.png");
			_foreground=Sprite::createWithSpriteFrameName("foreground3.png");
			_foreground2 = Sprite::createWithSpriteFrameName("foreground3.png");
			break;
	}

	audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("background_music.mp3", true);

			_bg1->setAnchorPoint(Vec2(0,0));
			_bg1->setPosition(Vec2(0,0));

			_bg2->setAnchorPoint(Vec2(0,0));
			_bg2->setPosition(Vec2(_bg1->getBoundingBox().size.width -1,0));


			// add the sprite as a child to this layer
			this->addChild(_bg1, 10);
			this->addChild(_bg2, 10);

		_foreground->setAnchorPoint(Vec2(0,0));
			_foreground->setPosition(Vec2(0,0));


			_foreground2->setAnchorPoint(Vec2(0,0));
			_foreground2->setPosition(Vec2(_foreground->getBoundingBox().size.width -1,0));
			this->addChild(_foreground2,20);
			this->addChild(_foreground,20);

			auto edgeBody= PhysicsBody::createEdgeBox(_foreground->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 3);
			edgeBody->setCollisionBitmask(GROUND_COLLISION_BITMASK);
			edgeBody->setContactTestBitmask(true);
			edgeBody->setDynamic(false);


			auto edgeNode= Node::create();
			edgeNode->setPosition(Point(visibleSize.width/2 + origin.x, _foreground->getContentSize().height/2));

			edgeNode->setPhysicsBody(edgeBody);

			this->addChild(edgeNode);
			auto edgeBody2= PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
				edgeBody2->setCollisionBitmask(EDGE_COLLISION_BITMASK);
				edgeBody2->setContactTestBitmask(false);

				auto edgeNode2= Node::create();
				edgeNode2->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

				edgeNode2->setPhysicsBody(edgeBody2);
				this->addChild(edgeNode2);




	createMenu();
	scheduler();

	isTouchingGround=true;

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(TagGame::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);



	//Swipe listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(TagGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TagGame::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TagGame::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(TagGame::onTouchCancelled, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	isTouchDown = false;

	initialTouchPos[0] = 0;
	initialTouchPos[1] = 0;

   this->scheduleUpdate();

	auto listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyReleased = CC_CALLBACK_2(TagGame::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithFixedPriority(listenerKeyboard, 1);

    return true;
}



void TagGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode==EventKeyboard::KeyCode::KEY_ESCAPE){
    	audio->pauseBackgroundMusic();
    	auto menu = MainMenu::createScene();
    	Director::getInstance()->replaceScene(menu);
    }
    else
        {
    		audio->pauseBackgroundMusic();
    		Director::getInstance()->pause();
    		auto gameMenu = GameMenu::createScene();
    		Director::getInstance()->pushScene(gameMenu);
        }
}

Vector<SpriteFrame *> TagGame::getAnimation(const char *format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame *> animFrames(8);
    char str[100];
    for(int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
   return animFrames;

}
void TagGame::jump(){
		isTouchingGround=false;
		_jumpNumber=_jumpNumber+1;
		/*if(_jumpNumber==1){
			auxFinish=auxFinish+1;
			protag->stopAllActions();
			protag->runAction(RepeatForever::create(Animate::create(animationArray[1])));
		}
		auto jumping=cocos2d::JumpBy::create(1.3,Vec2(0,0),300,1);
		protag->runAction(jumping);
		//auto jumping= MoveBy::create(0.5, Point(0,visibleSize.height/3));
*/
		if(world1 != NULL){
			auxFinish= world1->Jump(_jumpNumber,auxFinish);
		}
		else{
			auxFinish= world3->Jump(_jumpNumber,auxFinish);
		}
}

void TagGame::showMenu(Ref* pSender){
	//audio->pauseBackgroundMusic();
	Director::getInstance()->pause();
	auto gameMenu = GameMenu::createScene();
	Director::getInstance()->pushScene(gameMenu);
}

void TagGame::createMenu(){
	auto pause= Sprite::create("pause.png");

	auto menuButton = MenuItemImage::create("pause.png","pause.png", CC_CALLBACK_1(TagGame::showMenu, this));
	menuButton->setPosition(
				Vec2(
						origin.x + visibleSize.width
								- menuButton->getContentSize().width / 2,
						origin.y + visibleSize.height
								- menuButton->getContentSize().height / 2));

	auto menu = Menu::create(menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);


	_health= cocos2d::Label::createWithTTF("", "fonts/ArcadeClassic.TTF", 200);
	//_health->setAnchorPoint(Vec2(0,1));
	//_health->setPosition(Vec2(origin.x + _health->getContentSize().width / 2, origin.y + visibleSize.height - _health->getContentSize().height / 2));
	_health->setOpacity(0);
	_health->setPosition(Vec2(origin.x+visibleSize.width/2, visibleSize.height/2));
	this->addChild(_health, 70);

	skull= Sprite::create("skull.png");
	skull ->setAnchorPoint(Vec2(0.5,0));
	skull->setPosition(Vec2(visibleSize.width/2, (visibleSize.height/2) - 150));
	skull->setOpacity(0);
	this->addChild(skull,85);

	_points= cocos2d::Label::createWithTTF("", "fonts/ArcadeClassic.TTF", 100);
	_points->setAnchorPoint(Vec2(0,1));
	_points->setPosition(Vec2(origin.x + _points->getContentSize().width / 2, origin.y + visibleSize.height - _points->getContentSize().height / 2));
	this->addChild(_points, 71);
	updateHealth();
	updatePoints();

}


void TagGame::scheduler(){
	auto obs = _difficulty+2;
	this->schedule(schedule_selector(TagGame::spawnObstacles), (OBSTACLE_SPAWN_FREQUENCY/obs)*visibleSize.width/2);
	//this->schedule(schedule_selector(TagGame::spawnBonus), (OBSTACLE_SPAWN_FREQUENCY/obs)*visibleSize.height);
	this->schedule(schedule_selector(TagGame::ScrollBG), 0.01f);

}



void TagGame::checkFinish(){
	if(_difficulty ==0 &&_toFinish==5){
		UserDefault::getInstance()->setBoolForKey("Won",_isWon);
		UserDefault::getInstance()->setIntegerForKey("points", _currentPoints);
		auto GameWon = EndGame::createScene();
		Director::getInstance()->replaceScene(GameWon);
	}
	else if(_difficulty ==1 &&_toFinish==10){
		UserDefault::getInstance()->setBoolForKey("Won",_isWon);
		UserDefault::getInstance()->setIntegerForKey("points", _currentPoints);
		auto GameWon = EndGame::createScene();
		Director::getInstance()->replaceScene(GameWon);
	}
	else if(_difficulty==2 &&_toFinish==15){
		UserDefault::getInstance()->setBoolForKey("Won",_isWon);
		UserDefault::getInstance()->setIntegerForKey("points", _currentPoints);
		auto GameWon = EndGame::createScene();
		Director::getInstance()->replaceScene(GameWon);
	}
}
void TagGame::updateHealth(){
	char lives[50];
	sprintf(lives, "Lives %d", _currentHealth);
	std::string livesTxt (lives);
	_health->setString(livesTxt);
	auto fade = FadeIn::create(1);
	_health->runAction(fade);
	if(_currentHealth==0){
		_isWon=false;
		UserDefault::getInstance()->setBoolForKey("Won",_isWon);
		auto GameOver = EndGame::createScene();
		Director::getInstance()->replaceScene(GameOver);
	}

}

void TagGame::updatePoints(){
	if (isHit == true){
		_currentPoints=0;
		char points[50];
			sprintf(points, "Points   %d", _currentPoints);
			std::string pointsTxt (points);
			_points->setString(pointsTxt);
		isHit=false;
	}
	else{
		_currentPoints=_currentPoints+1;
		char points[50];
		sprintf(points, "Points   %d", _currentPoints);
		std::string pointsTxt (points);
		_points->setString(pointsTxt);
	}


}


void TagGame::spawnObstacles(float dt){
	//obstacle.spawnObstacles(this);
	if(world1!=NULL){
	world1->spawnObstacles(this);
	}
	else if(world2!=NULL){
		world2->spawnObstacles(this);
	}
	else if(world3!=NULL){
			world3->spawnObstacles(this);
		}
}

void TagGame::spawnObsW2(float dt){
	auto random=cocos2d::random(0,1);

	auto obstacle = Sprite::create("Obs.png");
	auto obsBody = PhysicsBody::createBox(obstacle->getContentSize());

	obsBody->setDynamic(false);
	obsBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
	obsBody->setContactTestBitmask( true );
	obstacle->setPhysicsBody( obsBody );

	auto r = cocos2d::random(0,1);
	auto Pos = ( r * visibleSize.height ) + ( obstacle->getContentSize( ).height / 2 );
	switch(random){
			case 0:
				obstacle->setPosition(visibleSize.width + obstacle->getContentSize( ).width + origin.x, Pos );
				break;
			case 1:
				obstacle->setPosition(visibleSize.width+obstacle->getContentSize().width+origin.x, Pos - obstacle->getContentSize().height);
				break;

		}

	this->addChild(obstacle,15);
	auto obstacleAction = MoveBy::create(OBSTACLE_MOVEMENT_SPEED * visibleSize.height, Point(-visibleSize.width*1.5,0));
	obstacle->runAction(obstacleAction);


}

void TagGame::ScrollBG(float dt){
	if(world2==NULL){
		_bg1->setPosition(Vec2(_bg1->getPosition().x-1, _bg1->getPosition().y));
		_bg2->setPosition(Vec2(_bg2->getPosition().x-1, _bg2->getPosition().y));
		if(_bg1->getPosition().x < -_bg1->getBoundingBox().size.width){
				_bg1->setPosition( Vec2( _bg2->getPosition().x + _bg2->getBoundingBox().size.width, _bg1->getPosition().y));
			}
			if(_bg2->getPosition().x < -_bg2->getBoundingBox().size.width){
				_bg2->setPosition( Vec2( _bg1->getPosition().x + _bg1->getBoundingBox().size.width, _bg2->getPosition().y));
			}

		_foreground->setPosition(Vec2(_foreground->getPosition().x-1, _foreground->getPosition().y));
		_foreground2->setPosition(Vec2(_foreground2->getPosition().x-1, _foreground2->getPosition().y));
		if(_foreground->getPosition().x < -_foreground->getBoundingBox().size.width){
				_foreground->setPosition( Vec2( _foreground2->getPosition().x + _foreground2->getBoundingBox().size.width, _foreground->getPosition().y));
			}
			if(_foreground2->getPosition().x < -_foreground2->getBoundingBox().size.width){
				_foreground2->setPosition( Vec2( _foreground->getPosition().x + _foreground->getBoundingBox().size.width, _foreground2->getPosition().y));
			}
	}

}

bool TagGame::onContactBegin(cocos2d::PhysicsContact &contact){
	PhysicsBody *a=contact.getShapeA()->getBody();
	PhysicsBody *b=contact.getShapeB()->getBody();

	//Character touches an obstacle
if(world1!=NULL){
	if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
				(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())){
		//protag->setTouchGround(true);
		_jumpNumber=0;
		world1->getRunningAnimation();
		/*protag->stopAllActions();
		protag->runAction(RepeatForever::create(Animate::create(animationArray[0])));*/

		checkFinish();
		//isTouchingGround=true;
		return true;
	}


	else if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
			(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())){
		_currentHealth=_currentHealth-1;
		audio->playEffect("effect.wav",false, 1.f, 0.f, 1.f);
		updateHealth();
		skull->setPosition(world1->getCurrentPos());
		auto fade = FadeIn::create(1);
		skull->runAction(fade);
		return false;

	}

	else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
					(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
				_toFinish= _toFinish+1;
				updatePoints();
				checkFinish();
					return false;

				}
	else if ((BONUS_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
						(BONUS_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
					_currentPoints= _currentPoints+4;
					updatePoints();

						return false;

					}
	else if ((BONUSHEALTH_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
							(BONUSHEALTH_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
						_currentHealth= _currentHealth+1;
						updateHealth();
							return false;

						}

	else if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && EDGE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
				(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask())){
			return true;

		}
	else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
				(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())){

				return false;

			}
	else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && EDGE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
					(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask())){

					return false;

				}
	else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
			(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())){

			return false;

		}

}
else if(world2!=NULL){
		 if ( ( PROTAG_COLLISION_BITMASK == a->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PROTAG_COLLISION_BITMASK == b->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
		    {
			 audio->playEffect("effect.wav",false, 1.f, 0.f, 1.f);
			 _currentHealth=_currentHealth-1;
		    	updateHealth();
		    	skull->setPosition(world2->getCurrentPos());
		    			auto fade = FadeIn::create(1);
		    			skull->runAction(fade);
		    	return false;
		    }
		 else if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
					(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())){
			 audio->playEffect("effect.wav",false, 1.f, 0.f, 1.f);
			 _currentHealth=_currentHealth-1;
			 updateHealth();
			 skull->setPosition(world2->getCurrentPos());
			 		auto fade = FadeIn::create(1);
			 		skull->runAction(fade);
			//isTouchingGround=true;
			return false;
		}

		 else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 				(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
		 			_toFinish= _toFinish+1;
		 			updatePoints();
		 			checkFinish();
		 				return false;

		 	}
		 else if ((BONUS_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
							(BONUS_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
						_currentPoints= _currentPoints+4;
						updatePoints();

							return false;

						}
		else if ((BONUSHEALTH_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
								(BONUSHEALTH_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
							_currentHealth= _currentHealth+1;
							updateHealth();
								return false;

							}

		 else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
				 				(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())){

				 				return false;

				 	}
		 else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 				(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())){

		 				return false;

		 			}
		 	else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && EDGE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 					(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask())){

		 					return false;

		 				}
	}
else if(world3!=NULL){
		 if ( ( PROTAG_COLLISION_BITMASK == a->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PROTAG_COLLISION_BITMASK == b->getCollisionBitmask( ) && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
		    {
			 audio->playEffect("effect.wav",false, 1.f, 0.f, 1.f);
			 _currentHealth=_currentHealth-1;
		    	updateHealth();
		    	skull->setPosition(world3->getCurrentPos());
		    			auto fade = FadeIn::create(1);
		    			skull->runAction(fade);
		    	return false;
		    }
		 else if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 		 				(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())){
		 		 		//protag->setTouchGround(true);
		 		 		_jumpNumber=0;
		 		 		world3->getRunningAnimation();
		 		 		/*protag->stopAllActions();
		 		 		protag->runAction(RepeatForever::create(Animate::create(animationArray[0])));*/

		 		 		auxFinish=_toFinish;
		 		 		checkFinish();
		 		 		//isTouchingGround=true;
		 		 		return true;
		 		 	}
		 else if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && HIGHGROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
								(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && HIGHGROUND_COLLISION_BITMASK == b->getCollisionBitmask())){
						//protag->setTouchGround(true);
						_jumpNumber=0;
						world3->getRunningAnimation();
						world3->setPosCharacter();
						/*protag->stopAllActions();
						protag->runAction(RepeatForever::create(Animate::create(animationArray[0])));*/

						auxFinish=_toFinish;
						checkFinish();
						//isTouchingGround=true;
						return false;
					}
		 else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
								(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
				_toFinish= _toFinish+1;
				updatePoints();
				checkFinish();
					return false;

					}
		else if ((BONUS_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
							(BONUS_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
						_currentPoints= _currentPoints+4;
						updatePoints();

							return false;

						}
		else if ((BONUSHEALTH_COLLISION_BITMASK == b->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == a->getCollisionBitmask()) ||
								(BONUSHEALTH_COLLISION_BITMASK == a->getCollisionBitmask() && PROTAG_COLLISION_BITMASK == b->getCollisionBitmask())){
							_currentHealth= _currentHealth+1;
							updateHealth();
								return false;

							}

		 else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 			(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask())){

		 			return false;

		 		}
		 else if((PROTAG_COLLISION_BITMASK == b->getCollisionBitmask() && EDGE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 				(PROTAG_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask())){
		 			return true;

		 		}
		 else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && GROUND_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 				(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && GROUND_COLLISION_BITMASK == b->getCollisionBitmask())){

		 				return false;

		 			}
		 	else if ((CONTROLLER_COLLISION_BITMASK == b->getCollisionBitmask() && EDGE_COLLISION_BITMASK == a->getCollisionBitmask()) ||
		 					(CONTROLLER_COLLISION_BITMASK == a->getCollisionBitmask() && EDGE_COLLISION_BITMASK == b->getCollisionBitmask())){

		 					return false;

		 				}
	}

	return true;
}


void TagGame::update(float dt)
{
	if (!audio->isBackgroundMusicPlaying()){
			    		audio->resumeBackgroundMusic();
		}
	if(world1!=NULL || world3 != NULL){


		if (true == isTouchDown)
		{
			if (initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05)
					{
						CCLOG("SWIPED LEFT");
						//auto action=MoveBy::create(1.3,Vec2(-300, 0));
						switch(_world){
							case 1:
								world1->dashLeft();
							break;
							case 3:
								world3->dashLeft();
							break;
						}
						//world1->dashLeft();
						auto fadeskull = FadeOut::create(1);
						skull->runAction(fadeskull);
						Action* action;
						auto fade = FadeOut::create(1);
						_health->runAction(fade);


						/*if(protag->getPosition().x-250 < protag->getContentSize().width/2){
							 action=MoveTo::create(0.7,Vec2(origin.x + protag->getContentSize().width/2, protag->getPosition().y));
							protag->runAction(action);

						}
						else{
							action=MoveTo::create(0.7,Vec2(protag->getPosition().x-250, protag->getPosition().y));
							protag->runAction(action);
						}
*/
						isTouchDown = false;

					}
			  else if (initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05)
					{
						CCLOG("SWIPED RIGHT");

						auto fadeskull = FadeOut::create(1);
						skull->runAction(fadeskull);

						auto fade = FadeOut::create(1);
						 _health->runAction(fade);
						//auto action=MoveBy::create(1.3,Vec2(protag->getPosition().x-300, 0));
						 //world1->dashRight();
						 switch(_world){
						 							case 1:
						 								world1->dashRight();
						 							break;
						 							case 3:
						 								world3->dashRight();
						 							break;
						 						}
						/*if(protag->getPosition().x+200 > visibleSize.width - protag->getContentSize().width/2){
								auto action=MoveTo::create(0.7,Vec2(visibleSize.width - protag->getContentSize().width/2, protag->getPosition().y));
								protag->runAction(action);
						}
						else{
								auto action=MoveTo::create(0.7,Vec2(protag->getPosition().x+200, protag->getPosition().y));
								protag->runAction(action);
						}
*/
						isTouchDown = false;
					}
			  else if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05)
			{
				CCLOG("SWIPED DOWN");

				isTouchDown = false;

			}
			else if (initialTouchPos[1] - currentTouchPos[1] < - visibleSize.width * 0.05 && _jumpNumber <2)
			{
				CCLOG("SWIPED UP");
				isTouchDown = false;
				auto fadeskull = FadeOut::create(1);
				skull->runAction(fadeskull);

				auto fade = FadeOut::create(1);
				_health->runAction(fade);
				jump();
				//protag->isTouchingGround() == true
				//protag->Jump();

			}
		}
	}
	else if(world2!=NULL){
		world2->Fall();
	}


}

void TagGame::Float(){
	 if ( true == isFloating )
	    {
	        protag->setPositionX( visibleSize.width / 2 + origin.x );
	        protag->setPositionY( protag->getPositionY() - ( PROTAG_FALLING_SPEED * visibleSize.height ) );
	    }
	    else
	    {
	        protag->setPositionX( visibleSize.width / 2 + origin.x );
	        protag->setPositionY( protag->getPositionY() + ( PROTAG_FALLING_SPEED * visibleSize.height ) );
	    }
}

void TagGame::stopSwimming(float dt){
	world2->stopSwimming();
}

bool TagGame::onTouchBegan(Touch *touch, Event *event)
{
	if(world2!=NULL){
		auto fade = FadeOut::create(1);
		_health->runAction(fade);
		auto fadeskull = FadeOut::create(1);
		skull->runAction(fadeskull);
		world2->swim();
		this->scheduleOnce( schedule_selector( TagGame::stopSwimming ), PROTAG_SWIM_DURATION );

	}
	else{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    isTouchDown = true;
	}
    return true;
}

void TagGame::onTouchMoved(Touch *touch, Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
}

void TagGame::onTouchEnded(Touch *touch, Event *event)
{
    isTouchDown = false;
}

void TagGame::onTouchCancelled(Touch *touch, Event *event)
{
    onTouchEnded(touch, event);
}

void TagGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
