#include "Player.h"
#include "Image.h"
#include "CollisionCheck.h"
#include "InventoryManager.h"

HRESULT Player::Init()
{
	// 플레이어 이미지
	ImageManager::GetSingleton()->AddImage("플레이어이미지", "Image/GreyCat.bmp", 128 * 3, 256 * 3, 4, 8, true, RGB(255, 255, 255));
	player = ImageManager::GetSingleton()->FindImage("플레이어이미지");

	// 플레이어 윈도우 상 위치
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y / 2;

	// 플레이어 발 위치
	playerRect = { (LONG)pos.x , (LONG)pos.y, (LONG)(pos.x + TILESIZE) , (LONG)(pos.y + TILESIZE) };

	// 플레이어 타일 인덱스
	//playerIndex.x = pos.x / TILESIZE;
	//playerIndex.y = pos.y / TILESIZE;

	// 플레이어 속도
	moveSpeed = 300.0f;
	
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
	// 플레이어 위치 잡기
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y / 2;
	if (imagePos.x < WINSIZE_X / 2)
		pos.x = imagePos.x;
	if (imagePos.x > curImageSize.x - WINSIZE_X / 2)
		pos.x = imagePos.x - curImageSize.x + WINSIZE_X;
	if (imagePos.y < WINSIZE_Y / 2)
		pos.y = imagePos.y;
	if (imagePos.y > curImageSize.y - WINSIZE_Y / 2)
		pos.y = imagePos.y - curImageSize.y + WINSIZE_Y;
	
	// 플레이어 발 위치 렉트 + 인덱스
	playerRect = { (LONG)(pos.x + 30) , (LONG)(pos.y + 45), (LONG)(pos.x + 30 + TILESIZE) , (LONG)(pos.y + 45 + TILESIZE) };

	// 움직임
	if (!InventoryManager::GetSingleton()->GetBigOpened())
		Move();

	// 인벤토리 업데이트
	InventoryManager::GetSingleton()->Update();
}

void Player::SetFuture(FPOINT playerFuturePos)
{
	playerFutureRect = { (LONG)(playerFuturePos.x + 30) , (LONG)(playerFuturePos.y + 45),
		(LONG)(playerFuturePos.x + 30 + (TILESIZE * 0.5)) , (LONG)(playerFuturePos.y + 45 + (TILESIZE * 0.5)) };
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

		// 예측
		SetFuture({ pos.x, pos.y - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() });

		// 움직임
		if (canMove)
			imagePos = {imagePos.x, imagePos.y - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// 하
	{
		playerDir = 1;

		frameIndex.y = 0;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		SetFuture({ pos.x, pos.y + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() });

		if (canMove)
			imagePos = { imagePos.x, imagePos.y + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// 좌
	{
		playerDir = 2;

		frameIndex.y = 3;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		SetFuture({ pos.x - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime(), pos.y });

		if (canMove)
			imagePos = { imagePos.x - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime(), imagePos.y };
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// 우
	{
		playerDir = 3;

		frameIndex.y = 1;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		SetFuture({ pos.x + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime(), pos.y });

		if (canMove)
			imagePos = { imagePos.x + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime(), imagePos.y };
	}
}

void Player::Render(HDC hdc)
{
	if (player)
	{		
		player->FrameRender(hdc, pos.x, pos.y, frameIndex.x, frameIndex.y);
		//Rectangle(hdc, playerRect.left, playerRect.top , playerRect.right, playerRect.bottom);
		//InventoryManager::GetSingleton()->Render(hdc);
	}
}
