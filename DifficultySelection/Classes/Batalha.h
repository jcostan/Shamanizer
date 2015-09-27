#pragma once

#include "cocos2d.h"

#include "ui/CocosGUI.h"

using namespace cocos2d;

class Batalha : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a event callback    
	void onTouchesBegan(const std::vector<Touch *> &touches, Event *event);
	void onTouchesMoved(const std::vector<Touch *> &touches, Event *event);
	void onTouchesEnded(const std::vector<Touch *> &touches, Event *event);

	void menuCloseCallback(cocos2d::Ref* pSender);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);

	void update(float) override;
    void configMagicSprites();
    void configUI();
	void SpawnEnemies();
	void configBrush();

	// implement the "static create()" method manually
	CREATE_FUNC(Batalha);

private:
	const static int MAX_TOUCHES = 5;
	float spawnTimer = 2;
	void ChangeScene(int id);
	Scene *VilaScene;
	Scene *BatalhaScene;
	void MoveCameraTo(float dt);
	Vec2 movePosition;
	bool isChangingScene = false;
	int cenaAtual = 0;//0 - Batalha, 1 - Vila
    int initialPosiTouch[2];
    int currentPosiTouch[2];
    bool isTouchDown = false;
	void ConfigOcas();
	ui::Button* ocas[4];
    //Node* rootNode;

protected:
	Label* labelTouchLocations[MAX_TOUCHES];

	Sprite *player;
	Sprite *shadow;
	Sprite *bgInGame;
	Sprite *brush;
    Sprite *fundoRolagem;
    Sprite *fogo;
    Sprite *magia1;

	Vector<Sprite*> shadows;

	Vector<Sprite*> brushs;

	DrawNode *nodeMagic;

	bool castando = false;

	int pontoAtual = 0;

	RenderTexture *target;
};

