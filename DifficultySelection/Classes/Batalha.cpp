#include "Batalha.h"
#include "HelloWorldScene.h"
#include "Vila.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;


Scene* Batalha::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Batalha::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Batalha::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    rootNode->setName("rootNode");
    
    addChild(rootNode);   
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	//--------------------------CONFIGURACAO DAS OCAS
	ConfigOcas();
	//--------------------------CONFIGURACAO DA VIDA
	ConfigVida();


    auto totemAzul = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ScrollView_1")->getChildByName("BtnsBatalha")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("TotemAzul"));
	auto magiaFogo = static_cast< cocos2d::ui::Button*>(rootNode->getChildByName("ScrollView_1")->getChildByName("BtnsBatalha")->getChildByName("BottomBar")->getChildByName("Pc_Bg_4")->getChildByName("MagiaFogo"));
    
	if (magiaFogo)
	{
		magiaFogo->addClickEventListener([=](Ref *){
			configMagicSprites();
		});
	}

    if (totemAzul)
    {
        totemAzul->addClickEventListener([=](Ref *){
			auto tAzul = Sprite::create("totemAzul.png");
			tAzul->setPosition(Vec2(visibleSize.width / 0.7f, visibleSize.height / 2 + origin.y));
			tAzul->setScale(0.5,0.5);
            // add the label as a child to this layer
			this->getChildByName("rootNode")->getChildByName("ScrollView_1")->addChild(tAzul, 1);
        });
    }
    
    // add "HelloWorld" splash screen"
    player = Sprite::create("shaman_dude.png");
    shadow = Sprite::create("shadow.png");
    
    // position the sprite on the center of the screen
    player->setPosition(Vec2(visibleSize.width / 2.5 + origin.x, visibleSize.height / 2 + origin.y));
    shadow->setPosition(Vec2(visibleSize.width * 1.5 + origin.x, visibleSize.height / 2 + origin.y));
    
    player->setScale(visibleSize.width*.0005, visibleSize.height*.00065);
    shadow->setScale(visibleSize.width*.0004, visibleSize.height*.0005);
    
    nodeMagic = DrawNode::create();
    nodeMagic->setName("magicbrush");
    
    // add the sprite as a child to this layer
    
    rootNode->getChildByName("ScrollView_1")->addChild(player, 1);
    rootNode->getChildByName("ScrollView_1")->addChild(shadow, 2);
    
    this->scheduleUpdate();
    return true;
}

void Batalha::configBrush()
{
	target = RenderTexture::create(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height, Texture2D::PixelFormat::RGBA8888);
	target->retain();
	target->setPosition(Director::getInstance()->getVisibleSize() / 2);

	this->addChild(target);

	brush = Sprite::create("largeBrush.png");
	brush->setColor(Color3B::BLACK);
	brush->retain();

	target->beginWithClear(1, 1, 1, 1);
	target->end();
}

void Batalha::configMagicSprites()
{
    fogo = Sprite::create("magiaFogo.png");
    fogo->setPosition(Vec2(player->getPosition().x + player->getPosition().x / 2, player->getPosition().y));
    fogo->setScale(1.3);
    
    auto moveBy = MoveBy::create(1.5, Vec2(100, fogo->getPosition().y));
    
    fogo->runAction(moveBy);
    
	this->getChildByName("rootNode")->getChildByName("ScrollView_1")->addChild(fogo, 5);
}

void Batalha::configUI()
{
    magia1 = Sprite::create("btFire.png");
    magia1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/4, Director::getInstance()->getVisibleSize().height/8));
    magia1->setScale(.5);
    
    this->addChild(magia1, 4);
}

