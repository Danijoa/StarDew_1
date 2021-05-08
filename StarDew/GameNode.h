#pragma once
#include "config.h"

class GameNode
{
public:
	virtual HRESULT Init() { return E_FAIL; }	// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release() {}					// �޸� ����
	virtual void Update() {}					// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc) {}				// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	GameNode() {};
	virtual ~GameNode() {};
};

