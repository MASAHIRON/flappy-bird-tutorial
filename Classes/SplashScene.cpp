#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    /**
     * audio file preloader
     */
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Sounds/Hit.mp3" );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Sounds/Point.mp3" );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Sounds/Wing.mp3" );
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
   
    const char *backgroundFileNames[] = {
        "iphonehd/Splash Screen.png",
        "iphone/Splash Screen.png"
    };
    
    //memo::
    //scheduleOnece 一定のタイミングで処理を行う, setTimeoutみたいなやつ
    //param1 schedule_selectorに実行関数を渡す
    //param2 delayを渡す
    //Root Node
    //DESPLAY_TIME_SPLASH_SCENEはdefine.hで定数として定義
    this->scheduleOnce( schedule_selector(SplashScene::GoToMainMenuScene), DESPLAY_TIME_SPLASH_SCENE);
    
    //memo::
    //背景画像の表示
    //Sprite Node
    //画像ファイル名の配列アドレス値を渡す
    //createで画像を生成 create(const char *filename)
    //Spriteのメンバ関数setPositionで座標を設定する(中央配置) Sprite->setPosition( const Point &position )
    //座標はPointオブジェクトのPointメソッドを使う。DataStrucures
    //第一引数に可視領域の横幅 / 2 に可視領域のx座標を足す
    //第二引数に可視領域の縦幅 / 2 に可視領域のy座標を足す
    //background用のspriteをaddChildで配置する Node->addChild(Node *child)
    auto backgroundSprite = Sprite::create(backgroundFileNames[0]);
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    this->addChild(backgroundSprite);
    
    return true;
}

void SplashScene::GoToMainMenuScene( float dt )
{
    //memo::
    //シーン移動
    
    //memo::
    //MainMenuSceneのsceneオブジェクトを生成
    auto scene = MainMenuScene::createScene();
    
    //memo::
    //replaceScene シーンの移動
    //param1 シーン移動のエフェクタを掛けられる replaceScene(Scene *scene)
    //Fade処理 TransitionFade
    //Root Director
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene) );
}