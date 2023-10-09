#include "EnemyManager.h"

EnemyManager::EnemyManager(DrawingByRasterize& arg_rasterize) :
	m_patrolData(arg_rasterize)
{
	m_enemys.resize(1);
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i].Init();
	}

	//����
	m_patrolData.Init();
}

void EnemyManager::Update()
{
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i].Update();
	}

	//����
	m_patrolData.Update();
}

void EnemyManager::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	//�G�`��
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i].Draw(
			arg_rasterize,
			arg_blasVec);
	}

	//����`��
	m_patrolData.Draw(
		arg_rasterize,
		arg_blasVec);
}
