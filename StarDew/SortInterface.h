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
		// �����ϴ� ������ ũ�Ⱑ 0 �Ǵ� 1�� �Ǿ���
		if (start >= end)						
			return;

		// �Ǻ��� ù��° ���ҷ� ����
		if ((*vInven)[start] == NULL)
			pivotOrderNum = MAX;
		else
			pivotOrderNum = (*vInven)[start]->orderNum;	
		// ������ �ι�° ��ġ
		leftIndex = start + 1;						
		// ������ ������ ��ġ
		rightIndex = end;							

		// ���� ����
		while (leftIndex <= rightIndex)
		{
			//�������� �� ĭ�� ����
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

			// ���������� ��ĭ�� ����
			if ((*vInven)[leftIndex] == NULL)
				leftOrderNum = MAX;
			else
				leftOrderNum = (*vInven)[leftIndex]->orderNum;

			while (leftOrderNum <= pivotOrderNum)
			{
				leftIndex += 1;

				if (leftIndex == 36)	// ���� ����� Ż��
					break;

				if ((*vInven)[leftIndex] == NULL)
					leftOrderNum = MAX;
				else
					leftOrderNum = (*vInven)[leftIndex]->orderNum;
			}
			
			// ������ ���� ���� ��ȯ
			if (leftIndex < rightIndex)
			{
				INVEN_INFO* temp = (*vInven)[rightIndex];
				(*vInven)[rightIndex] = (*vInven)[leftIndex];
				(*vInven)[leftIndex] = temp;
			}
		}

		// ������ ���ҿ� �Ǻ� ���� ��ȯ
		INVEN_INFO* temp = (*vInven)[rightIndex];
		(*vInven)[rightIndex] = (*vInven)[start];
		(*vInven)[start] = temp;

		// ��ȭ�� �Ǻ� ���� ���� �˻�
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
		// ��ü ���鼭 �˻� �Ұ���
		for (int i = 1; i < 36; i++)
		{
			int k = i;
			// ������ҿ�(k) �� ������ ���� ���� �ϳ��� �����ϸ鼭 �˻�
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

				// ��ȯ
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