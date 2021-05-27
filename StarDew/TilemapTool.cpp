/*
#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"

TILE_INFO TilemapTool::tilesetTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];		// 타일시트
TILE_INFO TilemapTool::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];			// 집
TILE_INFO TilemapTool::storeTileInfo[STORE_TILE_X * STORE_TILE_Y];			// 상점
TILE_INFO TilemapTool::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];				// 마당
// 광산

HRESULT TilemapTool::Init()
{
	// 크기 조정
	SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

	// 샘플
	ImageManager::GetSingleton()->AddImage("storetile", "Image/Store.bmp", 1408, 896, STORE_TILE_X, STORE_TILE_Y);
	sample = ImageManager::GetSingleton()->FindImage("storetile");


	// 메인 공간 이미지 정보 저장
	for (int i = 0; i < STORE_TILE_Y; i++)	// 세로
	{
		for (int j = 0; j < STORE_TILE_X; j++)	//가로
		{
			// 몇번째 프레임에 있는 이미지를 들고와서
			storeTileInfo[i * STORE_TILE_X + j].frameX = j;
			storeTileInfo[i * STORE_TILE_X + j].frameY = i;
										   
			// 차례대로 저장해주기		  
			storeTileInfo[i * STORE_TILE_X + j].rcTile.left = TILESIZE * j;
			storeTileInfo[i * STORE_TILE_X + j].rcTile.top = TILESIZE * i;
			storeTileInfo[i * STORE_TILE_X + j].rcTile.right = storeTileInfo[i * STORE_TILE_X + j].rcTile.left + TILESIZE;
			storeTileInfo[i * STORE_TILE_X + j].rcTile.bottom = storeTileInfo[i * STORE_TILE_X + j].rcTile.top + TILESIZE;

			storeTileInfo[i * STORE_TILE_X + j].tileType = TileType::FLOOR;

			storeTileInfo[i * STORE_TILE_X + j].objFrameX = -1;
			storeTileInfo[i * STORE_TILE_X + j].objFrameY = -1;

			storeTileInfo[i * STORE_TILE_X + j].seedFrameX = -1;
			storeTileInfo[i * STORE_TILE_X + j].seedFrameY = -1;
			storeTileInfo[i * STORE_TILE_X + j].seedType = SeedType::NONE;
			storeTileInfo[i * STORE_TILE_X + j].day = -1;
		}
	}

	return S_OK;
}

void TilemapTool::Release()
{
}

void TilemapTool::Update()
{
	// save & load
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
	{
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// 집
			//Load(1);
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// 마당
			//Load(2);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))	// 상점
			Load(3);
	}
	else
	{
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// 집
			//Save(1);
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// 마당
			//Save(2);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))	// 상점
			Save(3);
	}

	// 메인 영역 계산
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + (TILESIZE * STORE_TILE_X);
	rcMain.bottom = rcMain.top + (TILESIZE * STORE_TILE_Y);

	// 정보 변경
	if (PtInRect(&rcMain, g_ptMouse))
	{
		// 왼 클릭 벽
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			ptSelectedFrame.x = posX / TILESIZE;
			ptSelectedFrame.y = posY / TILESIZE;
			ptSelectedFrameShow.x = g_ptMouse.x / TILESIZE;
			ptSelectedFrameShow.y = g_ptMouse.y / TILESIZE;

			// 타입
			storeTileInfo[ptSelectedFrame.x + STORE_TILE_X * ptSelectedFrame.y].tileType = TileType::COUNTER;
		}

		// 우 클릭 길
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			ptSelectedFrame.x = posX / TILESIZE;
			ptSelectedFrame.y = posY / TILESIZE;
			ptSelectedFrameShow.x = g_ptMouse.x / TILESIZE;
			ptSelectedFrameShow.y = g_ptMouse.y / TILESIZE;

			// 타입
			storeTileInfo[ptSelectedFrame.x + STORE_TILE_X * ptSelectedFrame.y].tileType = TileType::FLOOR;
		}
	}
}

void TilemapTool::Render(HDC hdc)
{
	// 뒤에 흰 바탕으로 칠하기
	PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

	// 하우스 메인 타일 출력
	for (int i = 0; i < STORE_TILE_X * STORE_TILE_Y; i++)
	{
		sample->FrameRender(hdc,
			storeTileInfo[i].rcTile.left,
			storeTileInfo[i].rcTile.top,
			storeTileInfo[i].frameX,
			storeTileInfo[i].frameY);
	}

	// 선택 영역 표시
	hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
	Rectangle(hdc, ptSelectedFrameShow.x * TILESIZE, ptSelectedFrameShow.y * TILESIZE,
		ptSelectedFrameShow.x * TILESIZE + TILESIZE, ptSelectedFrameShow.y * TILESIZE + TILESIZE);
	SelectObject(hdc, hOldSelectedBrush);

	// 출력
	for (int i = 0; i < STORE_TILE_Y; i++)	// 세로
	{
		for (int j = 0; j < STORE_TILE_X; j++)	// 가로
		{
			int a;
			// 타입
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::GROUND)
				a = 0;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::WALL)
				a = 1;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::HOUSEDOOR)
				a = 2;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::CAVE)
				a = 3;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::STOREDOOR)
				a = 4;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::POND)
				a = 5;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::FLOOR)
				a = 6;
			if (storeTileInfo[i * STORE_TILE_X + j].tileType == TileType::COUNTER)
				a = 7;

			wsprintf(szText, "%d", a);
			TextOut(hdc, 300 + j * 15, i * 15, szText, strlen(szText));
		}
	}
}

void TilemapTool::Save(int sceneNum)
{
	//맵 이미지 save
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	//파일 이름에, 쓸것이다, 공유는 안하고, 보안, 항상 생성하고, 일반 파일로 생성할 것이며, 템플릿 머시기는 그냥 널 넣어두자
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //.c_str()
	DWORD writtenBytes;
	WriteFile(hFile, storeTileInfo, sizeof(TILE_INFO) * STORE_TILE_X * STORE_TILE_Y, &writtenBytes, NULL);   //viod*는 어떤 자료형이든 값으로 받을 수 있다
	CloseHandle(hFile);
}

void TilemapTool::Load(int sceneNum)
{
	//맵 이미지 load
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD readBytes;
	if (ReadFile(hFile, storeTileInfo, sizeof(TILE_INFO) * STORE_TILE_X * STORE_TILE_Y, &readBytes, NULL)) {}
	else
		MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
	CloseHandle(hFile);
}
*/



