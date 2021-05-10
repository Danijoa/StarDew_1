#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"

TILE_INFO TilemapTool::houseTileInfo[HOUSE_TILE_X * HOUSE_TILE_Y];	// ��
TILE_INFO TilemapTool::farmTileInfo[FARM_TILE_X * FARM_TILE_Y];		// ����
// ����
// ����
// ����

HRESULT TilemapTool::Init()
{
	// ũ�� ����
	SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

	// ��
	ImageManager::GetSingleton()->AddImage("���̹���", "Image/House.bmp", 800, 832, HOUSE_TILE_X, HOUSE_TILE_Y);	
	houseSample = ImageManager::GetSingleton()->FindImage("���̹���");
	// ����
	//ImageManager::GetSingleton()->AddImage("�����̹���", "Image/SpringFarm.bmp", 800, 832, FARM_TILE_X, FARM_TILE_Y);
	//houseSample = ImageManager::GetSingleton()->FindImage("�����̹���");

	// �Ͽ콺 ���� ���� �̹��� ���� ����
	for (int i = 0; i < HOUSE_TILE_Y; i++)	// ����
	{
		for (int j = 0; j < HOUSE_TILE_X; j++)	//����
		{
			// ���° �����ӿ� �ִ� �̹����� ���ͼ�
			houseTileInfo[i * HOUSE_TILE_X + j].frameX = j;
			houseTileInfo[i * HOUSE_TILE_X + j].frameY = i;
										   
			// ���ʴ�� �������ֱ�		  
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.left = TILESIZE * j;
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.top = TILESIZE * i;
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.right = houseTileInfo[i * HOUSE_TILE_X + j].rcTile.left + TILESIZE;
			houseTileInfo[i * HOUSE_TILE_X + j].rcTile.bottom = houseTileInfo[i * HOUSE_TILE_X + j].rcTile.top + TILESIZE;
	
			// Ÿ��
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
		// �ε� ��� ���
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// ��
			Load(1);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// ����
			Load(2);
	}
	else
	{
		// ���̺� ��� ���
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))	// ��
			Save(1);
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))	// ����
			Save(2);
	}


	// �Ͽ콺 ���� ���� ���
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + (TILESIZE * HOUSE_TILE_X);
	rcMain.bottom = rcMain.top + (TILESIZE * HOUSE_TILE_Y);

	if (PtInRect(&rcMain, g_ptMouse))
	{
		// �� Ŭ�� ��
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			ptSelectedFrame.x = posX / TILESIZE;
			ptSelectedFrame.y = posY / TILESIZE;

			// Ÿ��
			houseTileInfo[ptSelectedFrame.x + HOUSE_TILE_X * ptSelectedFrame.y].tileType = TileType::WALL;
		}

		// �� Ŭ�� ��
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			ptSelectedFrame.x = posX / TILESIZE;
			ptSelectedFrame.y = posY / TILESIZE;

			// Ÿ��
			//houseTileInfo[ptSelectedFrame.x + HOUSE_TILE_X * ptSelectedFrame.y].tileType = TileType::GROUND;
			houseTileInfo[ptSelectedFrame.x + HOUSE_TILE_X * ptSelectedFrame.y].tileType = TileType::HOUSEDOOR;
		}
	}
}

void TilemapTool::Render(HDC hdc)
{
	// �ڿ� �� �������� ĥ�ϱ�
	PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

	// �Ͽ콺 ���� Ÿ�� ���
	for (int i = 0; i < HOUSE_TILE_X * HOUSE_TILE_Y; i++)
	{
		houseSample->FrameRender(hdc,
			houseTileInfo[i].rcTile.left,
			houseTileInfo[i].rcTile.top,
			houseTileInfo[i].frameX,
			houseTileInfo[i].frameY);
	}

	// ���� ���� ǥ��
	hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
	Rectangle(hdc, ptSelectedFrame.x * TILESIZE, ptSelectedFrame.y * TILESIZE, 
		ptSelectedFrame.x * TILESIZE + TILESIZE, ptSelectedFrame.y * TILESIZE + TILESIZE);
	SelectObject(hdc, hOldSelectedBrush);

	// ���
	for (int i = 0; i < HOUSE_TILE_Y; i++)	// ����
	{
		for (int j = 0; j < HOUSE_TILE_X; j++)	// ����
		{
			int a;
			// Ÿ��
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
	//�� �̹��� save
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	//���� �̸���, �����̴�, ������ ���ϰ�, ����, �׻� �����ϰ�, �Ϲ� ���Ϸ� ������ ���̸�, ���ø� �ӽñ�� �׳� �� �־����
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //.c_str()
	DWORD writtenBytes;
	WriteFile(hFile, houseTileInfo, sizeof(TILE_INFO) * HOUSE_TILE_X * HOUSE_TILE_Y, &writtenBytes, NULL);   //viod*�� � �ڷ����̵� ������ ���� �� �ִ�
	CloseHandle(hFile);
}

void TilemapTool::Load(int sceneNum)
{
	//�� �̹��� load
	string fileName = "Save/saveMapData";
	fileName += to_string(sceneNum) + ".map";
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD readBytes;
	if (ReadFile(hFile, houseTileInfo, sizeof(TILE_INFO) * HOUSE_TILE_X * HOUSE_TILE_Y, &readBytes, NULL)) {}
	else
		MessageBox(g_hWnd, "�������� �ε� ����", "����", MB_OK);
	CloseHandle(hFile);
}
