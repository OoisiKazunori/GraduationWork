#pragma once
#include "../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include <queue>
#include <tuple>

class EnemyPatrolData
{
private:
	enum struct AstarType
	{
		Move,
		UnMove,
		Root
	};

	struct AstarData
	{
		int cost = 1;
		AstarType type = AstarType::Move;
		KazMath::Transform3D trans;
	};

private:
	std::vector<std::vector<AstarData>> m_astarDatas;
	std::vector<std::vector<bool>> m_rootData;

	std::vector<std::vector<std::unique_ptr<
		BasicDraw::BasicModelRender>>> m_modelRenders;

	size_t m_size_x;
	size_t m_size_y;

	int m_start_x;
	int m_start_y;
	int m_goal_x;
	int m_goal_y;
	float m_chipSize;
	bool m_isCheck;

	bool m_isInput = false;

public:
	EnemyPatrolData(
		DrawingByRasterize& arg_rasterize);
	~EnemyPatrolData();
	void Init();
	void Update();
	void Input();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

public:
	std::vector<std::vector<AstarData>>&
		GetData() { return m_astarDatas; };
};