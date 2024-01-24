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

		//一定範囲内のポイントを見つける


		//フィルタリング
		for (auto& obj : m_filterArray)
		{
			pointArray = obj->GetPoint(pointArray);
		}

		//目的に合わせて評価値を付けて指定のインデックスを見つける
		KazMath::Vec2<int> point = pointArray.back();

		return point;
	}

private:

	std::array<std::unique_ptr<IFilter>, 4>m_filterArray;



};

