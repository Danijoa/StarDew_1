#include "FarmScene.h"
#include "Image.h"
#include "DataManager.h"
#include "Player.h"
#include "InventoryManager.h"
#include "StoreData.h"

HRESULT FarmScene::Init()
{
	// 마당 미이지 샘플
	ImageManager::GetSingleton()->AddImage("마당이미지", "Image/SpringFarm.bmp", 1280 * 3, 896 * 3, FARM_TILE_X, FARM_TILE_Y);
	farmSample = ImageManager::GetSingleton()->FindImage("마당이미지");

	// 밭 갈기 샘플
	ImageManager::GetSingleton()->AddImage("tile", "Image/TileSet.bmp", 64 * 4, 128 * 4, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 255, 255));
	tilesetSample = ImageManager::GetSingleton()->FindImage("tile");
	
	// 곡식 키우기 샘플 
	ImageManager::GetSingleton()->AddImage("곡식키우기", "Image/CropsGrow.bmp", 96 * 4, 288 * 4, 6, 9 * 2, true, RGB(255, 255, 255));
	growCrop = ImageManager::GetSingleton()->FindImage("곡식키우기");
	storeData = new StoreData();
	storeData->Init();

	// 플레이어
	player = new Player();
	player->Init();
	if (DataManager::GetSingleton()->GetPreScene() == 0)    // 테스트
		player->SetPlayerImagePos({ 830, 630 });
	if (DataManager::GetSingleton()->GetPreScene() == 1)    // 집에서 전환된 장면
		player->SetPlayerImagePos({ 3050, 685 });
	if (DataManager::GetSingleton()->GetPreScene() == 3)    // 상점에서 전환된 장면
		player->SetPlayerImagePos({ 830, 630 });
	player->SetCurrImageSize({ farmSample->GetWidth(), farmSample->GetHeight() });

	// 농장 렌더 (화면상에 보여지는 이미지 시작 위치)
	renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
	renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

	//
	farmDir = 1;
	canMove = true;

	// 타일 정보 가져오기
	farmTileInfo = DataManager::GetSingleton()->GetFarmScene();

	// 빛
	ImageManager::GetSingleton()->AddImage("빛", "Image/blackLight.bmp", WINSIZE_X, WINSIZE_Y);
	light = ImageManager::GetSingleton()->FindImage("빛");

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
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// 상
		farmDir = 0;
	else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// 하
		farmDir = 1;
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// 좌
		farmDir = 2;
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// 우
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
		// 마당->집 씬 변경
		if (farmTileInfo[left + top * FARM_TILE_X].tileType == TileType::HOUSEDOOR ||
			farmTileInfo[right + top * FARM_TILE_X].tileType == TileType::HOUSEDOOR)
		{
			DataManager::GetSingleton()->SetPreScene(2); // 현재 씬(마당=2) 저장
			SceneManager::GetSingleton()->ChangeScene("하우스씬");
		}
		// 집->상점 씬 변경
		if (farmTileInfo[left + top * FARM_TILE_X].tileType == TileType::STOREDOOR ||
			farmTileInfo[right + top * FARM_TILE_X].tileType == TileType::STOREDOOR)
		{
			DataManager::GetSingleton()->SetPreScene(2); // 현재 씬(마당=2) 저장
			SceneManager::GetSingleton()->ChangeScene("상점씬");
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
  
/* 농사 부분 550 줄임.. */
void FarmScene::testDigUp()
{
	// 상 하 좌 우
	int dir[4][2] = { {0,-1}, {0, 1}, {-1, 0}, {1,0} };
	// 9개 돌면서 변화 확인하자
	int neighborDir[9][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}, {1,1} };

	/* 확인용 농사짓기 */
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		bool canDig = true;
		if (g_ptMouse.x > InventoryManager::GetSingleton()->GetSmallInvenRect().left && g_ptMouse.x < InventoryManager::GetSingleton()->GetSmallInvenRect().right
			&& g_ptMouse.y > InventoryManager::GetSingleton()->GetSmallInvenRect().top && g_ptMouse.y < InventoryManager::GetSingleton()->GetSmallInvenRect().bottom)
			canDig = false;

		if (canDig)
		{
			// 체력 줄이자
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

				if (farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType == TileType::GROUND) // 밭 아닌 경우는 검사 x
					continue;

				int check = 0;
				for (int j = 0; j < 4; j++)
				{
					if (farmTileInfo[(neighborX + dir[j][0]) + FARM_TILE_X * (neighborY + dir[j][1])].tileType != TileType::GROUND)
						check += pow(10, j);    // 상:1 하:10 좌:100 우:1000
				}

				// 타입 16개 경우의 수....
				if (check == 0) // 주변에 아무것도 없음
				{
					/*farmTileInfo[neighborX + FARM_TILE_X * neighborY].tileType = TileType::NONE;
					farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameX = 0;
					farmTileInfo[neighborX + FARM_TILE_X * neighborY].objFrameY = 2;*/
				}
				else if (check == 1)    // 상
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
				else if (check == 10)    // 하
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
				else if (check == 100)    // 좌
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
				else if (check == 1000)    // 우
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
				else if (check == 11)    // 상하
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
				else if (check == 101)    // 상좌
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
				else if (check == 1001)    // 상우
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
				else if (check == 110)    // 하좌
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
				else if (check == 1010)    // 하우
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
				else if (check == 1100)    // 좌우
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
				else if (check == 111)    // 상하좌
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
				else if (check == 1011)    // 상하우
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
				else if (check == 1110)    // 하좌우
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
				else if (check == 1101)    // 상좌우
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
				else if (check == 1111)    // 상하좌우
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
	// 상 하 좌 우
	int dir[4][2] = { {0,-1}, {0, 1}, {-1, 0}, {1,0} };
	// 9개 돌면서 변화 확인하자
	int neighborDir[9][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}, {1,1} };

	/* 확인용 물뿌리기 */
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		// 작은 인벤토리 피해서 물 뿌리자
		bool canWater = true;
		if (g_ptMouse.x > InventoryManager::GetSingleton()->GetSmallInvenRect().left && g_ptMouse.x < InventoryManager::GetSingleton()->GetSmallInvenRect().right
			&& g_ptMouse.y > InventoryManager::GetSingleton()->GetSmallInvenRect().top && g_ptMouse.y < InventoryManager::GetSingleton()->GetSmallInvenRect().bottom)
			canWater = false;

		// 클릭 위치 밭 인덱스
		int posX = (g_ptMouse.x + renderCoor.x) / F_TILESIZE;
		int posY = (g_ptMouse.y + renderCoor.y) / F_TILESIZE;

		// 밭 갈린 곳
		if (farmTileInfo[posX + FARM_TILE_X * posY].tileType == TileType::DIG)
		{
			// 체력 줄이자
			InventoryManager::GetSingleton()->SetEnergy(5);

			// 혼자
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 0 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 5;
			}

			// 상
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// 하
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 0 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// 좌
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 3;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// 우
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 1 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 3)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 6;
			}

			// 상 하
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 1 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 5;
			}

			// 좌 우
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 5;
			}

			// 상 우
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 1 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// 상 좌
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// 하 우
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 0 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// 하 좌
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 4)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 3;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 7;
			}

			// 상 우 하
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 2 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 0)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 0;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 0;
			}

			// 상 우 좌
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 2)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 2;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 1;
			}

			// 상 하 좌
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 1)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 1;
			}

			// 하 우 좌
			if (farmTileInfo[posX + FARM_TILE_X * posY].objFrameX == 3 &&
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY == 0)
			{
				farmTileInfo[posX + FARM_TILE_X * posY].tileType = TileType::WETDIG;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameX = 1;
				farmTileInfo[posX + FARM_TILE_X * posY].objFrameY = 0;
			}

			// 상 하 좌 우
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
	// 클릭
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		// 클릭 위치 밭 인덱스
		int posX = (g_ptMouse.x + renderCoor.x) / F_TILESIZE;
		int posY = (g_ptMouse.y + renderCoor.y) / F_TILESIZE;
			
		// 인벤토리 벤터 정보 가져오기 + 선택한 인덱스 위치
		vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
		int selectedIndex = InventoryManager::GetSingleton()->GetSelectedIndex();

		// 널이 아니면
		if ((*vInven)[selectedIndex] != NULL)
		{
			// 클릭 위치가 물뿌린곳 + 씨앗 없는 곳 이라면
			if (farmTileInfo[posX + FARM_TILE_X * posY].tileType == TileType::WETDIG
				&& farmTileInfo[posX + FARM_TILE_X * posY].seedType != SeedType::CROP)
			{
				// 곡식 타입으로 설정하기
				farmTileInfo[posX + FARM_TILE_X * posY].seedType = SeedType::CROP;

				// 땅에 렌더
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

				// 사용한 씨앗 개수 삭제
				(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount -= 1;
				if ((*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount <= 0)
				{
					// 0개면 해당값이 있던 벡터를 널로
					(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()] = NULL;
				}
			}
		}
	}
}

