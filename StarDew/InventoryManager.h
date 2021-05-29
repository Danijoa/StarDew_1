#pragma once
#include "config.h"
#include "Singleton.h"

enum class ToolType {};
enum class CropType {};

class Image;
class SortManager;
class InventoryManager : public Singleton<InventoryManager>
{
private:
	// ��Ʈ
	Image* numFont;
	Image* dayFont;

	// �ð�
	Image* clock;
	int dayCnt;
	int timeCnt;
	int money;
	bool checkDayPass;

	// ������
	bool sleepDay;
	bool timeoutDay;
	bool energyoutDay;

	// ü��
	Image* energyBar;
	Image* energy;
	int playerEnergy;

	// ����
	static SortManager sortMgr;

	// �÷��̾� ���� �κ��丮
	Image* smallInven;
	RECT smallRect;
	bool smallInvenOpened;
	int sInvenX;	// �ε���
	string selectedItem;
	int selectedIndex;
	ObjectType selectedObjectType;

	// �÷��̾� ū �κ��丮
	Image* bigInven;
	RECT bigRect;
	bool bigInvenOpened;
	INT_PAIR bInven;	// �ε���
	int tabCnt;

	// ���
	Image* crops;

	// ��� ��
	Image* tools;

	// �κ��� ����� �̹��� �̸�, ������ ��ġ�� ��ƾ� �ϴ°ǰ�..
	vector<INVEN_INFO*> vInven;
	int vIndex;
	int size;
	INT_PAIR downIndex;
	INT_PAIR upIndex;

	// ��ġ �˷��ִ� ����,�ʷ� ��Ʈ
	HPEN hpen;
	HPEN hpenOld;

	// ��ũ�� ��� ����
	bool scrolledUp;
	bool scrolledDown;
	bool storeOpened;

	/* Ȯ�ο� */
	char szText[128] = "";
	float timeCheck;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// ��ũ��
	inline void SetScrolledUp(bool up) { this->scrolledUp = up; }
	inline bool GetScrolledUp() { return this->scrolledUp; }

	inline void SetStoreListOpen(bool opened) { this->storeOpened = opened; }

	inline void SetScrolledDown(bool down) { this->scrolledDown = down; }
	inline bool GetScrolledDown() { return this->scrolledDown; }

	// �κ��丮�� �ִ��� ����
	inline vector<INVEN_INFO*>* GetVectorInven() { return &(this->vInven); }

	// �κ��丮 �ֱ�
	inline int GetInvenIndex() { return this->vIndex; }
	inline void SetAddInven(INVEN_INFO* ivData) { this->vInven[vIndex] = ivData; }

	// ū �κ��丮 ���� ����
	inline bool GetBigOpened() { return this->bigInvenOpened; }

	// ���� �κ��丮 ũ��
	inline RECT GetSmallInvenRect() { return this->smallRect; }

	// �÷��̾ ������ ������
	inline string GetSelectedItem() { return this->selectedItem; }
	inline int GetSelectedIndex() { return this->selectedIndex; }
	inline ObjectType GetSeletecObjectType() { return this->selectedObjectType; }

	// ��
	inline int GetPlayerMoney() { return this->money; }
	inline void SetPlayerMoneyDecrease(int price) { this->money -= price; }

	// ��¥
	inline int GetDay() { return this->dayCnt; }
	inline void SetDay() { this->dayCnt += 1; }

	inline bool GetDayCheck() { return this->checkDayPass; }
	inline void SetDayCheck(bool dayPass) { this->checkDayPass = dayPass; }

	// ü��
	inline void SetEnergy(int used) { this->playerEnergy -= used; }
	inline void SetReEnergy() { this->playerEnergy = 213; }

	// ������
	inline void SetSleepDay(bool check) { this->sleepDay = check; }
	inline bool GetSleepDay() { return this->sleepDay; }

	inline void SetTimeoutDay(bool check) { this->timeoutDay = check; }
	inline bool GetTimeoutDay() { return this->timeoutDay; }

	inline void SetEnergyoutDay(bool check) { this->energyoutDay = check; }
	inline bool GetEnergyoutDay() { return this->energyoutDay; }

};

