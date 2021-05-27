#pragma once
#include "config.h"
#include "Singleton.h"

class Image;
class DataManager : public Singleton<DataManager>
{
private:
	// ±◊ ¿¸ æ¿ ¿˙¿Â
	int preScene;

	// ≥Û¿Â ≈∏¿œ¿« ¡§∫∏
	static TILE_INFO farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

	/* »Æ¿ŒøÎ */
	char szText[128] = "";

public:
	HRESULT Init();
	void Release();
	
	// æ¿
	void SetPreScene(int pre) { this->preScene = pre; }
	int GetPreScene() { return this->preScene; }

	// ≥Û¿Â æ¿ ∑ŒµÂ
	void loadFarmScene(int sceneNum);
	TILE_INFO* GetFarmScene() { return farmTileInfo; }
};

