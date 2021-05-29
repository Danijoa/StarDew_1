#pragma once
#include "GameNode.h"

class Image;
class LoadingScene : public GameNode
{
private:
	Image* loadImage;

	Image* sleepFont;
	Image* timeoutFont;
	Image* energyoutFont;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~LoadingScene() {};
};

