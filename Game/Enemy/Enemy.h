#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include "PatrolData.h"

class Enemy
{
private:
	enum struct State
	{
		Patrol,
		Alert,
		Combat,
		Holdup,
		Death
	};

public:
	std::shared_ptr<
		BasicDraw::BasicModelRender> m_enemyBox;
	std::vector<std::pair<float, float>> m_rootPos;
	std::vector<std::pair<int, int>> m_checkPointDelay;
	KazMath::Transform3D m_trans;
	State m_status;
	int m_delayNum;
	int m_count;
	int m_delay;
	bool m_isCheckPoint;

public:
	Enemy();
	~Enemy();
	void Init();
	void Update();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

public:
	void SetData(
		DrawingByRasterize& arg_rasterize);
	void SetCheckPointDelay(
		std::vector<std::pair<int, int>> arg_checkPointDelay);

public:
	void SetRootPos(
		std::vector<std::pair<float, float>> arg_rootPos) {
		m_rootPos = arg_rootPos;
	}
};
