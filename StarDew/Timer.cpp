#include "Timer.h"

HRESULT Timer::Init()
{
    isHardware = false;
    timeScale = 0.0f;
    timeElapsed = 0.0f;
    currTime = 0.0;
    lastTime = 0.0;
    periodFrequency = 0.0;

    fpsTimeElapsed = 0.0f;
    fpsFrameCount = 0;
    gameSecond = 0;
    if (QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency))	// cpu 초당 진동수를 받아 올 수 있는 경우
    {
        isHardware = true;
        QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);				// 경과 진동수
        timeScale = 1.0f / periodFrequency;								// (1초 / cpu 초당 진동수) (1/10 같은..)
    }
    else
    {
        isHardware = false;
        lastTime = timeGetTime();
        timeScale = 0.001f;
    }

    return S_OK;
}

void Timer::Tick()
{
    // 현재 진동수 or 현재 시간을 구해서 경과 시간을 계산
    if (isHardware)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    }
    else
    {
        currTime = timeGetTime();
    }

    timeElapsed = (currTime - lastTime) * timeScale;	//경과 시간을 계산 ((6-3)*(1/10) 같은..)

    //프레임 계산
    fpsTimeElapsed += timeElapsed;
    fpsFrameCount++;

    if (fpsTimeElapsed >= 1.0f)
    {
        gameSecond += 1;
        FPS = fpsFrameCount;
        fpsFrameCount = 0.0f;
        fpsTimeElapsed -= 1.0f;
    }

    lastTime = currTime;								//timeElapsed가 계속 변하는 것을 추적
}

float Timer::GetCurrTime()
{
    __int64 time;
    if (isHardware)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&time);
    }
    else
    {
        time = timeGetTime();
    }

    return time / timeScale;
}
