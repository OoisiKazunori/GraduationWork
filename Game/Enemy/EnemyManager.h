#pragma once
#include "Enemy.h"
#include "PatrolDraw.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include "../Game/Collision/MeshCollision.h"

class EnemyData;

class EnemyManager
{
private:
	std::vector<std::unique_ptr<Enemy>> m_enemys;
	std::vector<PatrolData> m_patrolDatas;
	std::shared_ptr<PatrolConfig> m_config;

	PatrolDraw m_patrolDraw;

	SoundData m_checkSound;

public:
	EnemyManager();
	~EnemyManager();
	void Init(
		DrawingByRasterize& arg_rasterize);
	void Update(
		std::list<std::shared_ptr<MeshCollision>>
		arg_stageColliders,
		std::weak_ptr<BulletMgr> arg_bulletMgr,
		KazMath::Transform3D arg_playerTransform,
		std::weak_ptr<MeshCollision> arg_stageMeshCollision);
	void Draw(
		DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);

public:
	void ClearEnemyData() { m_enemys.clear(); }

	void SetModelData(
		DrawingByRasterize& arg_rasterize);

	void AddEnemyData(
		int arg_enemyNum,
		std::list<KazMath::Transform3D> arg_enemyList);

	int GetCombatStatusEnemyCount();

};

