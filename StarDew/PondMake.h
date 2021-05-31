#pragma once
#include "GameNode.h"

class PondMake : public GameNode
{
private:
	Image* pondSample;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~PondMake() {};
};