#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"

TILE_INFO TilemapTool::tilesetTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];		// 타일시트
TILE_INFO TilemapTool::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];			// 집
TILE_INFO TilemapTool::storeTileInfo[STORE_TILE_X * STORE_TILE_Y];			// 상점
TILE_INFO TilemapTool::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];				// 마당
// 광산

HRESULT TilemapTool::Init()
{
	// 크기 조정
	SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

	// 샘플
	ImageManager::GetSingleton()->AddImage("farmtile", "Image/SpringFarm.bmp", 1280*3, 896*3, FARM_TILE_X, FARM_TILE_Y); //1280 * 3//
	sample = ImageManager::GetSingleton()->FindImage("farmtile");


	// 메인 공간 이미지 정보 저장
	for (int i = 0; i < FARM_TILE_Y; i++)	// 세로
	{
		for (int j = 0; j < FARM_TILE_X; j++)	//가로
		{
			// 몇번째 프레임에 있는 이미지를 들고와서
			farmTileInfo[i * FARM_TILE_X + j].frameX = j;
			farmTileInfo[i * FARM_TILE_X + j].frameY = i;

			// 차례대로 저장해주기
			farmTileInfo[i * FARM_TILE_X + j].rcTile.left = F_TILESIZE * j;
			farmTileInfo[i * FARM_TILE_X + j].rcTile.top = F_TILESIZE * i;
			farmTileInfo[i * FARM_TILE_X + j].rcTile.right = farmTileInfo[i * FARM_TILE_X + j].rcTile.left + F_TILESIZE;
			farmTileInfo[i * FARM_TILE_X + j].rcTile.bottom = farmTileInfo[i * FARM_TILE_X + j].rcTile.top + F_TILESIZE;

			farmTileInfo[i * FARM_TILE_X + j].tileType = TileType::GROUND;

			farmTileInfo[i * FARM_TILE_X + j].objFrameX = -1;
			farmTileInfo[i * FARM_TILE_X + j].objFrameY = -1;

			farmTileInfo[i * FARM_TILE_X + j].seedFrameX = -1;
			farmTileInfo[i * FARM_TILE_X + j].seedFrameY = -1;
			farmTileInfo[i * FARM_TILE_X + j].seedType = SeedType::NONE;
			farmTileInfo[i * FARM_TILE_X + j].day = -1;
		}
	}

	return S_OK;
}

void TilemapTool::Release()
{
}

