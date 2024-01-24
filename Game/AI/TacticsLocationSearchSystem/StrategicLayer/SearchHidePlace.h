#pragma once
#include"../IFilter.h"

class SearchHidePlace
{
public:
	SearchHidePlace()
	{
		m_filterArray[0] = std::make_unique<FirstFilter>();
		m_filterArray[1] = std::make_unique<SecondFilter>();
	}
	KazMath::Vec2<int>GetPointIndex(const KazMath::Vec3<float>arg_pos)
	{
		std::vector<KazMath::Vec2<int>>pointArray;

		//���͈͓��̃|�C���g��������


		//�t�B���^�����O
		for (auto& obj : m_filterArray)
		{
			pointArray = obj->GetPoint(pointArray);
		}

		//�ړI�ɍ��킹�ĕ]���l��t���Ďw��̃C���f�b�N�X��������
		KazMath::Vec2<int> point = pointArray.back();

		return point;
	}

private:

	std::array<std::unique_ptr<IFilter>, 4>m_filterArray;



};

