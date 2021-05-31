#include "FarmScene.h"
#include "Image.h"
#include "DataManager.h"
#include "Player.h"
#include "InventoryManager.h"
#include "StoreData.h"

HRESULT FarmScene::Init()
{
	// ���� ������ ����
	ImageManager::GetSingleton()->AddImage("�����̹���", "Image/SpringFarm.bmp", 1280 * 3, 896 * 3, FARM_TILE_X, FARM_TILE_Y);
	farmSample = ImageManager::GetSingleton()->FindImage("�����̹���");

	// �� ���� ����
	ImageManager::GetSingleton()->AddImage("tile", "Image/TileSet.bmp", 64 * 4, 128 * 4, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 255, 255));
	tilesetSample = ImageManager::GetSingleton()->FindImage("tile");
	
	// ��� Ű��� ���� 
	ImageManager::GetSingleton()->AddImage("���Ű���", "Image/CropsGrow.bmp", 96 * 4, 288 * 4, 6, 9 * 2, true, RGB(255, 255, 255));
	growCrop = ImageManager::GetSingleton()->FindImage("���Ű���");
	storeData = new StoreData();
	storeData->Init();

	// �÷��̾�
	player = new Player();
	player->Init();
	if (DataManager::GetSingleton()->GetPreScene() == 0)    // �׽�Ʈ
		player->SetPlayerImagePos({ 830, 630 });
	if (DataManager::GetSingleton()->GetPreScene() == 1)    // ������ ��ȯ�� ���
		player->SetPlayerImagePos({ 3050, 685 });
	if (DataManager::GetSingleton()->GetPreScene() == 3)    // �������� ��ȯ�� ���
		player->SetPlayerImagePos({ 830, 630 });
	player->SetCurrImageSize({ farmSample->GetWidth(), farmSample->GetHeight() });

	// ���� ���� (ȭ��� �������� �̹��� ���� ��ġ)
	renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
	renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

	//
	farmDir = 1;
	canMove = true;

	// Ÿ�� ���� ��������
	farmTileInfo = DataManager::GetSingleton()->GetFarmScene();

	// ��
	ImageManager::GetSingleton()->AddImage("��", "Image/blackLight.bmp", WINSIZE_X, WINSIZE_Y);
	light = ImageManager::GetSingleton()->FindImage("��");

	return S_OK;
}

void FarmScene::Release()
{
}

void FarmScene::MoveControl()
{
	if (renderCoor.x < 0)
		renderCoor.x = 0;
	if (renderCoor.x > farmSample->GetWidth() - WINSIZE_X)
		renderCoor.x = farmSample->GetWidth() - WINSIZE_X;
	if (renderCoor.y < 0)
		renderCoor.y = 0;
	if (renderCoor.y > farmSample->GetHeight() - WINSIZE_Y)
		renderCoor.y = farmSample->GetHeight() - WINSIZE_Y;
}

void FarmScene::Move()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// ��
		farmDir = 0;
	else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// ��
		farmDir = 1;
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// ��
		farmDir = 2;
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// ��
		farmDir = 3;
	playerFarmCollision();
}

void FarmScene::playerFarmCollision()
{
	int left = (player->GetPlayerRect().left + renderCoor.x) / F_TILESIZE;
	int top = (player->GetPlayerRect().top + renderCoor.y) / F_TILESIZE;
	int right = (player->GetPlayerRect().right + renderCoor.x) / F_TILESIZE;
	int bottom = (player->GetPlayerRect().bottom + renderCoor.y) / F_TILESIZE;

	switch (farmDir)
	{
	case 0:
		// ����->�� �� ����
		if (farmTileInfo[left + top * FARM_TILE_X].tileType == TileType::HOUSEDOOR ||
			farmTileInfo[right + top * FARM_TILE_X].tileType == TileType::HOUSEDOOR)
		{
			DataManager::GetSingleton()->SetPreScene(2); // ���� ��(����=2) ����
			SceneManager::GetSingleton()->ChangeScene("�Ͽ콺��");
		}
		// ��->���� �� ����
		if (farmTileInfo[left + top * FARM_TILE_X].tileType == TileType::STOREDOOR ||
			farmTileInfo[right + top * FARM_TILE_X].tileType == TileType::STOREDOOR)
		{
			DataManager::GetSingleton()->SetPreScene(2); // ���� ��(����=2) ����
			SceneManager::GetSingleton()->ChangeScene("������");
		}
		if (farmTileInfo[left + top * FARM_TILE_X].tileType == TileType::WALL)	// left,top
			canMove = false;
		if (farmTileInfo[right + top * FARM_TILE_X].tileType == TileType::WALL)	// right,top
			canMove = false;
		break;
	case 1:
		if (farmTileInfo[left + bottom * FARM_TILE_X].tileType == TileType::WALL)	// left,bottom
			canMove = false;
		if (farmTileInfo[right + bottom * FARM_TILE_X].tileType == TileType::WALL)	// right,bottom
			canMove = false;
		break;
	case 2:
		if (farmTileInfo[left + top * FARM_TILE_X].tileType == TileType::WALL)	// left,top
			canMove = false;
		if (farmTileInfo[left + bottom * FARM_TILE_X].tileType == TileType::WALL)	// left,bottom
			canMove = false;
		break;
	case 3:
		if (farmTileInfo[right + top * FARM_TILE_X].tileType == TileType::WALL)	// right,top
			canMove = false;
		if (farmTileInfo[right + bottom * FARM_TILE_X].tileType == TileType::WALL)	// right,bottom
			canMove = false;
		break;
	default:
		break;
	}

	player->SetCanMove(canMove);
}
  