void TilemapTool::Update()
{
	// save & load
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
	{
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// 집
			//Load(1);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// 마당
			Load(2);
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))	// 상점
			//Load(3);
	}
	else
	{
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// 집
			//Save(1);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// 마당
			Save(2);
		//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))	// 상점
			//Save(3);
	}

	// 메인 영역 계산
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + (F_TILESIZE * FARM_TILE_X);
	rcMain.bottom = rcMain.top + (F_TILESIZE * FARM_TILE_Y);

	// 정보 변경
	if (PtInRect(&rcMain, g_ptMouse))
	{
		// 왼 클릭 벽
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x + TILEMAPTOOLSIZE_X * 2;
			int posY = g_ptMouse.y ;
			ptSelectedFrame.x = posX / F_TILESIZE;
			ptSelectedFrame.y = posY / F_TILESIZE;
			ptSelectedFrameShow.x = g_ptMouse.x / F_TILESIZE;
			ptSelectedFrameShow.y = g_ptMouse.y / F_TILESIZE;

			// 타입
			farmTileInfo[ptSelectedFrame.x + FARM_TILE_X * ptSelectedFrame.y].tileType = TileType::BUSSTOP;
		}

		// 우 클릭 길
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
		{
			int posX = g_ptMouse.x + TILEMAPTOOLSIZE_X*2;
			int posY = g_ptMouse.y ;
			ptSelectedFrame.x = posX / F_TILESIZE;
			ptSelectedFrame.y = posY / F_TILESIZE;
			ptSelectedFrameShow.x = g_ptMouse.x / F_TILESIZE;
			ptSelectedFrameShow.y = g_ptMouse.y / F_TILESIZE;

			// 타입
			farmTileInfo[ptSelectedFrame.x + FARM_TILE_X * ptSelectedFrame.y].tileType = TileType::GROUND;
		}
	}
}

void TilemapTool::Render(HDC hdc)
{
	// 뒤에 흰 바탕으로 칠하기
	PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

	// 하우스 메인 타일 출력
	for (int i = 0; i < FARM_TILE_X * FARM_TILE_Y; i++)
	{
		sample->FrameRender(hdc,
			farmTileInfo[i].rcTile.left - TILEMAPTOOLSIZE_X * 2,
			farmTileInfo[i].rcTile.top ,
			farmTileInfo[i].frameX,
			farmTileInfo[i].frameY);
	}

	// 선택 영역 표시
	hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
	Rectangle(hdc, ptSelectedFrameShow.x * F_TILESIZE, ptSelectedFrameShow.y * F_TILESIZE,
		ptSelectedFrameShow.x * F_TILESIZE + F_TILESIZE, ptSelectedFrameShow.y * F_TILESIZE + F_TILESIZE);
	SelectObject(hdc, hOldSelectedBrush);

	// 출력
	for (int i = 0; i < FARM_TILE_Y; i++)	// 세로
	{
		for (int j = 0; j < FARM_TILE_X; j++)	// 가로
		{
			int a;
			// 타입
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::GROUND)
				a = 0;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::WALL)
				a = 1;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::HOUSEDOOR)
				a = 2;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::CAVE)
				a = 3;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::STOREDOOR)
				a = 4;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::POND)
				a = 5;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::FLOOR)
				a = 6;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::COUNTER)
				a = 7;
			if (farmTileInfo[i * FARM_TILE_X + j].tileType == TileType::BUSSTOP)
				a = 8;

			wsprintf(szText, "%d", a);
			TextOut(hdc, 300 + j * 15, i * 15, szText, strlen(szText));
		}
	}
}

void TilemapTool::Save(int sceneNum)
{
	//맵 이미지 save
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	//파일 이름에, 쓸것이다, 공유는 안하고, 보안, 항상 생성하고, 일반 파일로 생성할 것이며, 템플릿 머시기는 그냥 널 넣어두자
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //.c_str()
	DWORD writtenBytes;
	WriteFile(hFile, farmTileInfo, sizeof(TILE_INFO) * FARM_TILE_X * FARM_TILE_Y, &writtenBytes, NULL);   //viod*는 어떤 자료형이든 값으로 받을 수 있다
	CloseHandle(hFile);
}

void TilemapTool::Load(int sceneNum)
{
	//맵 이미지 load
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD readBytes;
	if (ReadFile(hFile, farmTileInfo, sizeof(TILE_INFO) * FARM_TILE_X * FARM_TILE_Y, &readBytes, NULL)) {}
	else
		MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
	CloseHandle(hFile);
}