void FarmScene::testHarvest()
{
	vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
	// 우 클릭 한 위치가
	farmClicked.x = (winClicked.x + renderCoor.x) / F_TILESIZE;
	farmClicked.y = (winClicked.y + renderCoor.y) / F_TILESIZE;

	// 곡식이고 + 다 자랐으면(수확 가능 map에 있다) 수확하자
	if (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedType == SeedType::CROP
		&& checkDay.find(farmClicked.x + FARM_TILE_X * farmClicked.y) != checkDay.end())
	{
		// 어떤 곡식 확인 먼저 하고
		int mapKey = (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedFrameY - 1) / 2;

		// 씨앗 없다고 고쳐주고
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedType = SeedType::NONE;
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedFrameX = -1;
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedFrameY = -1;
		farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].day = -1;

		// map에서 지워주자
		checkDay.erase(farmClicked.x + FARM_TILE_X * farmClicked.y);

		// 인벤에 넣어주자
		string cropName = (storeData->GetMapCropData()->find(mapKey)->second->cropName) + "GROWN";
		bool alreadyExist = false;
		int existIndex;

		// 이미 있는가?
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

		// 없다
		if (alreadyExist == false)
		{
			// 새로생성
			int price = storeData->GetMapCropData()->find(mapKey)->second->price * 2;
			InventoryManager::GetSingleton()->SetAddInven(new INVEN_INFO(ObjectType::CROPGROWN, 1, mapKey, 0, price, cropName, 1));
		}
		// 있다
		else
		{
			// 기존 인벤토리 벡터에 있던 인덱스 위치에 있는 곡식 양 늘여주기 
			(*vInven)[existIndex]->amount += 1;
		}
	}
}