/* ��� �κ� 550 ����.. */
void FarmScene::testDigUp()
{
	// �� �� �� ��
	int dir[4][2] = { {0,-1}, {0, 1}, {-1, 0}, {1,0} };
	// 9�� ���鼭 ��ȭ Ȯ������
	int neighborDir[9][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}, {1,1} };

	/* Ȯ�ο� ������� */
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		bool canDig = true;
		if (g_ptMouse.x > InventoryManager::GetSingleton()->GetSmallInvenRect().left && g_ptMouse.x < InventoryManager::GetSingleton()->GetSmallInvenRect().right
			&& g_ptMouse.y > InventoryManager::GetSingleton()->GetSmallInvenRect().top && g_ptMouse.y < InventoryManager::GetSingleton()->GetSmallInvenRect().bottom)
			canDig = false;

		if (canDig)
		{
			// ü�� ������
			InventoryManager::GetSingleton()->SetEnergy(5);

			int posX = (g_ptMouse.x + renderCoor.x) / F_TILESIZE;
			int posY = (g_ptMouse.y + renderCoor.y) / F_TILESIZE;

			farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::DIG;
			farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
			farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 2;

			for (int i = 0; i < 9; i++)
			{
				int neighborX = posX + neighborDir[i][0];
				int neighborY = posY + neighborDir[i][1];

				if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::GROUND) // �� �ƴ� ���� �˻� x
					continue;

				int check = 0;
				for (int j = 0; j < 4; j++)
				{
					if (farmTileInfo[(neighborX + dir[j][0]) + FARM_TILE_X * (neighborY + dir[j][1])].tileType != TileType::GROUND)
						check += pow(10, j);    // ��:1 ��:10 ��:100 ��:1000
				}

				// Ÿ�� 16�� ����� ��....
				if (check == 0) // �ֺ��� �ƹ��͵� ����
				{
					/*farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::NONE;
					farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
					farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 2;*/
				}
				else if (check == 1)    // ��
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 6;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 3;
					}
				}
				else if (check == 10)    // ��
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 6;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 3;
					}
				}
				else if (check == 100)    // ��
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 6;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 3;
					}
				}
				else if (check == 1000)    // ��
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 6;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 3;
					}
				}
				else if (check == 11)    // ����
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 5;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 2;
					}
				}
				else if (check == 101)    // ����
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 7;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 4;
					}
				}
				else if (check == 1001)    // ���
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 7;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 4;
					}
				}
				else if (check == 110)    // ����
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 7;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 4;
					}
				}
				else if (check == 1010)    // �Ͽ�
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 7;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 4;
					}
				}
				else if (check == 1100)    // �¿�
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 5;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 2;
					}
				}
				else if (check == 111)    // ������
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 1;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 1;
					}
				}
				else if (check == 1011)    // ���Ͽ�
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 0;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 0;
					}
				}
				else if (check == 1110)    // ���¿�
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 0;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 0;
					}
				}
				else if (check == 1101)    // ���¿�
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 1;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 2;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 2;
					}
				}
				else if (check == 1111)    // �����¿�
				{
					if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::WETDIG)
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 1;
					}
					else
					{
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::DIG;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 3;
						farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 5;
					}
				}
			}
		}
	}
}

