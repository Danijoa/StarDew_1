#pragma once
#include "config.h"
// 1초에 이동하는 거리를 저장해 두면
// 컴퓨터 마다 1초에 처리하는 프레임 수가 달라도
// 1초 후의 결과는 동일하게 나타난다
// 이를 계산하려면, 내가 사용하고 있는 컴퓨터의
// 정확한 시간을 구하는 것이 필요하다

// 1초에 10번 진동을 한다 
// 시간을 측정하는 이전지점 진동수가 3이고, 현재지점 진동수가 6일 때
// 총 3번 진동 하였다
// 시간을 구하기 위한 식을 세우면
// 1초:10번 = x초:3번

class Timer
{
private:
	bool isHardware;			// 고성능 타이머(cpu clock)를 쓸 수 있는지 여부
	float timeScale;			// 경과 진동수(현재 - 이전)를 초당 진동수로 나눠서 진동수를 시간으로 환산
	float timeElapsed;			// 마직막 시간과 현재 시간의 경과량
	__int64 currTime;			// 현재 시간(고성능 타이머 진동수)
	__int64 lastTime;			// 이전 시간(고성능 타이머 진동수)
	__int64 periodFrequency;	// 고성능 타이머 주파수(초당 진동수)

	float fpsTimeElapsed;			// timeElapsed 누적 -> 프레임 경과 시간(1초가 지났는지 확인 하기 위한 용도)
	unsigned long fpsFrameCount;	// 초당 프레임 수 확인용
	unsigned long FPS;				// 초당 프레임 수

	int gameSecond;

public:
	HRESULT Init();
	void Tick();
	float GetCurrTime();

	inline unsigned long GetFPS() { return this->FPS; }
	inline float GetElapsedTime() { return this->timeElapsed; }
	inline int getGameSecond() { return this->gameSecond; }
};

