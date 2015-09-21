#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

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
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto oca = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ScrollView_1")->getChildByName("Oca1"));
	auto vbutton = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ScrollView_1")->getChildByName("BtnsBatalha")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("TotemAzul"));
	if (vbutton)
	{
		vbutton->addClickEventListener([=](Ref *){

			CCLOG("Pressed ");
			auto label = Label::createWithTTF("Shamanizer: O ataque das sombras", "fonts/arial.ttf", 18);

			// position the label on the center of the screen
			label->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height - label->getContentSize().height));
			label->setColor(Color3B(255, 255, 255));

			// add the label as a child to this layer
			this->addChild(label, 1);
		});
	}

	if (oca)
	{
		oca->addClickEventListener([=](Ref *){
			auto playerLvl = static_cast< cocos2d::ui::Text*>(rootNode->getChildByName("ScrollView_1")->getChildByName("ProjectNode_1")->getChildByName("TopBar")->getChildByName("NamePanel_4")->getChildByName("Stars_5")->getChildByName("StarsCount_4"));
			playerLvl->setText("5");
		});
	}


    return true;
}
