#include "Player.h"
#include "Image.h"
#include "CollisionCheck.h"

HRESULT Player::Init()
{
	// 집 타일
	houseTileInfo = new TILE_INFO();

	// 플레이어 이미지
	ImageManager::GetSingleton()->AddImage("플레이어이미지", "Image/GreyCat.bmp", 128 * 3, 256 * 3, 4, 8, true, RGB(255, 255, 255));
	player = ImageManager::GetSingleton()->FindImage("플레이어이미지");

	// 플레이어 윈도우 상 위치
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y / 2;

	// 플레이어 발 위치
	playerRect = { (LONG)pos.x , (LONG)pos.y, (LONG)(pos.x + TILESIZE) , (LONG)(pos.y + TILESIZE) };

	// 플레이어 타일 인덱스
	playerIndex.x = pos.x / TILESIZE;
	playerIndex.y = pos.y / TILESIZE;

	// 플레이어 속도
	moveSpeed = 200.0f;
	
	// 플레이어 방향
	playerDir = 1;	// 하(아래)바라봄

	// 플레이어 이미지 프레임
	frameIndex.x = 0;
	frameIndex.y = 0;

	// 움직임 예측
	playerFuturePos = pos;

	// 움직임 가능 여부
	canMove = true;

	return S_OK;
}

void Player::Release()
{
}

void Player::Update()
{
	// 플레이어 발 위치
	playerRect = { (LONG)(pos.x + 25) , (LONG)(pos.y + 35), (LONG)(pos.x + 25 + TILESIZE * 0.7) , (LONG)(pos.y + 35 + TILESIZE * 0.7) };

	// 플레이어 타일 인덱스
	playerIndex.x = pos.x / TILESIZE;
	playerIndex.y = pos.y / TILESIZE;

	// 움직임
	Move();
}

void Player::SetFuture(FPOINT playerFuturePos)
{
	playerFutureRect = { (LONG)(playerFuturePos.x + 25) , (LONG)(playerFuturePos.y + 35),
		(LONG)(playerFuturePos.x + 25 + (TILESIZE * 0.7)) , (LONG)(playerFuturePos.y + 35 + (TILESIZE * 0.7)) };

	if (sceneNum == 1)	// 집
	{
		playerFutureRectIndex = { (int)(playerFutureRect.left - 300) / TILESIZE,
		   (int)(playerFutureRect.top + 45) / TILESIZE,
		   (int)(playerFutureRect.right - 300) / TILESIZE,
		   (int)(playerFutureRect.bottom + 45) / TILESIZE };
	}
}

void Player::Move()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// 상
	{
		// 방향
		playerDir = 0;

		// 프레임 변경
		frameIndex.y = 2;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		// 위치 예측
		playerFuturePos = { pos.x, pos.y - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
		SetFuture(playerFuturePos);

		// 충돌체크
		canMove = true;
		if (houseTileInfo[playerFutureRectIndex.leftIndex
			+ playerFutureRectIndex.topIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// left,top
		{
			canMove = false;
		}
		if(houseTileInfo[playerFutureRectIndex.rightIndex
			+ playerFutureRectIndex.topIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// right,top
		{
			canMove = false;
		}

		// 움직임
		if(canMove)
			pos.y -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// 하
	{
		playerDir = 1;

		frameIndex.y = 0;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		playerFuturePos = { pos.x, pos.y + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
		SetFuture(playerFuturePos);

		// 충돌체크
		canMove = true;
		if (houseTileInfo[playerFutureRectIndex.leftIndex
			+ playerFutureRectIndex.bottomIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// left,bottom
		{
			canMove = false;
		}
		if (houseTileInfo[playerFutureRectIndex.rightIndex
			+ playerFutureRectIndex.bottomIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// right,bottom
		{
			canMove = false;
		}

		if(canMove)
			pos.y += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// 좌
	{
		playerDir = 2;

		frameIndex.y = 3;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		playerFuturePos = { pos.x - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() , pos.y };
		SetFuture(playerFuturePos);

		// 충돌체크
		canMove = true;
		if (houseTileInfo[playerFutureRectIndex.leftIndex
			+ playerFutureRectIndex.topIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// left,top
		{
			canMove = false;
		}
		if (houseTileInfo[playerFutureRectIndex.leftIndex
			+ playerFutureRectIndex.bottomIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// left,bottom
		{
			canMove = false;
		}
		
		if (canMove)
			pos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// 우
	{
		playerDir = 3;

		frameIndex.y = 1;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		playerFuturePos = { pos.x + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() , pos.y };
		SetFuture(playerFuturePos);

		// 충돌체크
		canMove = true;
		if (houseTileInfo[playerFutureRectIndex.rightIndex
			+ playerFutureRectIndex.topIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// right,top
		{
			canMove = false;
		}
		if (houseTileInfo[playerFutureRectIndex.rightIndex
			+ playerFutureRectIndex.bottomIndex * HOUSE_TILE_X].tileType == TileType::WALL)	// right,bottom
		{
			canMove = false;
		}

		if (canMove)
			pos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}

}

void Player::Render(HDC hdc)
{
	if (player)
	{
		Rectangle(hdc, playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);
		player->FrameRender(hdc, pos.x, pos.y, frameIndex.x, frameIndex.y);
	}
}
