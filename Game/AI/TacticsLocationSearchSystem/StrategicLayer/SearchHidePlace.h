#pragma once
#include"../IFilter.h"
#include"../Game/MapLoader/MapLoader.h"
#include <typeinfo>

class SearchHidePlace
{
public:
	SearchHidePlace() :MAP_SIZE(10, 10)
	{
		m_filterArray.emplace_back(std::make_unique<FirstFilter>());
		m_filterArray.emplace_back(std::make_unique<SecondFilter>());

		m_filterNameArray.emplace_back(typeid(m_filterArray[0]).name());
		m_filterNameArray.emplace_back(typeid(m_filterArray[1]).name());
	}

	KazMath::Vec2<int>GetPointIndex(const KazMath::Vec3<float>arg_pos)
	{
		std::vector<KazMath::Vec2<int>>pointArray;

		//���͈͓��̃|�C���g��������
		//���S���W������͈͂̃O���b�h�ԍ����A�N�Z�X���鏈���ɂ��邱�ƂŌy�ʉ���}��v�Z����
		KazMath::Vec2<int>playerXZ(arg_pos.ConvertVec2XZ().Int());
		playerXZ.x /= MapManager::GetMapSizeData(0).x;
		playerXZ.y /= MapManager::GetMapSizeData(0).y;
		//�͈͓��̃`�b�v�w��
		for (int x = -MAP_SIZE.x / 2; x < MAP_SIZE.x; ++x)
		{
			for (int y = -MAP_SIZE.y; y < MAP_SIZE.y; ++y)
			{
				pointArray.emplace_back(playerXZ.x + x, playerXZ.y + y);
			}
		}

		//�t�B���^�����O
		pointArray = FilterHelper::GetPoint(pointArray, m_filterArray, m_filterPointArray);

		//�ړI�ɍ��킹�ĕ]���l��t���Ďw��̃C���f�b�N�X��������
		KazMath::Vec2<int> point = pointArray.back();
		return point;
	}
	std::vector<std::string>m_filterNameArray;
private:

	std::vector<std::unique_ptr<IFilter>>m_filterArray;

	const KazMath::Vec2<int>MAP_SIZE;
	std::vector<std::vector<KazMath::Vec2<int>>>m_filterPointArray;


};