#pragma mark -
#pragma mark - Update
void Batalha::update(float delta){

	spawnTimer -= delta;

    if (true == isTouchDown)
    {
        if (initialPosiTouch[0] - currentPosiTouch[0] > Director::getInstance()->getVisibleSize().width * 0.05)
        {
            CCLOG("SWIPED LEFT");
            
            isTouchDown = false;
        }
        else if (initialPosiTouch[0] - currentPosiTouch[0] < - Director::getInstance()->getVisibleSize().width * 0.05)
        {
            CCLOG("SWIPED RIGHT");
            
            isTouchDown = false;
        }
        else if (initialPosiTouch[1] - currentPosiTouch[1] > Director::getInstance()->getVisibleSize().width * 0.05)
        {
            CCLOG("SWIPED DOWN");
            
            isTouchDown = false;
        }
        else if (initialPosiTouch[1] - currentPosiTouch[1] < - Director::getInstance()->getVisibleSize().width * 0.05)
        {
            CCLOG("SWIPED UP");
            
            isTouchDown = false;
        }
    }
    
	//if (JOGANDO)
	//{
		SpawnEnemies();
		DiminuirVida(delta);
		int count = 0;
		//-------------------------------------MOVER SOMBRAS------------------
		for (int i = 0; i < shadows.size(); i++) {
			auto position = shadows.at(i)->getPosition();

			if (position.x < Director::getInstance()->getVisibleSize().width*0.8) {
				//Ele saiu da tela, deve ir pra outra(e dominar uma casinha)
				//A linha abaixo faz ele aparecer na direita da tela
				//position.x = this->getBoundingBox().getMaxX() + shadows.at(i)->getBoundingBox().size.width / 2;
				DominarOca(i);
			}
			else {//Ainda esta na tela, ir pra esquerda
				position.x -= 50 * delta;
				shadows.at(i)->setPosition(position);
			}

			count++;
		}
		//---------------------------------------FIM MOVER SOMBRAS--------------
	//}
}

void Batalha::DiminuirVida(float delta){
	if (malEspiriTotal > 0){
		vida -= delta*malEspiriTotal;
		if (vida < 0){
			vida = 0;
			auto scene = HelloWorld::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.7, scene, Color3B(0, 0, 0)));
		}
		vidaTxt->setText(std::to_string((int)vida)+"/100");
		vidaBar->setPercent(vida);
	}
}

void Batalha::DominarOca(int i){
	float fatorEscurecimento = 100.0f;
	int randOca = rand() % 4;
	estadOcas[randOca]++;
	malEspiriTotal++;
	Color3B corAtual = ocas[randOca]->getColor();
	corAtual.r = corAtual.r <= 55 ? 0 : corAtual.r - fatorEscurecimento;
	corAtual.g = corAtual.r;
	corAtual.b = corAtual.r;
	ocas[randOca]->setColor(corAtual);
	shadows.at(i)->removeFromParentAndCleanup(true);
	shadows.erase(i);
}

void Batalha::SpawnEnemies(){
	if (spawnTimer <= 0){
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		float randY = rand() % 100 + (-100);
		shadows.pushBack(Sprite::create("shadow.png"));
		shadows.at(shadows.size() - 1)->setPosition(Vec2(visibleSize.width * 1.5 + origin.x, visibleSize.height / 2 + randY));
		shadows.at(shadows.size() - 1)->setScale(visibleSize.width*.0004, visibleSize.height*.0005);
        this->getChildByName("rootNode")->getChildByName("ScrollView_1")->addChild(shadows.at(shadows.size() - 1), shadows.size() + 3);
		spawnTimer = 4;
	}
}

void Batalha::ConfigOcas(){
	//ocas = new cocos2d::ui::Button();
	estadOcas[0] = estadOcas[1] = estadOcas[2] = estadOcas[3] = 0;
	for (int i = 0; i < 4; i++){
		ocas[i] = static_cast<cocos2d::ui::Button*>(this->getChildByName("rootNode")->getChildByName("ScrollView_1")->getChildByName("Oca" + std::to_string(i + 1)));
		if (ocas[i])
		{
			ocas[i]->addClickEventListener([=](Ref *){
				if (estadOcas[i] > 0){
					Color3B corAtual = ocas[i]->getColor();
					corAtual.r = corAtual.r >= 200 ? 255 : corAtual.r + 100;
					corAtual.g = corAtual.r;
					corAtual.b = corAtual.r;
					ocas[i]->setColor(corAtual);

					estadOcas[i]--;
					malEspiriTotal--;
				}
			});
		}
	}
}

