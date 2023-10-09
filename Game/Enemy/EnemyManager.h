#pragma once
#include "Enemy.h"

class EnemyManager
{
private:
	std::vector<Enemy> m_enemys;
	EnemyPatrolData m_patrolData;

public:
	EnemyManager(DrawingByRasterize& arg_rasterize);
	~EnemyManager();
	void Init();
	void Update();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);
};

