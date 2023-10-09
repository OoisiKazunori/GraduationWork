#include "EnemyPatrolData.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"

EnemyPatrolData::EnemyPatrolData(
	DrawingByRasterize& arg_rasterize)
{
	//マップデータ
	m_size_x = 10;
	m_size_y = 10;
	m_chipSize = 2;

	//検索初期位置
	m_start_x = 0;
	m_start_y = 0;
	m_goal_x = 5;
	m_goal_y = 9;

	//初期化
	m_isCheck = false;

	//リサイズ
	m_astarDatas.resize(m_size_x);
	m_rootData.resize(m_size_x);

	m_modelRenders.reserve(m_size_x);
	m_modelRenders.resize(m_size_x);

	for (int i = 0; i < m_size_x; ++i)
	{
		m_astarDatas[i].resize(m_size_y);
		m_rootData[i].resize(m_size_y);

		m_modelRenders[i].reserve(m_size_y);
		m_modelRenders[i].resize(m_size_y);

		for (int j = 0; j < m_size_y; ++j)
		{
			//モデルデータ代入
			m_modelRenders[i][j] = std::make_unique<BasicDraw::BasicModelRender>(
				arg_rasterize, "Resource/cubeFrame/", "cubeFrame.gltf"
				);

			//箱トランスフォーム
			float l_multiPos = 2.5f;
			m_astarDatas[i][j].trans.pos = {
				i * m_chipSize * l_multiPos,
				0,
				j * m_chipSize * l_multiPos
			};
			m_astarDatas[i][j].trans.scale = {
				m_chipSize,
				m_chipSize,
				m_chipSize
			};
		}
	}
}

EnemyPatrolData::~EnemyPatrolData()
{
}

void EnemyPatrolData::Init()
{
	for (int i = 0; i < m_size_x; ++i)
	{
		for (int j = 0; j < m_size_y; ++j)
		{
			m_astarDatas[i][j].type = AstarType::Move;
			m_rootData[i][j] = false;
		}
	}

	//m_astarDatas[0][0].type = AstarType::UnMove;
	m_astarDatas[2][1].type = AstarType::UnMove;
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
	m_astarDatas[4][4].type = AstarType::UnMove;
}

void EnemyPatrolData::Update()
{
	//Input
	Input();

	//入力があった場合
	if (m_isCheck)
	{
		m_isCheck = false;

		//キュー
		std::priority_queue<
			tuple<int, int, int>,
			vector<tuple<int, int, int>>,
			greater<tuple<int, int, int>>> l_queue;

		//コスト保存用
		std::vector<std::vector<int>> l_costs;
		l_costs.resize(m_size_x);
		for (int i = 0; i < m_size_x; ++i) {
			l_costs[i].resize(m_size_y);
		}

		//合計値
		int l_heu =
			abs(m_start_x - m_goal_x) +
			abs(m_start_y - m_goal_y);

		//スタート地点
		l_queue.push(std::make_tuple(l_heu, m_start_x, m_start_y));
		m_rootData[m_start_x][m_start_y] = true;

		//方角
		int l_dir_x[4] = { 1,0,-1,0 };
		int l_dir_y[4] = { 0,-1,0,1 };

		//Astar
		int l_goal_heu = -1;
		while (l_goal_heu == -1)
		{
			//検索スタート位置
			int l_start_x = std::get<1>(l_queue.top());
			int l_start_y = std::get<2>(l_queue.top());

			//先頭を削除
			l_queue.pop();

			for (int i = 0; i < 4; ++i)
			{
				//検索先マス
				int l_x = l_start_x + l_dir_x[i];
				int l_y = l_start_y + l_dir_y[i];

				//マップ外
				if (l_x < 0 || l_y < 0 ||
					l_x >= m_size_x || l_y >= m_size_y) {
					continue;
				}

				//検索範囲内(heu+スタート地点cost+進む先cost)
				int l_addCost = l_costs[l_start_x][l_start_y] +
					m_astarDatas[l_x][l_y].cost;
				int l_currentHeu =
					abs(l_x - m_goal_x) +
					abs(l_y - m_goal_y) +
					l_addCost;

				//ゴール
				if (l_x == m_goal_x && l_y == m_goal_y) {
					l_goal_heu = l_currentHeu;
				}

				//進行可能かつ、未探索
				if (m_astarDatas[l_x][l_y].type ==
					AstarType::Move &&
					!m_rootData[l_x][l_y])
				{
					l_queue.push(
						std::make_tuple(l_currentHeu, l_x, l_y));
					l_costs[l_x][l_y] = l_addCost;
					m_rootData[l_x][l_y] = true;

					//ゴール時
					if (l_goal_heu != -1) {
						break;
					}
				}
			}

			//空チェック
			if (l_queue.empty()) {
				break;
			}
		}

		//ゴールした場合
		if (l_goal_heu != -1)
		{
			//スタート（ゴール）地点
			std::priority_queue<
				tuple<int, int, int>,
				vector<tuple<int, int, int>>,
				greater<tuple<int, int, int>>> l_rootQueue;
			l_rootQueue.push(std::make_tuple(
				l_goal_heu, m_goal_x, m_goal_y));
			m_astarDatas[m_goal_x][m_goal_y].type = AstarType::Root;
			m_rootData[m_goal_x][m_goal_y] = false;

			//逆探索
			int l_searchCount = 1;
			bool l_isGoal = false;
			while (!l_isGoal)
			{
				//検索スタート位置
				int l_start_x = std::get<1>(l_rootQueue.top());
				int l_start_y = std::get<2>(l_rootQueue.top());

				//先頭を削除
				l_rootQueue.pop();

				for (int i = 0; i < 4; ++i)
				{
					//検索先マス
					int l_x = l_start_x + l_dir_x[i];
					int l_y = l_start_y + l_dir_y[i];

					//マップ外
					if (l_x < 0 || l_y < 0 ||
						l_x >= m_size_x || l_y >= m_size_y) {
						continue;
					}

					//検索範囲内
					int l_heu =
						abs(l_x - m_goal_x) +
						abs(l_y - m_goal_y);
					int l_addHeu = l_heu + l_costs[l_x][l_y];

					//スタートにゴール
					if (l_x == m_start_x && l_y == m_start_x) {
						l_isGoal = true;
					}

					//探索した道か否か
					if (m_rootData[l_x][l_y] &&
						l_heu == l_searchCount &&
						l_addHeu == l_goal_heu)
					{
						l_searchCount++;
						m_astarDatas[l_x][l_y].type = AstarType::Root;
						l_rootQueue.push(std::make_tuple(
							l_heu, l_x, l_y));
						break;
					}
				}
			}
		}
	}
}

