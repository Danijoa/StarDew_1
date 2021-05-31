#pragma once
#include "config.h"
#include "Singleton.h"

class Image;
class DataManager : public Singleton<DataManager>
{
private:
	// 그 전 씬 저장
	int preScene;

	// 농장 타일의 정보
	static TILE_INFO farmTileInfo[FARM_TILE_X * FARM_TILE_Y];

	/* 확인용 */
	char szText[128] = "";

	Image* curBackBuffer;

public:
	HRESULT Init();
	void Release();
	void Update();

	void testDry();
	
	// 씬
	void SetPreScene(int pre) { this->preScene = pre; }
	int GetPreScene() { return this->preScene; }

	// 농장 씬 로드
	void loadFarmScene(int sceneNum);
	TILE_INFO* GetFarmScene() { return farmTileInfo; }

	// 현재 백버퍼에 그려진 이미지
	inline void SetCurBackBuffer(Image* backBuffer) { this->curBackBuffer = backBuffer; }
	inline Image* GetCurBackBuffer() { return this->curBackBuffer; }
};

