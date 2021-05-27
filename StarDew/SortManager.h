#pragma once
#include "config.h"
#include "SortInterface.h"

class SortManager
{
private:
	SortInterface* sort;

public:
	virtual void ChangeSort(SortInterface* changeSort)
	{
		if (sort)
			delete sort;
		sort = changeSort;
	}

	void DoSort(vector<INVEN_INFO*>* vInven)
	{
		if (sort)
			sort->DoSort(vInven);
	}

	SortManager() : sort(nullptr) {};
	~SortManager()
	{
		if (sort)
			delete sort;
		sort = nullptr;
	}
};

