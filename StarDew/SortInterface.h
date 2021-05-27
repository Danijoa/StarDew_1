#pragma once
#include "config.h"
#define MAX 987654321

class SortInterface
{
protected:

public:
	virtual void DoSort(vector<INVEN_INFO*>* vInven) = 0;
};

class QuickSort : public SortInterface 
{
private:
	int pivotOrderNum;
	int leftIndex;
	int rightIndex;

	int leftOrderNum;
	int rightOrderNum;

public:
	void solve(vector<INVEN_INFO*>* vInven, int start, int end)
	{
		// 정렬하는 벡터의 크기가 0 또는 1이 되었다
		if (start >= end)						
			return;

		// 피봇을 첫번째 원소로 잡자
		if ((*vInven)[start] == NULL)
			pivotOrderNum = MAX;
		else
			pivotOrderNum = (*vInven)[start]->orderNum;	
		// 벡터의 두번째 위치
		leftIndex = start + 1;						
		// 벡터의 마지막 위치
		rightIndex = end;							

		// 정렬 하자
		while (leftIndex <= rightIndex)
		{
			//왼쪽으로 한 칸씩 후진
			if ((*vInven)[rightIndex] == NULL)
				rightOrderNum = MAX;
			else
				rightOrderNum = (*vInven)[rightIndex]->orderNum;

			while (rightOrderNum > pivotOrderNum)
			{
				rightIndex -= 1;

				if ((*vInven)[rightIndex] == NULL)
					rightOrderNum = MAX;
				else
					rightOrderNum = (*vInven)[rightIndex]->orderNum;
			}

			// 오른쪽으로 한칸씩 전진
			if ((*vInven)[leftIndex] == NULL)
				leftOrderNum = MAX;
			else
				leftOrderNum = (*vInven)[leftIndex]->orderNum;

			while (leftOrderNum <= pivotOrderNum)
			{
				leftIndex += 1;

				if (leftIndex == 36)	// 범위 벗어나면 탈출
					break;

				if ((*vInven)[leftIndex] == NULL)
					leftOrderNum = MAX;
				else
					leftOrderNum = (*vInven)[leftIndex]->orderNum;
			}
			
			// 오른쪽 왼쪽 원소 교환
			if (leftIndex < rightIndex)
			{
				INVEN_INFO* temp = (*vInven)[rightIndex];
				(*vInven)[rightIndex] = (*vInven)[leftIndex];
				(*vInven)[leftIndex] = temp;
			}
		}

		// 오른쪽 원소와 피봇 원소 교환
		INVEN_INFO* temp = (*vInven)[rightIndex];
		(*vInven)[rightIndex] = (*vInven)[start];
		(*vInven)[start] = temp;

		// 변화된 피봇 양쪽 정렬 검사
		solve(vInven, start, rightIndex - 1);
		solve(vInven, rightIndex + 1, end);
	}

	void DoSort(vector<INVEN_INFO*>* vInven)
	{
		solve(vInven, 0, 36 - 1);
	}
};

class SelectionSort : public SortInterface
{
private:
	int iOrderNum;
	int kOrderNum;
	int jOrderNum;

public:
	void DoSort(vector<INVEN_INFO*>* vInven)
	{
		for (int i = 0; i < 36 - 1; i++)
		{
			if ((*vInven)[i] == NULL)
				iOrderNum = MAX;
			else
				iOrderNum = (*vInven)[i]->orderNum;

			int k = i + 1;
			if ((*vInven)[k] == NULL)
				kOrderNum = MAX;
			else
				kOrderNum = (*vInven)[k]->orderNum;

			for (int j = i + 1; j < 36; j++)
			{
				if ((*vInven)[j] == NULL)
					jOrderNum = MAX;
				else
					jOrderNum = (*vInven)[j]->orderNum;

				if (kOrderNum > jOrderNum)
				{
					k = j;
				}

				if ((*vInven)[k] == NULL)
					kOrderNum = MAX;
				else
					kOrderNum = (*vInven)[k]->orderNum;
			}

			if (iOrderNum > kOrderNum)
			{
				INVEN_INFO* temp = (*vInven)[i];
				(*vInven)[i] = (*vInven)[k];
				(*vInven)[k] = temp;
			}
		}
	}
};

//class InsertionSort : public SortInterface
//{
//private:
//	int iOrderNum;
//	int kOrderNum;
//	int jOrderNum;
//
//public:
//	void DoSort(vector<INVEN_INFO*>* vInven)
//	{
//		for (int i = 1; i < 36; i++)
//		{
//			int k = i;
//			for (int j = i - 1; j >= 0; j--)
//			{
//				if ((*vInven)[k] == NULL)
//					kOrderNum = MAX;
//				else
//					kOrderNum = (*vInven)[k]->orderNum;
//
//				if ((*vInven)[j] == NULL)
//					jOrderNum = MAX;
//				else
//					jOrderNum = (*vInven)[j]->orderNum;
//
//				if (kOrderNum < jOrderNum)
//				{
//					INVEN_INFO* temp = (*vInven)[k];
//					(*vInven)[k] = (*vInven)[j];
//					(*vInven)[j] = temp;
//
//					k = j;
//				}
//				else
//					break;
//			}
//		}
//	}
//};

class InsertionSort : public SortInterface
{
private:
	string iProductName;
	string kProductName;
	string jProductName;

public:
	void DoSort(vector<INVEN_INFO*>* vInven)
	{
		// 전체 돌면서 검사 할거임
		for (int i = 1; i < 36; i++)
		{
			int k = i;
			// 현재원소와(k) 그 앞쪽의 원소 값들 하나씩 후진하면서 검사
			for (int j = i - 1; j >= 0; j--)
			{
				if ((*vInven)[k] == NULL)
					kProductName = "Z";
				else
					kProductName = (*vInven)[k]->productName.at(0);

				if ((*vInven)[j] == NULL)
					jProductName = "Z";
				else
					jProductName = (*vInven)[j]->productName.at(0);

				// 교환
				if (kProductName < jProductName)
				{
					INVEN_INFO* temp = (*vInven)[k];
					(*vInven)[k] = (*vInven)[j];
					(*vInven)[j] = temp;

					k = j;
				}
				else
					break;
			}
		}
	}
};