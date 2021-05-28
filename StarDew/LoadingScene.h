#pragma once
#include "GameNode.h"

class Image;
class LoadingScene : public GameNode
{
private:
	Image* loadImage;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~LoadingScene() {};
};

