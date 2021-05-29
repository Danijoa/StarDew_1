#include "LoadingScene.h"
#include "Image.h"
#include "InventoryManager.h"

HRESULT LoadingScene::Init()
{
	loadImage = ImageManager::GetSingleton()->AddImage("�ε�ȭ��", "Image/loading.bmp", WINSIZE_X, WINSIZE_Y);

	sleepFont = ImageManager::GetSingleton()->AddImage("������", "Image/sleepFont.bmp", 229, 101);
	timeoutFont = ImageManager::GetSingleton()->AddImage("Ÿ�Ӿƿ�", "Image/timeoutFont.bmp", 304, 92);
	energyoutFont = ImageManager::GetSingleton()->AddImage("�������ƿ�", "Image/energyoutFont.bmp", 314, 91);

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
	if (loadImage)	// �ε� ���
	{
		loadImage->Render(hdc);
	}

	if (InventoryManager::GetSingleton()->GetSleepDay() == true)	// ���� �����
	{
		sleepFont->Render(hdc, WINSIZE_X / 2 - sleepFont->GetWidth() / 2, WINSIZE_Y / 2 - sleepFont->GetHeight() / 2);
	}

	if (InventoryManager::GetSingleton()->GetTimeoutDay() == true)	// �ð��� �� �Ǿ���
	{
		timeoutFont->Render(hdc, WINSIZE_X / 2 - timeoutFont->GetWidth() / 2, WINSIZE_Y / 2 - timeoutFont->GetHeight() / 2);
	}

	if (InventoryManager::GetSingleton()->GetEnergyoutDay() == true)	// ������ ����
	{
		energyoutFont->Render(hdc, WINSIZE_X / 2 - energyoutFont->GetWidth() / 2, WINSIZE_Y / 2 - energyoutFont->GetHeight() / 2);
	}
}
 