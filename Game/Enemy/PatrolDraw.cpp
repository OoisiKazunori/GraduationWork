#include "PatrolDraw.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"

PatrolDraw::PatrolDraw()
{
	m_baseColor = { 0,0,0,255 };
}

PatrolDraw::~PatrolDraw()
{
}

void PatrolDraw::SetData(
	DrawingByRasterize& arg_rasterize,
	std::weak_ptr<PatrolConfig> arg_config)
{
	m_patrolConfig = arg_config;

	size_t l_x = m_patrolConfig.lock()->GetSizeX();
	size_t l_y = m_patrolConfig.lock()->GetSizeY();
	//size_t l_x = 0;
	//size_t l_y = 0;

	m_modelRenders.reserve(l_x);
	m_modelRenders.resize(l_x);

	for (int i = 0; i < l_x; ++i)
	{
		m_modelRenders[i].reserve(l_y);
		m_modelRenders[i].resize(l_y);

		for (int j = 0; j < l_y; ++j)
		{
			//モデルデータ代入
			m_modelRenders[i][j] =
				std::make_unique<BasicDraw::BasicModelRender>(
					arg_rasterize,
					"Resource/cubeFrame/",
					"cubeFrame.gltf"
					);
		}
	}
}

void PatrolDraw::Init()
{
	/*for (int i = 0; i < m_patrolConfig.lock()->GetSizeX(); ++i) {
		for (int j = 0; j < m_patrolConfig.lock()->GetSizeY(); ++j) {
			m_patrolConfig.lock()->SetType(
				i, j,
				PatrolConfig::AstarType::Move);
		}
	}*/

	/*m_astarDatas[2][1].type = AstarType::UnMove;
	m_astarDatas[2][2].type = AstarType::UnMove;
	m_astarDatas[3][1].type = AstarType::UnMove;
	m_astarDatas[3][2].type = AstarType::UnMove;

	m_astarDatas[3][6].type = AstarType::UnMove;
	m_astarDatas[3][7].type = AstarType::UnMove;
	m_astarDatas[4][6].type = AstarType::UnMove;
	m_astarDatas[4][7].type = AstarType::UnMove;

	m_astarDatas[6][1].type = AstarType::UnMove;
	m_astarDatas[6][2].type = AstarType::UnMove;
	m_astarDatas[7][1].type = AstarType::UnMove;
	m_astarDatas[7][2].type = AstarType::UnMove;

	m_astarDatas[6][5].type = AstarType::UnMove;
	m_astarDatas[6][6].type = AstarType::UnMove;
	m_astarDatas[7][5].type = AstarType::UnMove;
	m_astarDatas[7][6].type = AstarType::UnMove;

	m_astarDatas[2][4].type = AstarType::UnMove;
	m_astarDatas[3][4].type = AstarType::UnMove;
	m_astarDatas[4][4].type = AstarType::UnMove;*/
}

void PatrolDraw::Update()
{
	//Input
	Input();
}

void PatrolDraw::Input()
{
	//仮
	//if (KeyBoradInputManager::
	//	Instance()->InputTrigger(DIK_Q))
	//{
	//	//Init();
	//	m_isCheck = true;
	//	m_isInput = true;
	//}
	//if (KeyBoradInputManager::
	//	Instance()->InputTrigger(DIK_R))
	//{
	//	//Init();
	//	m_isInput = false;
	//}

	/*if (!m_isInput)
	{
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_LEFT))
		{
			int l_sub = l_base_goal_x - 1;
			if (l_sub < 0) {
				return;
			}
			if (m_astarDatas[l_sub][l_base_goal_y].type ==
				AstarType::Move) {
				l_base_goal_x--;
			}
		}
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_RIGHT))
		{
			int l_sub = l_base_goal_x + 1;
			if (l_sub >= m_size_x) {
				return;
			}
			if (m_astarDatas[l_sub][l_base_goal_y].type ==
				AstarType::Move) {
				l_base_goal_x++;
			}
		}
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_DOWN))
		{
			int l_sub = l_base_goal_y - 1;
			if (l_sub < 0) {
				return;
			}
			if (m_astarDatas[l_base_goal_x][l_sub].type ==
				AstarType::Move) {
				l_base_goal_y--;
			}
		}
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_UP))
		{
			int l_sub = l_base_goal_y + 1;
			if (l_sub >= m_size_y) {
				return;
			}
			if (m_astarDatas[l_base_goal_x][l_sub].type ==
				AstarType::Move) {
				l_base_goal_y++;
			}
		}
	}*/
}

void PatrolDraw::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	for (int i = 0; i < m_patrolConfig.lock()->GetSizeX(); ++i)
	{
		for (int j = 0; j < m_patrolConfig.lock()->GetSizeY(); ++j)
		{
			if (i != 0 &&
				i != m_patrolConfig.lock()->GetSizeX() - 1)
			{
				if (j != 0 &&
					j != m_patrolConfig.lock()->GetSizeY() - 1)
				{
					continue;
				}
			}

			KazMath::Color l_color = m_baseColor;

			if (m_patrolConfig.lock()->GetData(i, j).type ==
				PatrolConfig::AstarType::UnMove) {
				continue;
			}
			else if (m_patrolConfig.lock()->GetData(i, j).type ==
				PatrolConfig::AstarType::Root) {
				l_color =
					m_patrolConfig.lock()->GetData(i, j).color;
			}

			m_modelRenders[i][j]->m_model.Draw(
				arg_rasterize,
				arg_blasVec,
				m_patrolConfig.lock()->GetData(i, j).trans,
				l_color);
		}
	}
}
