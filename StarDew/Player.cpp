#include "Player.h"
#include "Image.h"
#include "CollisionCheck.h"

HRESULT Player::Init()
{
	// �� Ÿ��
	houseTileInfo = new TILE_INFO();

	// �÷��̾� �̹���
	ImageManager::GetSingleton()->AddImage("�÷��̾��̹���", "Image/GreyCat.bmp", 128 * 3, 256 * 3, 4, 8, true, RGB(255, 255, 255));
	player = ImageManager::GetSingleton()->FindImage("�÷��̾��̹���");

	// �÷��̾� ������ �� ��ġ
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y / 2;

	// �÷��̾� �� ��ġ
	playerRect = { (LONG)pos.x , (LONG)pos.y, (LONG)(pos.x + TILESIZE) , (LONG)(pos.y + TILESIZE) };

	// �÷��̾� Ÿ�� �ε���
	playerIndex.x = pos.x / TILESIZE;
	playerIndex.y = pos.y / TILESIZE;

	// �÷��̾� �ӵ�
	moveSpeed = 200.0f;
	
	// �÷��̾� ����
	playerDir = 1;	// ��(�Ʒ�)�ٶ�

	// �÷��̾� �̹��� ������
	frameIndex.x = 0;
	frameIndex.y = 0;

	// ������ ����
	playerFuturePos = pos;

	// ������ ���� ����
	canMove = true;

	return S_OK;
}

void Player::Release()
{
}

void Player::Update()
{
	// �÷��̾� �� ��ġ
	playerRect = { (LONG)(pos.x + 25) , (LONG)(pos.y + 35), (LONG)(pos.x + 25 + TILESIZE * 0.7) , (LONG)(pos.y + 35 + TILESIZE * 0.7) };

	// �÷��̾� Ÿ�� �ε���
	playerIndex.x = pos.x / TILESIZE;
	playerIndex.y = pos.y / TILESIZE;

	// ������
	Move();
}

void Player::SetFuture(FPOINT playerFuturePos)
{
	playerFutureRect = { (LONG)(playerFuturePos.x + 25) , (LONG)(playerFuturePos.y + 35),
		(LONG)(playerFuturePos.x + 25 + (TILESIZE * 0.7)) , (LONG)(playerFuturePos.y + 35 + (TILESIZE * 0.7)) };

	if (sceneNum == 1)	// ��
	{
		playerFutureRectIndex = { (int)(playerFutureRect.left - 300) / TILESIZE,
		   (int)(playerFutureRect.top + 45) / TILESIZE,
		   (int)(playerFutureRect.right - 300) / TILESIZE,
		   (int)(playerFutureRect.bottom + 45) / TILESIZE };
	}
}

void Player::Move()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))			// ��
	{
		// ����
		playerDir = 0;

		// ������ ����
		frameIndex.y = 2;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		// ��ġ ����
		playerFuturePos = { pos.x, pos.y - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
		SetFuture(playerFuturePos);

		// �浹üũ
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

		// ������
		if(canMove)
			pos.y -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// ��
	{
		playerDir = 1;

		frameIndex.y = 0;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		playerFuturePos = { pos.x, pos.y + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
		SetFuture(playerFuturePos);

		// �浹üũ
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
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// ��
	{
		playerDir = 2;

		frameIndex.y = 3;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		playerFuturePos = { pos.x - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() , pos.y };
		SetFuture(playerFuturePos);

		// �浹üũ
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
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// ��
	{
		playerDir = 3;

		frameIndex.y = 1;
		frameIndex.x += 1;
		if (frameIndex.x == 3)
			frameIndex.x = 0;

		playerFuturePos = { pos.x + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() , pos.y };
		SetFuture(playerFuturePos);

		// �浹üũ
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
