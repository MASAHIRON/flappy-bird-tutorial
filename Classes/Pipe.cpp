#include "Pipe.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;

Pipe::Pipe()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin      = Director::getInstance()->getVisibleOrigin();

}

void Pipe::SpawnPipe( cocos2d::Layer *layer )
{
    log( "SPAWN PIPE" );
    
    const char *pipeImages[2] = {
        "iphone/Pipe.png",
        "iphonehd/Pipe.png",
    };
    
    const char *ballImages[2] = {
        "iphone/Ball.png",
        "iphonehd/Ball.png",
    };
    
    auto topPipe    = Sprite::create(pipeImages[1]);
    auto bottomPipe = Sprite::create(pipeImages[1]);
    auto ball       = Sprite::create(ballImages[1]);
    
    /*
     * memo::
     * PIPE用境界線無しの構造体を生成
     */
    auto topPipeBody    = PhysicsBody::createBox( (*topPipe).getContentSize() );
    auto bottomPipeBody = PhysicsBody::createBox( (*bottomPipe).getContentSize() );
    
    /*
     * memo::
     * PIPEの幅、高さを取得
     */
    float pipeContentWidth  = topPipe->getContentSize().width;
    float pipeContentHeight = topPipe->getContentSize().height;
    
    /**
     * memo::
     * Ballの幅、高さを取得
     */
    float ballContanteHeight = ball->getContentSize().height;
    
    /* memo::
     * Random マクロ
     * 0〜1までのランダムな数値（小数点）を小数点で返す
     * define ((float)rand()/RAND_MAX)
     * CCRANDOM_0_1()*100で0〜100までのランダムな数値を小数点で返す
     * www.programering.com/a/MDM2ITMwATk.html
     */
    auto random = CCRANDOM_0_1( );
    
    if( random < LOWER_SCREEN_PIPE_THRESHOLD )
    {
        random = LOWER_SCREEN_PIPE_THRESHOLD;
    }
    else if ( random > UPPER_SCREEN_PIPE_THRESHOLD )
    {
        random = UPPER_SCREEN_PIPE_THRESHOLD;
    }
    
    //memo::
    auto topPipePosition = ( ( random * visibleSize.height ) + ( pipeContentHeight / 2 ) );
    
    /*
     * memo::
     * PhysicsBody setDynamic( bool dynamic );
     * true 構造体に重力効果を付け
     * false 構造体に重力効果を付けない
     *
     * PhysicsBody setCollisionBitmask( int bitmask )
     * PhysicsBody setContactTestBitmask( int bitmask )
     */
    topPipeBody->setDynamic( false );
    topPipeBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
    topPipeBody->setContactTestBitmask( true );
    
    bottomPipeBody->setDynamic( false );
    bottomPipeBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
    bottomPipeBody->setContactTestBitmask( true );
    
    
    topPipe->setPhysicsBody( topPipeBody );
    bottomPipe->setPhysicsBody( bottomPipeBody );
    
    /* memo::
     * 上下のパイプのランダム配置
     * CCRANDOM_MINUS1_1
     * 1〜-1までのランダムな数値を小数点で返す
     */
    topPipe->setPosition( Point(
                                visibleSize.width + pipeContentWidth + origin.x + CCRANDOM_MINUS1_1() * 250,
                                topPipePosition
                                ) );
    
    bottomPipe->setPosition( Point(
                                topPipe->getPositionX(),
                                ( topPipePosition - ballContanteHeight * PIPE_GAP ) - pipeContentHeight
                                ) );
    layer->addChild( topPipe );
    layer->addChild( bottomPipe );
    
    /*
     * memo::
     * Action Class
     * MoveBy::create(<#float duration#>, <#const cocos2d::Vec2 &deltaPosition#>)
     * Spriteが配置されている座標を原点とした座標からをduration時間かけて指定したdeltaPositionまで移動する
     */
    auto topPipeAction    = MoveBy::create( PIPE_MOVEMENT_SPEED * visibleSize.width, Point( -visibleSize.width * 1.3, 0 ) );
    auto bottomPipeAction = MoveBy::create( PIPE_MOVEMENT_SPEED * visibleSize.width, Point( -visibleSize.width * 1.3, 0 ) );
    
    /*
     * memo::
     * runAction
     * アニメーション開始
     */
    topPipe->runAction( topPipeAction );
    bottomPipe->runAction( bottomPipeAction );
    
    /**
     * point
     */
    auto pointNode = Node::create();
    auto pointBody = PhysicsBody::createBox( Size(1, ballContanteHeight * PIPE_GAP ) );
    
    pointBody->setDynamic( false );
    pointBody->setCollisionBitmask( POINT_COLLISION_BITMASK );
    pointBody->setContactTestBitmask( true );
    
    pointNode->setPhysicsBody( pointBody );
    pointNode->setPosition(
                           topPipe->getPositionX(),
                           topPipe->getPositionY() - ( pipeContentHeight / 2 ) - ( ( ballContanteHeight * PIPE_GAP ) / 2 ) );
    
    layer->addChild( pointNode );
    
    auto pointNodeAction    = MoveBy::create( PIPE_MOVEMENT_SPEED * visibleSize.width, Point( -visibleSize.width * 1.3, 0 ) );
    pointNode->runAction( pointNodeAction );
};