void FarmScene::testWater()
{
	// �� �� �� ��
	int dir[4][2] = { {0,-1}, {0, 1}, {-1, 0}, {1,0} };
	// 9�� ���鼭 ��ȭ Ȯ������
	int neighborDir[9][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}, {1,1} };

	/* Ȯ�ο� ���Ѹ��� */
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		// ���� �κ��丮 ���ؼ� �� �Ѹ���
		bool canWater = true;
		if (g_ptMouse.x > InventoryManager::GetSingleton()->GetSmallInvenRect().left && g_ptMouse.x < InventoryManager::GetSingleton()->GetSmallInvenRect().right
			&& g_ptMouse.y > InventoryManager::GetSingleton()->GetSmallInvenRect().top && g_ptMouse.y < InventoryManager::GetSingleton()->GetSmallInvenRect().bottom)
			canWater = false;

		// Ŭ�� ��ġ �� �ε���
		int posX = (g_ptMouse.x + renderCoor.x) / F_TILESIZE;
		int posY = (g_ptMouse.y + renderCoor.y) / F_TILESIZE;

		// �� ���� ��
		if (farmTileInfo[posX + FARM_TILE_X * posY].tileType == TileType::DIG)
		{
			// ü�� ������
			InventoryManager::GetSingleton()->SetEnergy(5);

			// ȥ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 0 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 5;
			}

			// ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 0 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 3;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 1 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 1 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 5;
			}

			// �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 5;
			}

			// �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 1 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 0 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 3;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// �� �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 0)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 0;
			}

			// �� �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 1;
			}

			// �� �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 1)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 1;
			}

			// �� �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 0)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 0;
			}

			// �� �� �� ��
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 5)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 1;
			}
		}
	}
}

void FarmScene::testGrow()
{
	// Ŭ��
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		// Ŭ�� ��ġ �� �ε���
		int posX = (g_ptMouse.x + renderCoor.x) / F_TILESIZE;
		int posY = (g_ptMouse.y + renderCoor.y) / F_TILESIZE;
			
		// �κ��丮 ���� ���� �������� + ������ �ε��� ��ġ
		vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
		int selectedIndex = InventoryManager::GetSingleton()->GetSelectedIndex();

		// ���� �ƴϸ�
		if ((*vInven)[selectedIndex] != NULL)
		{
			// Ŭ�� ��ġ�� ���Ѹ��� + ���� ���� �� �̶��
			if (farmTileInfo[posX + FARM_TILE_X * posY].tileType == TileType::WETDIG
				&& farmTileInfo[posX + FARM_TILE_X * posY].seedType != SeedType::CROP)
			{
				// ��� Ÿ������ �����ϱ�
				farmTileInfo[posX + FARM_TILE_X * posY].seedType = SeedType::CROP;

				// ���� ����
				farmTileInfo[posX + FARM_TILE_X * posY].day = InventoryManager::GetSingleton()->GetDay();
				farmTileInfo[posX + FARM_TILE_X * posY].seedFrameX = 0;
				for (int i = 0; i < storeData->GetMapCropData()->size(); i++)
				{
					string curMapCrop = (storeData->GetMapCropData()->at(i)->cropName);
					string selectedCropName = (*vInven)[selectedIndex]->productName;
					if (curMapCrop.compare(selectedCropName) == 0)
					{
						farmTileInfo[posX + FARM_TILE_X * posY].seedFrameY = i * 2 + 1;
						break;
					}
				}

				// ����� ���� ���� ����
				(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount -= 1;
				if ((*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount <= 0)
				{
					// 0���� �ش簪�� �ִ� ���͸� �η�
					(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()] = NULL;
				}
			}
		}
	}
}

void FarmScene::testHarvest()
{
	vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
	// �� Ŭ�� �� ��ġ��
	farmClicked.x = (winClicked.x + renderCoor.x) / F_TILESIZE;
	farmClicked.y = (winClicked.y + renderCoor.y) / F_TILESIZE;

	// ����̰� + �� �ڶ�����(��Ȯ ���� map�� �ִ�) ��Ȯ����
	if (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedType == SeedType::CROP
		&& checkDay.find(farmClicked.x + FARM_TILE_X * farmClicked.y) != checkDay.end())
	{
		// � ��� Ȯ�� ���� �ϰ�
		int mapKey = (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedFrameY - 1) / 2;

		// ���� ���ٰ� �����ְ�
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedType = SeedType::NONE;
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedFrameX = -1;
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedFrameY = -1;
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].day = -1;

		// map���� ��������
		checkDay.erase(farmClicked.x + FARM_TILE_X * farmClicked.y);

		// �κ��� �־�����
		string cropName = (storeData->GetMapCropData()->find(mapKey)->second->cropName) + "GROWN";
		bool alreadyExist = false;
		int existIndex;

		// �̹� �ִ°�?
		for (int i = 0; i < vInven->size(); i++)
		{
			if ((*vInven)[i] != NULL)
			{
				string test = (*vInven)[i]->productName;
				if (((*vInven)[i]->productName).compare(cropName) == 0)
				{
					existIndex = i;
					alreadyExist = true;
					break;
				}
			}
		}

		// ����
		if (alreadyExist == false)
		{
			// ���λ���
			int price = storeData->GetMapCropData()->find(mapKey)->second->price * 2;
			InventoryManager::GetSingleton()->SetAddInven(new INVEN_INFO(ObjectType::CROPGROWN, 1, mapKey, 0, price, cropName, 1));
		}
		// �ִ�
		else
		{
			// ���� �κ��丮 ���Ϳ� �ִ� �ε��� ��ġ�� �ִ� ��� �� �ÿ��ֱ� 
			(*vInven)[existIndex]->amount += 1;
		}
	}
}

