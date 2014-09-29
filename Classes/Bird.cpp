#include "Bird.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;

Bird::Bird( cocos2d::Layer *layer )
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin      = Director::getInstance()->getVisibleOrigin();
    
    const char *ballImages[2] = {
        "iphone/Ball.png",
        "iphonehd/Ball.png",
    };
    
    flappyBird = Sprite::create(ballImages[1]);
    flappyBird->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    
    auto flappyBody = PhysicsBody::createCircle( flappyBird->getContentSize().width / 2 );
    flappyBird->setPhysicsBody( flappyBody );
    
    /**
     * memo
     *
     * PhysicsBody Class
     * setCollisionBitmask( int bitmask )
     * 衝突判定の理論値をセットする
     * setConstactTestBitmask( int bitmask )
     * 接触判定の理論値をセットする
     */
    flappyBody->setContactTestBitmask( true );
    flappyBody->setCollisionBitmask( BIRD_COLLISION_BITMASK );
    
    /**
     * localZOrder
     */
    layer->addChild(flappyBird, 100);
    
    /**
     * 初期化
     */
    isFalling = true;
};

void Bird::Fall()
{

    if( isFalling == true ){
        flappyBird->setPositionX( visibleSize.width / 2 + origin.x );
        flappyBird->setPositionY( flappyBird->getPositionY() - ( BIRD_FALLING_SPEED * visibleSize.height ) );
    }else{
    
        flappyBird->setPositionX( visibleSize.width / 2 + origin.x );
        flappyBird->setPositionY( flappyBird->getPositionY() + ( BIRD_FALLING_SPEED * visibleSize.height ) );
    
    }
    
};
