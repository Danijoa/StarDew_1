#include "StartScene.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT StartScene::Init()
{
    // ũ�� ����
    SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    // ��� �̹���
    ImageManager::GetSingleton()->AddImage("���۹��", "Image/startBG.bmp", WINSIZE_X, WINSIZE_Y);
    startBackGround = ImageManager::GetSingleton()->FindImage("���۹��");

    // ��ư �̹���

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
