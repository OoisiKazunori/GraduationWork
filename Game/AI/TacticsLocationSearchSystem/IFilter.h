#pragma once
#include"../KazLibrary/Math/KazMath.h"

class IFilter
{
public:
	IFilter() {};
	virtual ~IFilter() {};

	virtual std::vector<KazMath::Vec2<int>> GetPoint(std::vector<KazMath::Vec2<int>> arg_pointArray) = 0;
};

namespace FilterHelper
{
	inline std::vector<KazMath::Vec2<int>> GetPoint(
		std::vector<KazMath::Vec2<int>>arg_pointIndexArray,
		std::vector<std::unique_ptr<IFilter>>&arg_filter,
		std::vector<std::vector<KazMath::Vec2<int>>>&arg_debugFilterPoint
	)
	{
		for (auto& obj : arg_filter)
		{
			arg_debugFilterPoint.emplace_back(arg_pointIndexArray);
			arg_pointIndexArray = obj->GetPoint(arg_pointIndexArray);
		}
		return arg_pointIndexArray;
	}
}

class FirstFilter:public IFilter
{
public:

	std::vector<KazMath::Vec2<int>> GetPoint(std::vector<KazMath::Vec2<int>> arg_pointArray)
	{
		return std::vector<KazMath::Vec2<int>>();
	};

};

class SecondFilter :public IFilter
{
public:

	std::vector<KazMath::Vec2<int>> GetPoint(std::vector<KazMath::Vec2<int>> arg_pointArray)
	{
		return std::vector<KazMath::Vec2<int>>();
	}

};
