#pragma once
#include "GameNode.h"

class Image;
class Button;
class StartScene : public GameNode
{
private:
	// ���
	Image* startBackGround;

	// Ÿ��Ʋ
	Image* titleImg;

	// ��ư
	Button* btnNew;
	Button* btnLoad;
	Button* btnExit;

public:
	virtual HRESULT Init();	
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	static void ChangeToHouseScene()
	{
		SceneManager::GetSingleton()->ChangeScene("�Ͽ콺��");
	}

	virtual ~StartScene() {};
};

