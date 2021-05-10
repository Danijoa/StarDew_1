#include "StartScene.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Button.h"

HRESULT StartScene::Init()
{
    // 크기 조정
    SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // 배경 이미지
    ImageManager::GetSingleton()->AddImage("시작배경", "Image/startBG.bmp", WINSIZE_X, WINSIZE_Y);
    startBackGround = ImageManager::GetSingleton()->FindImage("시작배경");

    // 타이틀 이미지
    ImageManager::GetSingleton()->AddImage("타이틀", "Image/Tile.bmp", 400*2, 220*2, true, RGB(255, 255, 255));
    titleImg = ImageManager::GetSingleton()->FindImage("타이틀");

    // 버튼 이미지
    ImageManager::GetSingleton()->AddImage("new버튼", "Image/NewButton.bmp", 282*1.5, 102*1.5, 2, 1, true, RGB(255, 255, 255));
    btnNew = new Button();
    btnNew->Init("new버튼", WINSIZE_X * 0.2 + 30, WINSIZE_Y - 200);   //300
    btnNew->SetFunc(ChangeToHouseScene);

    ImageManager::GetSingleton()->AddImage("load버튼", "Image/LoadButton.bmp", 282*1.5, 102*1.5, 2, 1, true, RGB(255, 255, 255));
    btnLoad = new Button();
    btnLoad->Init("load버튼", WINSIZE_X * 0.4 + 30, WINSIZE_Y - 200);

    ImageManager::GetSingleton()->AddImage("exit버튼", "Image/ExitButton.bmp", 282*1.5, 102*1.5, 2, 1, true, RGB(255, 255, 255));
    btnExit = new Button();
    btnExit->Init("exit버튼", WINSIZE_X * 0.6 + 30, WINSIZE_Y - 200);

    return S_OK;
}

void StartScene::Release()
{
    SAFE_RELEASE(btnNew);
    SAFE_RELEASE(btnLoad);
    SAFE_RELEASE(btnExit);
}

void StartScene::Update()
{
    if (btnNew)
    {
        btnNew->Update();
    }
    if (btnLoad)
    {
        btnLoad->Update();
    }
    if (btnExit)
    {
        btnExit->Update();
    }
}

void StartScene::Render(HDC hdc)
{
    if (startBackGround)
    {
        startBackGround->Render(hdc, 0, 0);
    }

    if (titleImg)
    {
        titleImg->Render(hdc, WINSIZE_X*0.5 - titleImg->GetWidth()/2, 70);
    }

    if (btnNew)
    {
        btnNew->Render(hdc);
    }
    if (btnLoad)
    {
        btnLoad->Render(hdc);
    }
    if (btnExit)
    {
        btnExit->Render(hdc);
    }
}
