#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	char szText[128];
	int gameSecond;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	inline int GetGameSecond() { return this->gameSecond; }

	float GetElapsedTime();
	float GetCurrTime();
};

