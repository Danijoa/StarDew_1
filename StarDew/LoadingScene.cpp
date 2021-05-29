#include "LoadingScene.h"
#include "Image.h"
#include "InventoryManager.h"

HRESULT LoadingScene::Init()
{
	loadImage = ImageManager::GetSingleton()->AddImage("로딩화면", "Image/loading.bmp", WINSIZE_X, WINSIZE_Y);

	sleepFont = ImageManager::GetSingleton()->AddImage("잠들었다", "Image/sleepFont.bmp", 229, 101);
	timeoutFont = ImageManager::GetSingleton()->AddImage("타임아웃", "Image/timeoutFont.bmp", 304, 92);
	energyoutFont = ImageManager::GetSingleton()->AddImage("에너지아웃", "Image/energyoutFont.bmp", 314, 91);

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
	if (loadImage)	// 로딩 배경
	{
		loadImage->Render(hdc);
	}

	if (InventoryManager::GetSingleton()->GetSleepDay() == true)	// 잠이 들었다
	{
		sleepFont->Render(hdc, WINSIZE_X / 2 - sleepFont->GetWidth() / 2, WINSIZE_Y / 2 - sleepFont->GetHeight() / 2);
	}

	if (InventoryManager::GetSingleton()->GetTimeoutDay() == true)	// 시간이 다 되었다
	{
		timeoutFont->Render(hdc, WINSIZE_X / 2 - timeoutFont->GetWidth() / 2, WINSIZE_Y / 2 - timeoutFont->GetHeight() / 2);
	}

	if (InventoryManager::GetSingleton()->GetEnergyoutDay() == true)	// 에너지 부족
	{
		energyoutFont->Render(hdc, WINSIZE_X / 2 - energyoutFont->GetWidth() / 2, WINSIZE_Y / 2 - energyoutFont->GetHeight() / 2);
	}
}
 