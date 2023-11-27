#pragma once
#include "Enemy.h"
#include "PatrolDraw.h"

class EnemyData;

class EnemyManager
{
private:
	std::vector<Enemy> m_enemys;
	std::vector<PatrolData> m_patrolDatas;
	std::shared_ptr<PatrolConfig> m_config;

	PatrolDraw m_patrolDraw;

public:
	EnemyManager();
	~EnemyManager();
	void Init();
	void Update(
		std::list<std::shared_ptr<MeshCollision>>
		arg_stageColliders,
		KazMath::Vec3<float> arg_playerPos);
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

public:
	void SetMapData(
		int arg_stageNum,
		std::list<EnemyData> arg_mapDatas,
		DrawingByRasterize& arg_rasterize);
};

