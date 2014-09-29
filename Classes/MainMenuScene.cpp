#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    const char *backgroundImages[2] = {
        "iphone/Background.png",
        "iphonehd/Background.png",
    };
    
    const char *playButtonImages[4] = {
        "iphone/Play Button.png",
        "iphone/Play Button Clicked.png",
        "iphonehd/Play Button.png",
        "iphonehd/Play Button Clicked.png",
    };
    
    const char *titleImages[2] = {
        "iphone/Title.png",
        "iphonehd/Title.png"
    };
    
    
    //memo::
    //Background
    auto backgroundSprite = Sprite::create(backgroundImages[1]);
    backgroundSprite->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    this->addChild(backgroundSprite);
    
    //memo::
    //title
    //Node->getContentSize()
    //幅と高さを取得
    auto titleSprite = Sprite::create(titleImages[1]);
    titleSprite->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height) );
    this->addChild(titleSprite);
    
    
    //memo::
    //Button
    //MenuItemImage - MenuItemSprite - MenuItem - NodeRGBA - NODE
    //MenuItemImage::create(<#const std::string &normalImage#>, <#const std::string &selectedImage#>)
    //CC_CALLBACK_1
    auto playItem = MenuItemImage::create(
                                          playButtonImages[2],
                                          playButtonImages[3],
                                          CC_CALLBACK_1(MainMenuScene::GoToGameScene, this)
                                          );
    
    playItem->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    
    //memo::
    //Menu
    //Menu::create()
    //Point::ZERO
    auto menu = Menu::create( playItem, NULL );
    menu->setPosition( Point::ZERO );
    this->addChild( menu );
    return true;
}

void MainMenuScene::GoToGameScene( cocos2d::Ref *sender )
{
    //cocos2d::Ref
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create(TRANSITION_TIME, scene) );
}