#pragma once
#include "Enemy.h"
#include "PatrolDraw.h"

class EnemyManager
{
private:
	std::vector<Enemy> m_enemys;
	std::vector<PatrolData> m_patrolDatas;
	std::shared_ptr<PatrolConfig> m_config;

	PatrolDraw m_patrolDraw;

public:
	EnemyManager(DrawingByRasterize& arg_rasterize);
	~EnemyManager();
	void Init();
	void Update();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);
};

