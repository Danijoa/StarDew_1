#include "Player.h"
#include "Image.h"
#include "CollisionCheck.h"
#include "InventoryManager.h"

HRESULT Player::Init()
{
	// �÷��̾� �̹���
	ImageManager::GetSingleton()->AddImage("�÷��̾��̹���", "Image/GreyCat.bmp", 128 * 3, 256 * 3, 4, 8, true, RGB(255, 255, 255));
	player = ImageManager::GetSingleton()->FindImage("�÷��̾��̹���");

	// �÷��̾� ������ �� ��ġ
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y / 2;

	// �÷��̾� �� ��ġ
	playerRect = { (LONG)pos.x , (LONG)pos.y, (LONG)(pos.x + TILESIZE) , (LONG)(pos.y + TILESIZE) };

	// �÷��̾� Ÿ�� �ε���
	//playerIndex.x = pos.x / TILESIZE;
	//playerIndex.y = pos.y / TILESIZE;

	// �÷��̾� �ӵ�
	moveSpeed = 300.0f;
	
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
	// �÷��̾� ��ġ ���
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
	
	// �÷��̾� �� ��ġ ��Ʈ + �ε���
	playerRect = { (LONG)(pos.x + 30) , (LONG)(pos.y + 45), (LONG)(pos.x + 30 + TILESIZE) , (LONG)(pos.y + 45 + TILESIZE) };

	// ������
	if (!InventoryManager::GetSingleton()->GetBigOpened())
		Move();

	// �κ��丮 ������Ʈ
	InventoryManager::GetSingleton()->Update();
}

void Player::SetFuture(FPOINT playerFuturePos)
{
	playerFutureRect = { (LONG)(playerFuturePos.x + 30) , (LONG)(playerFuturePos.y + 45),
		(LONG)(playerFuturePos.x + 30 + (TILESIZE * 0.5)) , (LONG)(playerFuturePos.y + 45 + (TILESIZE * 0.5)) };
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

		// ����
		SetFuture({ pos.x, pos.y - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() });

		// ������
		if (canMove)
			imagePos = {imagePos.x, imagePos.y - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() };
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('S'))	// ��
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
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A'))	// ��
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
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))	// ��
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
