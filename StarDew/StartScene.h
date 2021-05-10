#pragma once
#include "GameNode.h"

class Image;
class Button;
class StartScene : public GameNode
{
private:
	// 배경
	Image* startBackGround;

	// 타이틀
	Image* titleImg;

	// 버튼
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
		SceneManager::GetSingleton()->ChangeScene("하우스씬");
	}

	virtual ~StartScene() {};
};

