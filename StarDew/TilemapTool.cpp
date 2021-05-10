#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"

TILE_INFO TilemapTool::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];	// 집
TILE_INFO TilemapTool::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];		// 마당
// 광산
// 마을
// 상점

HRESULT TilemapTool::Init()
{
	// 크기 조정
	SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

	// 집
	ImageManager::GetSingleton()->AddImage("집이미지", "Image/House.bmp", 800, 832, HOUSE_TILE_X, HOUSE_TILE_Y);	
	houseSample = ImageManager::GetSingleton()->FindImage("집이미지");
	// 마당
	//ImageManager::GetSingleton()->AddImage("마당이미지", "Image/SpringFarm.bmp", 800, 832, FARM_TILE_X, FARM_TILE_Y);
	//houseSample = ImageManager::GetSingleton()->FindImage("마당이미지");

	// 하우스 메인 공간 이미지 정보 저장
	for (int i = 0; i < HOUSE_TILE_Y; i++)	// 세로
	{
		for (int j = 0; j < HOUSE_TILE_X; j++)	//가로
		{
			// 몇번째 프레임에 있는 이미지를 들고와서
			houseTileInfo[i * HOUSE_TILE_X + j].frameX = j;
			houseTileInfo[i * HOUSE_TILE_X + j].frameY = i;
										   
			// 차례대로 저장해주기		  
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.left = TILESIZE * j;
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.top = TILESIZE * i;
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.right = houseTileInfo[i * HOUSE_TILE_X + j].rcTile.left + TILESIZE;
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.bottom = houseTileInfo[i * HOUSE_TILE_X + j].rcTile.top + TILESIZE;
	
			// 타입
			houseTileInfo[i * HOUSE_TILE_X + j].tileType = TileType::WALL;
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
		// 로드 기능 사용
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// 집
			Load(1);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// 마당
			Load(2);
	}
	else
	{
		// 세이브 기능 사용
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// 집
			Save(1);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// 마당
			Save(2);
	}


	// 하우스 메인 영역 계산
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + (TILESIZE * HOUSE_TILE_X);
	rcMain.bottom = rcMain.top + (TILESIZE * HOUSE_TILE_Y);

	if (PtInRect(&rcMain, g_ptMouse))
	{
		// 왼 클릭 벽
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			ptSelectedFrame.x = posX / TILESIZE;
			ptSelectedFrame.y = posY / TILESIZE;

			// 타입
			houseTileInfo[ptSelectedFrame.x + HOUSE_TILE_X * ptSelectedFrame.y].tileType = TileType::WALL;
		}

		// 우 클릭 길
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			ptSelectedFrame.x = posX / TILESIZE;
			ptSelectedFrame.y = posY / TILESIZE;

			// 타입
			//houseTileInfo[ptSelectedFrame.x + HOUSE_TILE_X * ptSelectedFrame.y].tileType = TileType::GROUND;
			houseTileInfo[ptSelectedFrame.x + HOUSE_TILE_X * ptSelectedFrame.y].tileType = TileType::HOUSEDOOR;
		}
	}
}

void TilemapTool::Render(HDC hdc)
{
	// 뒤에 흰 바탕으로 칠하기
	PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

	// 하우스 메인 타일 출력
	for (int i = 0; i < HOUSE_TILE_X * HOUSE_TILE_Y; i++)
	{
		houseSample->FrameRender(hdc,
			houseTileInfo[i].rcTile.left,
			houseTileInfo[i].rcTile.top,
			houseTileInfo[i].frameX,
			houseTileInfo[i].frameY);
	}

	// 선택 영역 표시
	hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
	Rectangle(hdc, ptSelectedFrame.x * TILESIZE, ptSelectedFrame.y * TILESIZE, 
		ptSelectedFrame.x * TILESIZE + TILESIZE, ptSelectedFrame.y * TILESIZE + TILESIZE);
	SelectObject(hdc, hOldSelectedBrush);

	// 출력
	for (int i = 0; i < HOUSE_TILE_Y; i++)	// 세로
	{
		for (int j = 0; j < HOUSE_TILE_X; j++)	// 가로
		{
			int a;
			// 타입
			if (houseTileInfo[i * HOUSE_TILE_X + j].tileType == TileType::WALL)
				a = 0;
			if (houseTileInfo[i * HOUSE_TILE_X + j].tileType == TileType::GROUND)
				a = 1;
			if (houseTileInfo[i * HOUSE_TILE_X + j].tileType == TileType::HOUSEDOOR)
				a = 2;

			wsprintf(szText, "%d", a);
			TextOut(hdc, 800 + j * 50, i * 20, szText, strlen(szText));
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
	WriteFile(hFile, houseTileInfo, sizeof(TILE_INFO) * HOUSE_TILE_X * HOUSE_TILE_Y, &writtenBytes, NULL);   //viod*는 어떤 자료형이든 값으로 받을 수 있다
	CloseHandle(hFile);
}

void TilemapTool::Load(int sceneNum)
{
	//맵 이미지 load
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD readBytes;
	if (ReadFile(hFile, houseTileInfo, sizeof(TILE_INFO) * HOUSE_TILE_X * HOUSE_TILE_Y, &readBytes, NULL)) {}
	else
		MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);
	CloseHandle(hFile);
}
