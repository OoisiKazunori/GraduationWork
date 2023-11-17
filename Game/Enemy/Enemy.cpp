#include "Enemy.h"
#include "EnemyConfig.h"

Enemy::Enemy()
{
	//m_trans.scale /= 2;
	m_status = State::Patrol;
	m_isCheckPoint = false;
	m_delayNum = 0;
	m_count = 0;
	m_delay = 0;
}

Enemy::~Enemy()
{
}

void Enemy::SetData(
	DrawingByRasterize& arg_rasterize)
{
	//モデルデータ代入
	m_enemyBox =
		std::make_unique<BasicDraw::BasicModelRender>(
			arg_rasterize,
			"Resource/cubeFrame/",
			"cubeFrame.gltf"
			);
}

void Enemy::SetCheckPointDelay(
	std::vector<std::pair<int, int>> arg_checkPointDelay)
{
	m_checkPointDelay = arg_checkPointDelay;
	for (int i = 0; i < m_checkPointDelay.size(); ++i)
	{
		m_checkPointDelay[i].second = EnemyConfig::delay;
	}
}

void Enemy::Init()
{
}

void Enemy::Update()
{
	if (m_isCheckPoint)
	{
		m_delay++;
		if (m_delay == 
			m_checkPointDelay[m_delayNum].second)
		{
			m_delay = 0;
			m_isCheckPoint = false;
		}
	}
	else if (m_rootPos.size() > 0)
	{
		m_trans.pos = {
			m_rootPos[m_count].first,
			0.0f,
			m_rootPos[m_count].second
		};
		if (m_count < m_rootPos.size() - 1) {
			m_count++;
		}
		else {
			m_count = 0;
		}

		for (int i = 0; i < m_checkPointDelay.size(); ++i)
		{
			if (m_count != m_checkPointDelay[i].first) {
				continue;
			}
			m_delayNum = i;
			m_isCheckPoint = true;
			break;
		}
	}
}

void Enemy::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	if (m_rootPos.size() > 0)
	{
		KazMath::Color l_player = { 0, 255, 255,255 };

		m_enemyBox->m_model.Draw(
			arg_rasterize,
			arg_blasVec,
			m_trans,
			l_player);
	}
}
