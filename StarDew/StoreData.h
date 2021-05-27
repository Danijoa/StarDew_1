#pragma once
#include "GameNode.h"

typedef struct myCrop
{
	int mapIndex;
	string cropName;
	int price;

	myCrop(int mapIndex, string cropName, int price)
	{
		this->mapIndex = mapIndex;
		this->cropName = cropName;
		this->price = price;
	}
} CROP_DATA;

class Image;
class StoreData : public GameNode
{
private:
	Image* cropData;

	map<int, CROP_DATA*> mCropData;

	int listIndex;

	char szText[128] = "";

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	// ÀÎµ¦½º
	inline void SetListIndex(int idx) { this->listIndex = idx; }

	// °î¹° ¸Ê
	inline map<int, CROP_DATA*>* GetMapCropData() { return (&this->mCropData); }

	virtual ~StoreData() {};
};

