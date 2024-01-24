#pragma once
#include"../KazLibrary/Math/KazMath.h"

class IFilter
{
public:
	IFilter() {};
	virtual ~IFilter() {};

	virtual std::vector<KazMath::Vec2<int>> GetPoint(std::vector<KazMath::Vec2<int>> arg_pointArray) = 0;
};


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
