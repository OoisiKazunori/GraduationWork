#include "EnemyManager.h"

EnemyManager::EnemyManager(DrawingByRasterize& arg_rasterize)
{
	m_config = std::make_shared<PatrolConfig>(10, 10, 2.0f);

	int l_eNum = 3;
	m_enemys.resize(l_eNum);
	m_patrolDatas.resize(l_eNum);
	for (int i = 0; i < l_eNum; ++i)
	{
		m_enemys[i].SetData(arg_rasterize);
		m_patrolDatas[i].SetData(m_config);
	}

	m_patrolDraw.SetData(arg_rasterize, m_config);
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	//“G‰Šú‰»
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i].Init();
		m_patrolDatas[i].Init();
	}

	//ŒŸõƒ|ƒCƒ“ƒgˆÊ’u
	m_patrolDatas[0].AddCheckPoint(0, 0);
	m_patrolDatas[0].AddCheckPoint(0, 4);
	//m_patrolDatas[0].AddCheckPoint(2, 6);
	//m_patrolDatas[0].AddCheckPoint(2, 2);
	//m_patrolDatas[0].AddCheckPoint(2, 0);

	//m_patrolData[0].AddCheckPoint(0, 0);
	//m_patrolData[0].AddCheckPoint(0, 2);
	//m_patrolData[0].AddCheckPoint(2, 2);
	//m_patrolData[0].AddCheckPoint(2, 0);
	//m_patrolData[0].AddCheckPoint(9, 3);

	m_patrolDatas[1].AddCheckPoint(4, 4);
	m_patrolDatas[1].AddCheckPoint(6, 4);
	m_patrolDatas[1].AddCheckPoint(6, 1);
	m_patrolDatas[1].AddCheckPoint(5, 0);
	//m_patrolData[1].AddCheckPoint(3, 7);

	m_patrolDatas[2].AddCheckPoint(7, 6);
	m_patrolDatas[2].AddCheckPoint(7, 9);
	//m_patrolDatas[2].AddCheckPoint(9, 5);
	//m_patrolDatas[2].AddCheckPoint(7, 2);
	//m_patrolData[2].AddCheckPoint(7, 9);

	m_patrolDatas[0].SetColor(KazMath::Color(255, 255, 255, 255));
	m_patrolDatas[1].SetColor(KazMath::Color(255, 255, 0, 255));
	m_patrolDatas[2].SetColor(KazMath::Color(0, 255, 0, 255));

	//”»’è
	m_patrolDraw.Init();
}

void EnemyManager::Update()
{
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_patrolDatas[i].Update();
		m_enemys[i].SetRootPos(m_patrolDatas[i].GetRootPos());
		m_enemys[i].SetCheckPointDelay(
			m_patrolDatas[i].GetCheckPointDelay());
		m_enemys[i].Update();
	}

	//ˆÚ“®
	//m_patrolDraw.SetRootPos(m_patrolData[0].GetRootPos());

	//”»’è
	m_patrolDraw.Update();
}

void EnemyManager::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	//“G•`‰æ
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i].Draw(
			arg_rasterize,
			arg_blasVec);
	}

	//”»’è•`‰æ
	m_patrolDraw.Draw(
		arg_rasterize,
		arg_blasVec);
}
