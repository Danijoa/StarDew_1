#include "TimerManager.h"
#include "Timer.h"

HRESULT TimerManager::Init()
{
    timer = new Timer();
    timer->Init();
    gameSecond = 0;

    return S_OK;
}

void TimerManager::Release()
{
    SAFE_DELETE(timer);
}

void TimerManager::Update()
{
    if (timer)
    {
        timer->Tick();
        gameSecond = timer->getGameSecond();
    }
}

void TimerManager::Render(HDC hdc)
{
    if (timer)
    {
        //wsprintf(szText, "FPS : %d", timer->GetFPS());
        //TextOut(hdc, 0, 0, szText, strlen(szText));

        //wsprintf(szText, "�ð� ��� : %d", gameSecond);
        //TextOut(hdc, WINSIZE_X - 150, 195, szText, strlen(szText));
    }
}

float TimerManager::GetElapsedTime()
{
    return timer->GetElapsedTime();
}

float TimerManager::GetCurrTime()
{
    return timer->GetCurrTime();
}
