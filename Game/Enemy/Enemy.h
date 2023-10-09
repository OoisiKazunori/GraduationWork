#pragma once
#include "EnemyPatrolData.h"

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
	Enemy();
	~Enemy();
	void Init();
	void Update();
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);
};

