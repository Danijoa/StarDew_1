#pragma once
#include "config.h"
#include "Singleton.h"

class Image;
class DataManager : public Singleton<DataManager>
{
private:
	// �� �� �� ����
	int preScene;

	// ���� Ÿ���� ����
	static TILE_INFO farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

	/* Ȯ�ο� */
	char szText[128] = "";

public:
	HRESULT Init();
	void Release();
	
	// ��
	void SetPreScene(int pre) { this->preScene = pre; }
	int GetPreScene() { return this->preScene; }

	// ���� �� �ε�
	void loadFarmScene(int sceneNum);
	TILE_INFO* GetFarmScene() { return farmTileInfo; }
};