void Batalha::ConfigVida(){
	vidaTxt = static_cast< cocos2d::ui::Text*>(this->getChildByName("rootNode")->getChildByName("ScrollView_1")->getChildByName("ProjectNode_1")->getChildByName("TopBar")->getChildByName("PvpenergyPanel_22")->getChildByName("EnergyCount_16"));
	vidaBar = static_cast<cocos2d::ui::LoadingBar*>(this->getChildByName("rootNode")->getChildByName("ScrollView_1")->getChildByName("ProjectNode_1")->getChildByName("TopBar")->getChildByName("PvpenergyPanel_22")->getChildByName("Bar_Energy_4"));
	vidaTxt->setText(std::to_string((int)vida)+"/100");
}

#pragma mark -
#pragma mark - Events Key/Touch

void Batalha::onTouchesBegan(const std::vector<Touch *> &touches, Event *event)
{
   // if (touches[0]->getLocationInView().x > Director::getInstance()->getVisibleSize().width/2)
    //    ChangeScene(0);
    //else
     //   ChangeScene(1);
    
    auto touch = touches[0];
    auto point = touch->getLocationInView();
    
    initialPosiTouch[0] = point.x;
    initialPosiTouch[1] = point.y;
    
    currentPosiTouch[0] = point.x;
    currentPosiTouch[1] = point.y;
    
    auto btFireRect = Rect(magia1->getPosition().x, magia1->getPosition().y, magia1->getContentSize().width, magia1->getContentSize().height);
    
    if (btFireRect.containsPoint(point)) {
        configMagicSprites();
    }
}

void Batalha::onTouchesMoved(const std::vector<Touch *> &touches, Event *unused_event)
{
	auto touch = touches[0];
	auto start = touch->getLocationInView();
	auto end = touch->getPreviousLocation();

    currentPosiTouch[0] = touches[0]->getLocationInView().x;
    currentPosiTouch[1] = touches[0]->getLocationInView().y;
    
	// begin drawing to the render texture
	target->begin();

	// for extra points, we'll draw this smoothly from the last position and vary the sprite's
	// scale/rotation/offset
	float distance = start.getDistance(end);
	if (distance > 1)
	{
		int d = (int)distance;
		brushs.clear();
		for (int i = 0; i < d; ++i)
		{
			Sprite * sprite = Sprite::create("largeBrush.png");
			sprite->setColor(Color3B::RED);
			sprite->setOpacity(20);
			brushs.pushBack(sprite);
		}
		for (int i = 0; i < d; i++)
		{
			float difx = end.x - start.x;
			float dify = end.y - start.y;
			float delta = (float)i / distance;
			brushs.at(i)->setPosition(Vec2(start.x + (difx * delta), start.y + (dify * delta)));
			brushs.at(i)->setRotation(rand() % 360);
			float r = (float)(rand() % 50 / 50.f) + 0.25f;
			brushs.at(i)->setScale(r);
			/*_brush->setColor(Color3B(CCRANDOM_0_1() * 127 + 128, 255, 255));*/
			// Use CCRANDOM_0_1() will cause error when loading libtests.so on android, I don't know why.
			brushs.at(i)->setColor(Color3B(rand() % 127 + 128, 255, 255));
			// Call visit to draw the brush, don't call draw..
			brushs.at(i)->visit();
		}
	}

	// finish drawing and return context back to the screen
	target->end();
}

void Batalha::onTouchesEnded(const std::vector<Touch *> &touches, Event *event)
{
	auto touch = touches[0];
	auto start = touch->getLocation();
	auto end = touch->getPreviousLocation();
}

void Batalha::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		//player->setPosition(Vec2(player->getPositionX() + 2, player->getPositionY()));
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		//player->setPosition(Vec2(player->getPositionX() - 2, player->getPositionY()));
		break;
	}
}

void Batalha::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
	/*switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:

	player->setPosition(Vec2(player->getPositionX() - 2, player->getPositionY()));
	shadow->setPosition(Vec2(shadow->getPositionX() - 2, shadow->getPositionY()));

	break;

	default:
	break;
	}*/
}

#pragma mark -
#pragma mark FINISHING DE APLICATION

void Batalha::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}