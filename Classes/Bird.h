#ifndef __BIRD_H__
#define __BIRD_H__

#include "cocos2d.h"
#include <iostream>

class Bird
{
public:
    //constructor
    Bird( cocos2d::Layer *layer );
    
    //destructor
    ~Bird(){ std::cout << "Bird destructor" << std::endl; };
    
    void Fall();
    void Fly(){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( "Sounds/Wing.mp3" );
        isFalling = false;
    };
    void StopFlying(){ isFalling = true; };
    
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *flappyBird;
    
    bool isFalling;
};

#endif // __BIRD_H__
