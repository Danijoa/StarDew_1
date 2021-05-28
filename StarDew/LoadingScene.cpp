#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	loadImage = ImageManager::GetSingleton()->AddImage("�ε�ȭ��", "Image/loading.bmp", WINSIZE_X, WINSIZE_Y);

	return S_OK; 
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
}

void LoadingScene::Render(HDC hdc)
{
	if (loadImage)
	{
		loadImage->Render(hdc);
	}
}
 