void FarmScene::testSale()
{
	// �κ��丮 ���� ���� �������� + ������ �ε��� ��ġ
	vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
	int selectedIndex = InventoryManager::GetSingleton()->GetSelectedIndex();

	// ���� �� �� �ִ� ��ǰ�̸�
	if ((*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->salePrice != 0)
	{
		// �Ǹ��� ��� ���� ��
		int price = (*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->salePrice;
		InventoryManager::GetSingleton()->SetPlayerMoneyIncrease(price);

		// �Ǹ��� ��� ���� ����
		(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount -= 1;
		if ((*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount <= 0)
		{
			// 0���� �ش簪�� �ִ� ���͸� �η�
			(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()] = NULL;
		}
	}
}

void FarmScene::Update()
{
	player->Update();

	// ȭ�� �̵� + �÷��̾� �浹 üũ
	canMove = true;
	Move();

	// ���� ���� ��ġ
	renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
	renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

	// ȭ�� �̵� ����
	MoveControl();

	// ���� ���� �ε���
	startFrame.x = renderCoor.x / F_TILESIZE;
	startFrame.y = renderCoor.y / F_TILESIZE;

	/* Ȯ�ο� ������� */
	//!InventoryManager::GetSingleton()->GetBigOpened() &&
	if (InventoryManager::GetSingleton()->GetSelectedItem().compare("Axe") == 0)
		testDigUp();
	else if (InventoryManager::GetSingleton()->GetSelectedItem().compare("Can") == 0)
		testWater();
	else if (InventoryManager::GetSingleton()->GetSeletecObjectType() == ObjectType::CROPS)
		testGrow();

	// �� Ŭ�� �� ������ �� ��ġ�� 
	winClicked.x = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * ((int)((g_ptMouse.x + renderCoor.x) / F_TILESIZE) - startFrame.x);
	winClicked.y = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * ((int)((g_ptMouse.y + renderCoor.y) / F_TILESIZE) - startFrame.y);

	// �� Ŭ�� �� �ε��� ��ġ��
	farmClicked.x = (winClicked.x + renderCoor.x) / F_TILESIZE;
	farmClicked.y = (winClicked.y + renderCoor.y) / F_TILESIZE;

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
	{
		// ��Ȯ
		if(farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedType == SeedType::CROP)
			testHarvest();
		// �Ǹ� -> CAVE Ÿ�� ����� �״�..
		else if ((farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 25 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 4)
			|| (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 25 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 5)
			|| (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 26 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 4)
			|| (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 26 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 5))
			testSale();
	}

	// ��¥�� �귶����
	DataManager::GetSingleton()->Update();
}

void FarmScene::Render(HDC hdc)
{
	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // ����
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // ����
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);

			// ���� ���
			farmSample->FrameRender(hdc,
				tempPosX,
				tempPosY,
				farmTileInfo[tempIndex].frameX,
				farmTileInfo[tempIndex].frameY);

			// �� ���� + �� ����
			if (farmTileInfo[tempIndex].tileType == TileType::DIG
				|| farmTileInfo[tempIndex].tileType == TileType::WETDIG)
			{
				tilesetSample->FrameRender(hdc,
					tempPosX,
					tempPosY,
					farmTileInfo[tempIndex].objFrameX,
					farmTileInfo[tempIndex].objFrameY);
			}

			// ��
			if (farmTileInfo[tempIndex].tileType == TileType::WALL || farmTileInfo[tempIndex].tileType == TileType::POND)
			{
				 //Rectangle(hdc, tempPosX, tempPosY,
					 //tempPosX + F_TILESIZE, tempPosY + F_TILESIZE);
			}

			//wsprintf(szText, "%d , %d", farmTileInfo[tempIndex].frameX, farmTileInfo[tempIndex].frameY);
			//TextOut(hdc, tempPosX, tempPosY, szText, strlen(szText));
		}
	}

	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // ����
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // ����
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);

			if (player->GetPlayerPos().y >= tempPosY - F_TILESIZE)
			{
				// ��� Ű����
				if (farmTileInfo[tempIndex].seedType == SeedType::CROP)
				{
					int a = farmTileInfo[tempIndex].seedFrameX;
					int b = farmTileInfo[tempIndex].day;
					int seedFrameX = a + (InventoryManager::GetSingleton()->GetDay() - b);

					// 6�ϵ����� �ִ�� Ű��� map�� ����
					if (seedFrameX >= 5)
					{
						seedFrameX = 5;
						checkDay.insert(pair<int, bool>(tempIndex, true));
					}

					growCrop->FrameRender(hdc,
						tempPosX,
						tempPosY,
						seedFrameX,
						farmTileInfo[tempIndex].seedFrameY);

					// ��� ���κе� �׷����� Ű����
					growCrop->FrameRender(hdc,
						tempPosX,
						tempPosY - F_TILESIZE,
						seedFrameX,
						farmTileInfo[tempIndex].seedFrameY - 1);
				}
			}
		}
	}

	// �÷��̾�
	player->Render(hdc);

	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // ����
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // ����
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);

			if (player->GetPlayerPos().y < tempPosY - F_TILESIZE)
			{
				// ��� Ű����
				if (farmTileInfo[tempIndex].seedType == SeedType::CROP)
				{
					int a = farmTileInfo[tempIndex].seedFrameX;
					int b = farmTileInfo[tempIndex].day;
					int seedFrameX = a + (InventoryManager::GetSingleton()->GetDay() - b);

					// 6�ϵ����� �ִ�� Ű��� map�� ����
					if (seedFrameX >= 5)
					{
						seedFrameX = 5;
						checkDay.insert(pair<int, bool>(tempIndex, true));
					}

					growCrop->FrameRender(hdc,
						tempPosX,
						tempPosY,
						seedFrameX,
						farmTileInfo[tempIndex].seedFrameY);

					// ��� ���κе� �׷����� Ű����
					growCrop->FrameRender(hdc,
						tempPosX,
						tempPosY - F_TILESIZE,
						seedFrameX,
						farmTileInfo[tempIndex].seedFrameY - 1);
				}
			}
		}
	}

	// ���콺 ��ġ ��
	hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
	hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

	if (InventoryManager::GetSingleton()->GetSmallInvenRect().left < g_ptMouse.x
		&& InventoryManager::GetSingleton()->GetSmallInvenRect().right > g_ptMouse.x
		&& InventoryManager::GetSingleton()->GetSmallInvenRect().top < g_ptMouse.y
		&& InventoryManager::GetSingleton()->GetSmallInvenRect().bottom > g_ptMouse.y)
	{
	}
	else
	{
		MoveToEx(hdc, winClicked.x, winClicked.y, NULL);
		LineTo(hdc, winClicked.x + F_TILESIZE, winClicked.y);
		LineTo(hdc, winClicked.x + F_TILESIZE, winClicked.y + F_TILESIZE);
		LineTo(hdc, winClicked.x, winClicked.y + F_TILESIZE);
		LineTo(hdc, winClicked.x, winClicked.y);
	}

	hpen = (HPEN)::SelectObject(hdc, hpenOld);
	DeleteObject(hpen);

	// �κ��丮
	InventoryManager::GetSingleton()->Render(hdc);
}
