#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Batalha.h"

USING_NS_CC;


using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("CenaPrincipal.csb");
    
    addChild(rootNode);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bt = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ProjectNode_1")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("BtnAltar_4"));
    
    auto bt1 = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ProjectNode_1")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("BtnLvl_6"));
    
    auto bt2 = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ProjectNode_1")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("BtnPVP_8"));
    
    auto bt3 = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ProjectNode_1")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("BtnShop_10"));
    
    auto bt4 = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ProjectNode_1")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("BtnFrd_12"));
    
    auto bt5 = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ProjectNode_1")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("BtnSetting_14"));
    
    if (bt) {
        
        bt->addClickEventListener([=] (Ref *) {
            
            CCLOG("Pressed ");
            auto scene = Batalha::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.7, scene, Color3B(0,0,0)));
        });
    }
    
    return true;
}