void FarmScene::testSale()
{
	// 인벤토리 벤터 정보 가져오기 + 선택한 인덱스 위치
	vector<INVEN_INFO*>* vInven = InventoryManager::GetSingleton()->GetVectorInven();
	int selectedIndex = InventoryManager::GetSingleton()->GetSelectedIndex();

	// 찬매 할 수 있는 상품이면
	if ((*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->salePrice != 0)
	{
		// 판매한 곡식 가격 겟
		int price = (*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->salePrice;
		InventoryManager::GetSingleton()->SetPlayerMoneyIncrease(price);

		// 판매한 곡식 개수 삭제
		(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount -= 1;
		if ((*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()]->amount <= 0)
		{
			// 0개면 해당값이 있던 벡터를 널로
			(*vInven)[InventoryManager::GetSingleton()->GetSelectedIndex()] = NULL;
		}
	}
}

void FarmScene::Update()
{
	player->Update();

	// 화면 이동 + 플레이어 충돌 체크
	canMove = true;
	Move();

	// 렌더 시작 위치
	renderCoor.x = player->GetPlayerImagePos().x - WINSIZE_X / 2;
	renderCoor.y = player->GetPlayerImagePos().y - WINSIZE_Y / 2;

	// 화면 이동 제어
	MoveControl();

	// 렌더 시작 인덱스
	startFrame.x = renderCoor.x / F_TILESIZE;
	startFrame.y = renderCoor.y / F_TILESIZE;

	/* 확인용 농사짓기 */
	//!InventoryManager::GetSingleton()->GetBigOpened() &&
	if (InventoryManager::GetSingleton()->GetSelectedItem().compare("Axe") == 0)
		testDigUp();
	else if (InventoryManager::GetSingleton()->GetSelectedItem().compare("Can") == 0)
		testWater();
	else if (InventoryManager::GetSingleton()->GetSeletecObjectType() == ObjectType::CROPS)
		testGrow();

	// 우 클릭 한 윈도우 상 위치가 
	winClicked.x = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * ((int)((g_ptMouse.x + renderCoor.x) / F_TILESIZE) - startFrame.x);
	winClicked.y = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * ((int)((g_ptMouse.y + renderCoor.y) / F_TILESIZE) - startFrame.y);

	// 우 클릭 한 인덱스 위치가
	farmClicked.x = (winClicked.x + renderCoor.x) / F_TILESIZE;
	farmClicked.y = (winClicked.y + renderCoor.y) / F_TILESIZE;

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
	{
		// 수확
		if(farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].seedType == SeedType::CROP)
			testHarvest();
		// 판매 -> CAVE 타일 안찍어 뒀다..
		else if ((farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 25 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 4)
			|| (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 25 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 5)
			|| (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 26 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 4)
			|| (farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameX == 26 && farmTileInfo[farmClicked.x + FARM_TILE_X * farmClicked.y].frameY == 5))
			testSale();
	}

	// 날짜가 흘렀으면
	DataManager::GetSingleton()->Update();
}

void FarmScene::Render(HDC hdc)
{
	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // 세로
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // 가로
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);

			// 농장 배경
			farmSample->FrameRender(hdc,
				tempPosX,
				tempPosY,
				farmTileInfo[tempIndex].frameX,
				farmTileInfo[tempIndex].frameY);

			// 밭 갈자 + 물 주자
			if (farmTileInfo[tempIndex].tileType == TileType::DIG
				|| farmTileInfo[tempIndex].tileType == TileType::WETDIG)
			{
				tilesetSample->FrameRender(hdc,
					tempPosX,
					tempPosY,
					farmTileInfo[tempIndex].objFrameX,
					farmTileInfo[tempIndex].objFrameY);
			}

			// 벽
			if (farmTileInfo[tempIndex].tileType == TileType::WALL || farmTileInfo[tempIndex].tileType == TileType::POND)
			{
				 //Rectangle(hdc, tempPosX, tempPosY,
					 //tempPosX + F_TILESIZE, tempPosY + F_TILESIZE);
			}

			//wsprintf(szText, "%d , %d", farmTileInfo[tempIndex].frameX, farmTileInfo[tempIndex].frameY);
			//TextOut(hdc, tempPosX, tempPosY, szText, strlen(szText));
		}
	}

	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // 세로
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // 가로
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);

			if (player->GetPlayerPos().y >= tempPosY - F_TILESIZE)
			{
				// 곡식 키우자
				if (farmTileInfo[tempIndex].seedType == SeedType::CROP)
				{
					int a = farmTileInfo[tempIndex].seedFrameX;
					int b = farmTileInfo[tempIndex].day;
					int seedFrameX = a + (InventoryManager::GetSingleton()->GetDay() - b);

					// 6일동안이 최대로 키운거 map에 저장
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

					// 곡식 윗부분도 그려주자 키우자
					growCrop->FrameRender(hdc,
						tempPosX,
						tempPosY - F_TILESIZE,
						seedFrameX,
						farmTileInfo[tempIndex].seedFrameY - 1);
				}
			}
		}
	}

	// 플레이어
	player->Render(hdc);

	for (int i = startFrame.y; i <= startFrame.y + WINSIZE_Y / F_TILESIZE + 1; i++)        // 세로
	{
		for (int j = startFrame.x; j <= startFrame.x + WINSIZE_X / F_TILESIZE + 1; j++)    // 가로
		{
			int tempIndex = i * FARM_TILE_X + j;
			int tempPosX = -((int)renderCoor.x % F_TILESIZE) + F_TILESIZE * (j - startFrame.x);
			int tempPosY = -((int)renderCoor.y % F_TILESIZE) + F_TILESIZE * (i - startFrame.y);

			if (player->GetPlayerPos().y < tempPosY - F_TILESIZE)
			{
				// 곡식 키우자
				if (farmTileInfo[tempIndex].seedType == SeedType::CROP)
				{
					int a = farmTileInfo[tempIndex].seedFrameX;
					int b = farmTileInfo[tempIndex].day;
					int seedFrameX = a + (InventoryManager::GetSingleton()->GetDay() - b);

					// 6일동안이 최대로 키운거 map에 저장
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

					// 곡식 윗부분도 그려주자 키우자
					growCrop->FrameRender(hdc,
						tempPosX,
						tempPosY - F_TILESIZE,
						seedFrameX,
						farmTileInfo[tempIndex].seedFrameY - 1);
				}
			}
		}
	}

	// 마우스 위치 땅
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

	// 인벤토리
	InventoryManager::GetSingleton()->Render(hdc);
}
