#include "StartScene.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT StartScene::Init()
{
    // 크기 조정
    SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // 배경 이미지
    ImageManager::GetSingleton()->AddImage("시작배경", "Image/startBG.bmp", WINSIZE_X, WINSIZE_Y);
    startBackGround = ImageManager::GetSingleton()->FindImage("시작배경");

    // 버튼 이미지

    return S_OK;
}

void StartScene::Release()
{
}

void StartScene::Update()
{
}

void StartScene::Render(HDC hdc)
{
    if (startBackGround)
    {
        startBackGround->Render(hdc, 0, 0);
    }
}