void EnemyPatrolData::Input()
{
	//仮
	if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_Q))
	{
		Init();
		m_isCheck = true;
		m_isInput = true;
	}
	if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_R))
	{
		Init();
		m_isInput = false;
	}

	if (!m_isInput)
	{
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_LEFT))
		{
			int l_sub = m_goal_x - 1;
			if (l_sub < 0) {
				return;
			}
			if (m_astarDatas[l_sub][m_goal_y].type ==
				AstarType::Move) {
				m_goal_x--;
			}
		}
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_RIGHT))
		{
			int l_sub = m_goal_x + 1;
			if (l_sub >= m_size_x) {
				return;
			}
			if (m_astarDatas[l_sub][m_goal_y].type ==
				AstarType::Move) {
				m_goal_x++;
			}
		}
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_DOWN))
		{
			int l_sub = m_goal_y - 1;
			if (l_sub < 0) {
				return;
			}
			if (m_astarDatas[m_goal_x][l_sub].type ==
				AstarType::Move) {
				m_goal_y--;
			}
		}
		if (KeyBoradInputManager::
			Instance()->InputTrigger(DIK_UP))
		{
			int l_sub = m_goal_y + 1;
			if (l_sub >= m_size_y) {
				return;
			}
			if (m_astarDatas[m_goal_x][l_sub].type ==
				AstarType::Move) {
				m_goal_y++;
			}
		}
	}

}

void EnemyPatrolData::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	for (int i = 0; i < m_size_x; ++i)
	{
		for (int j = 0; j < m_size_y; ++j)
		{
			KazMath::Color l_color = { 255,0,255,255 };
			if (m_astarDatas[i][j].type ==
				AstarType::UnMove) {
				l_color = { 0,0,0,0 };
			}
			else if (m_astarDatas[i][j].type ==
				AstarType::Root) {
				l_color = { 255,0,0,255 };
			}

			//仮
			if (m_isInput) {
				if (m_astarDatas[i][j].type !=
					AstarType::Root) {
					l_color = { 0,0,0,0 };
				}
			}
			if (i == m_start_x && j == m_start_y ||
				i == m_goal_x && j == m_goal_y) {
				l_color = { 255,0,0,255 };
			}

			m_modelRenders[i][j]->m_model.Draw(
				arg_rasterize,
				arg_blasVec,
				m_astarDatas[i][j].trans,
				l_color);
		}
	}
}
