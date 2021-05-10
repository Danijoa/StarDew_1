#pragma once
#include "GameNode.h"

enum class ButtonState
{
	NONE, STAY, DOWN, UP
};

typedef void (*func_t)();

class Image;
class Button : public GameNode
{
private:
	ButtonState state;
	Image* buttonImg;
	INT_PAIR pos;
	RECT rc;
	func_t func;

public:
	HRESULT Init(const char* fileName, int posX, int posY);
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetFunc(func_t func)
	{
		this->func = func;
	}

	virtual ~Button() {};
};

