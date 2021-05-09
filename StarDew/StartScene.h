#pragma once
#include "GameNode.h"

class Image;
class StartScene : public GameNode
{
private:
	Image* startBackGround;

	//Button* new
	//Button* load
	//Button* exit

public:
	virtual HRESULT Init();	
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~StartScene() {};
};

