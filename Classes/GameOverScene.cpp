#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;

unsigned int score;

Scene* GameOverScene::createScene( unsigned int tempscore )
{
    score = tempscore;
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
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
    
    const char *retryButtonImages[4] = {
        "iphone/Retry Button.png",
        "iphone/Retry Button Clicked.png",
        "iphonehd/Retry Button.png",
        "iphonehd/Retry Button Clicked.png",
    };
    
    const char *menuButtonImages[4] = {
        "iphone/Menu Button.png",
        "iphone/Menu Button Clicked.png",
        "iphonehd/Menu Button.png",
        "iphonehd/Menu Button Clicked.png",
    };
    
    //memo::
    //Background
    auto backgroundSprite = Sprite::create( backgroundImages[1] );
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    this->addChild( backgroundSprite );
    
    /**
     * create retry button
     */
    auto retryItem = MenuItemImage::create(
                                           retryButtonImages[2],
                                           retryButtonImages[3],
                                           CC_CALLBACK_1(GameOverScene::GoToGameScene, this)
                                           );

    auto mainMenuItem = MenuItemImage::create(
                                           menuButtonImages[2],
                                           menuButtonImages[3],
                                           CC_CALLBACK_1(GameOverScene::GoToMainMenuScene, this)
                                           );
    
    retryItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 4 * 3) );
    mainMenuItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 4 ) );
  
    auto menu = Menu::create(retryItem, mainMenuItem, NULL);
    menu->setPosition( Point::ZERO );
    
    this->addChild( menu );
    
    /**
     * save user score
     */
    
    UserDefault *def = UserDefault::getInstance();
    
    auto hightScore  = def->getIntegerForKey( "HIGHTSCORE", 0 );
    
    if ( score > hightScore ) {
        hightScore = score;
        def->setIntegerForKey( "HIGHTSCORE" , hightScore );
    }
    
    def->flush();
    
    std::cout << hightScore << std::endl;
    
    __String* tempHightScore = __String::createWithFormat( "HIGHT SCORE : %i", hightScore );
    hightScoreLabel = Label::createWithTTF( tempHightScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
    hightScoreLabel->setColor( Color3B::YELLOW );
    hightScoreLabel->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + 50 ) );
    this->addChild( hightScoreLabel );
    
    
    /**
     * craete score
     */
    __String* tempscore = __String::createWithFormat( "SCORE : %i", score );
    currentScoreLabel = Label::createWithTTF( tempscore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    currentScoreLabel->setColor( Color3B::WHITE );
    currentScoreLabel->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    
    this->addChild( currentScoreLabel );
    
    
    
    return true;
}


void GameOverScene::GoToMainMenuScene( cocos2d::Ref *sender )
{

    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create(TRANSITION_TIME, scene) );
    

}

void GameOverScene::GoToGameScene( cocos2d::Ref *sender )
{
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create(TRANSITION_TIME, scene) );
    
}