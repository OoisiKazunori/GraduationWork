#include "EnemyManager.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include"../MapLoader/MapLoader.h"

EnemyManager::EnemyManager()
{
	m_checkSound =
		SoundManager::Instance()->SoundLoadWave("Resource/Sound/Discovery.wav");
	m_checkSound.volume = 0.05f;
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(
	DrawingByRasterize& arg_rasterize)
{
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		//m_enemys[i]->Init();
	}

	//”»’è
	//m_patrolDatas[0].SetColor(KazMath::Color(255, 255, 255, 255));
	//m_patrolDraw.Init();
}

void EnemyManager::SetModelData(
	DrawingByRasterize& arg_rasterize)
{
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i]->SetData(arg_rasterize);
	}
}

void EnemyManager::AddEnemyData(
	int arg_enemyNum,
	std::list<KazMath::Transform3D> arg_enemyList)
{
	m_enemys.emplace_back(std::make_unique<Enemy>());
	m_enemys[arg_enemyNum]->Init(arg_enemyList);
}

int EnemyManager::GetCombatStatusEnemyCount()
{
	int count = 0;
	for (auto& index : m_enemys) {
		if (!index->IsCombat()) continue;
		++count;
	}
	return count;
}

void EnemyManager::Update(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Transform3D arg_playerTransform,
	std::weak_ptr<MeshCollision> arg_stageMeshCollision,
	HPUI& arg_hpUI)
{
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i]->CheckInEcho(arg_stageMeshCollision);
		m_enemys[i]->Update(
			arg_stageColliders,
			arg_bulletMgr,
			arg_playerTransform,
			arg_stageMeshCollision,
			arg_hpUI);

		//”­Œ©Žž
		if (m_enemys[i]->IsDiscovery()) {
			SoundManager::Instance()->
				SoundPlayerWave(m_checkSound, 0);
		}
	}

	//”»’è
	//m_config->Update();
	//m_patrolDraw.Update();
}

void EnemyManager::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	//“G•`‰æ
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i]->Draw(
			arg_rasterize,
			arg_blasVec);
	}

	//”»’è•`‰æ
	/*m_patrolDraw.Draw(
		arg_rasterize,
		arg_blasVec);*/
}
