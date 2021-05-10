#include "StartScene.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Button.h"

HRESULT StartScene::Init()
{
    // ũ�� ����
    SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // ��� �̹���
    ImageManager::GetSingleton()->AddImage("���۹��", "Image/startBG.bmp", WINSIZE_X, WINSIZE_Y);
    startBackGround = ImageManager::GetSingleton()->FindImage("���۹��");

    // Ÿ��Ʋ �̹���
    ImageManager::GetSingleton()->AddImage("Ÿ��Ʋ", "Image/Tile.bmp", 400*2, 220*2, true, RGB(255, 255, 255));
    titleImg = ImageManager::GetSingleton()->FindImage("Ÿ��Ʋ");

    // ��ư �̹���
    ImageManager::GetSingleton()->AddImage("new��ư", "Image/NewButton.bmp", 282*1.5, 102*1.5, 2, 1, true, RGB(255, 255, 255));
    btnNew = new Button();
    btnNew->Init("new��ư", WINSIZE_X * 0.2 + 30, WINSIZE_Y - 200);   //300
    btnNew->SetFunc(ChangeToHouseScene);

    ImageManager::GetSingleton()->AddImage("load��ư", "Image/LoadButton.bmp", 282*1.5, 102*1.5, 2, 1, true, RGB(255, 255, 255));
    btnLoad = new Button();
    btnLoad->Init("load��ư", WINSIZE_X * 0.4 + 30, WINSIZE_Y - 200);

    ImageManager::GetSingleton()->AddImage("exit��ư", "Image/ExitButton.bmp", 282*1.5, 102*1.5, 2, 1, true, RGB(255, 255, 255));
    btnExit = new Button();
    btnExit->Init("exit��ư", WINSIZE_X * 0.6 + 30, WINSIZE_Y - 200);

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
