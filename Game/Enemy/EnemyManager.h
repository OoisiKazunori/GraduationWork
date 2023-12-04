#pragma once
#include "Enemy.h"
#include "PatrolDraw.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include "../Game/Collision/MeshCollision.h"

class EnemyData;

class EnemyManager
{
private:
	std::vector<Enemy> m_enemys;
	std::vector<PatrolData> m_patrolDatas;
	std::shared_ptr<PatrolConfig> m_config;

	PatrolDraw m_patrolDraw;

	SoundData m_checkSound;

public:
	EnemyManager();
	~EnemyManager();
	void Init();
	void Update(
		std::list<std::shared_ptr<MeshCollision>>
		arg_stageColliders,
		std::weak_ptr<BulletMgr> arg_bulletMgr,
		KazMath::Vec3<float> arg_playerPos,
		std::weak_ptr<MeshCollision> arg_stageMeshCollision);
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

public:
	void SetMapData(
		int arg_stageNum,
		std::list<EnemyData> arg_mapDatas,
		DrawingByRasterize& arg_rasterize);
};

