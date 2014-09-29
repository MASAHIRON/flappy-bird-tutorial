#include "GameScene.h"
#include "GameOverScene.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;


Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    //memo::
    //createWithPhysics
    //sceneでPhysicsを使用できるようにする
    //getPhysicsWorldでPhysicsWorldクラスのポインタを返す
    //PhysicsWorld::setDebugDrawMask(int mask)
    auto scene = Scene::createWithPhysics();
    PhysicsWorld * world = scene->getPhysicsWorld();
    scene->getPhysicsWorld()->setGravity( Vect(0, 0) );
    
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    /**
     * memo:
     * setGravity
     * Vect is a typedef Point
     * Vect(float x, float y)
     */
    
    // 'layer' is an autorelease object
    //memo::
    auto layer = GameScene::create();
    layer->SetPhysicsWorld( world );

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
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
    
    //memo::
    //Background
    auto backgroundSprite = Sprite::create(backgroundImages[1]);
    backgroundSprite->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    this->addChild(backgroundSprite);

    //memo::
    //PhysicsBodyクラスから短形シェイプの構造体を生成する
    //const size &size, constPhysicsMaterial PHYSICSBODY_MATERIAL_DEFAULT, float border., const Point &point
    //body->addShape(PhysicsShapeEdgeBox::create(size, material, border, offset));
    //PhysicsMaterialは(密度, 反発係数, 摩擦係数)
    //edgeNodeに物理構造体を与える setPhysicsBody(<#cocos2d::PhysicsBody *body#>)
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 2);
    edgeBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
    edgeBody->setContactTestBitmask( true );
    
    auto edgeNode = Node::create();
    
    edgeNode->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    edgeNode->setPhysicsBody(edgeBody);
    
    this->addChild(edgeNode);
    
    this->schedule( schedule_selector(GameScene::SpawnPipe), PIPE_SPAWN_FREQUENCY * visibleSize.width );
    
    /**
     * memo::
     *
     * new Type( expression );
     * 動的メモリアロケータ
     */
    bird = new Bird( this );
    
    EventDispatcher *_eventDispatch = Director::getInstance()->getEventDispatcher();
    
    
    /**
     * memo::
     *
     * std::function<bool (PhysicsContact &) > onContactBegin
     * 引数の値が合っていないとNo viable overloaded '='と表示される
     * 衝突判定のイベントを登録
     * onContactBegin(衝突前)のcallbackを設定
     * getEventDispatcherでSecene NodeにcontactListenerEventを設定する
     */
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( GameScene::onContactBegin, this );
    _eventDispatch->addEventListenerWithSceneGraphPriority( contactListener, this );
    
    
    /**
     * memo::
     * std::function<bool (Touch *, Event*)> onTouchBegan
     */
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( GameScene::onTouchBegan, this);
    _eventDispatch->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    /**
     * score initialize
     */
    score = 0;
    __String* tempscore = __String::createWithFormat( "%i", score );
    
    scoreLable = Label::createWithTTF( tempscore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    scoreLable->setColor( Color3B::WHITE );
    scoreLable->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height * 0.75 + origin.y ) );
    
    this->addChild( scoreLable );
    
    
    /**
     * memo::
     */
    this->scheduleUpdate();
    
    
//    delete bird;
    
    return true;
}

void GameScene::SpawnPipe( float dt )
{
    pipe.SpawnPipe( this );
};

bool GameScene::onContactBegin( cocos2d::PhysicsContact &contact )
{
    /**
     * memo::
     * 衝突しあったphysicsBodyを取得する
     * getShapeA
     * return PhysicsShape pointer
     * getBody
     * return PhysicsBody pointer
     */
    
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    if( ( BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) ||
        ( BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( "Sounds/Hit.mp3" );
        CCLOG( "SCORE %i", score );
        auto scene = GameOverScene::createScene( score );
        
        Director::getInstance()->replaceScene( TransitionFade::create(TRANSITION_TIME, scene) );
    }
    else if( ( BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && POINT_COLLISION_BITMASK == b->getCollisionBitmask() ) ||
             ( BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && POINT_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( "Sounds/Point.mp3" );
        
        score++;
        __String* tempscore = __String::createWithFormat( "%i", score );
        scoreLable->setString( tempscore->getCString() );
    
    }
    return true;
};

bool GameScene::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{

    /**
     * memo::
     */
    bird->Fly();
    this->scheduleOnce( schedule_selector( GameScene::StopFlying ), BIRD_FRY_DURATION );
    
    return true;

};


void GameScene::StopFlying( float dt )
{
    bird->StopFlying();
};

void GameScene::update( float dt )
{
    std::cout << 'fall' << std::endl;
    bird->Fall();
};