#include "PatrolData.h"
#include "EnemyConfig.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"

PatrolData::PatrolData()
{
	m_color = { 255,0,0,255 };
}

PatrolData::~PatrolData()
{
}

void PatrolData::SetData(
	std::weak_ptr<PatrolConfig> arg_config)
{
	m_patrolConfig = arg_config;
}

void PatrolData::Init()
{
	//マップチップ検索
	std::vector<std::pair<int, int>> l_base_chipRoots;
	l_base_chipRoots = CalcChipRoots(m_checkPoints);

	//角をケアしながら通過座標の算出
	m_rootPos = CalcRootPos(l_base_chipRoots);
}

void PatrolData::Update()
{
}

std::vector<std::pair<float, float>>
PatrolData::CheckSound(
	std::pair<float, float> arg_enemyPos,
	std::pair<float, float> arg_soundPos
)
{
	std::vector<std::pair<int, int>> l_base_chipRoots;
	std::vector<std::pair<int, int>> l_checkPoints;

	//チップサイズ
	float l_chipSize =
		m_patrolConfig.lock()->GetChipSize();

	//敵位置(オフセット込みのデータが来る為、除外)
	l_checkPoints.push_back(
		std::make_pair(
			static_cast<int>((arg_enemyPos.first -
				m_patrolConfig.lock()->GetOffsetX()) / l_chipSize),
			static_cast<int>((arg_enemyPos.second -
				m_patrolConfig.lock()->GetOffsetY()) / l_chipSize)));
	//音発生位置(仮)
	l_checkPoints.push_back(
		std::make_pair(
			static_cast<int>((arg_soundPos.first -
				m_patrolConfig.lock()->GetOffsetX()) / l_chipSize),
			static_cast<int>((arg_soundPos.second -
				m_patrolConfig.lock()->GetOffsetY()) / l_chipSize)));

	l_base_chipRoots = CalcChipRoots(l_checkPoints, true);

	//角をケアしながら通過座標の算出
	std::vector<std::pair<float, float>> l_soundCheckPos;
	l_soundCheckPos = CalcRootPos(
		l_base_chipRoots, arg_enemyPos, true);

	return l_soundCheckPos;
}

std::vector<std::pair<int, int>>
PatrolData::CalcChipRoots(
	const std::vector<std::pair<int, int>>
	& arg_checkPoints,
	bool arg_isPatrol)
{
	std::vector<std::pair<int, int>> l_base_chipRoots;
	l_base_chipRoots.push_back(
		std::make_pair(
			arg_checkPoints[0].first,
			arg_checkPoints[0].second));

	//固定
	if (arg_checkPoints.size() == 1) {
		return l_base_chipRoots;
	}

	//チェックポイント分回す
	for (int p = 0; p < arg_checkPoints.size(); ++p)
	{
		//キュー
		std::priority_queue<
			std::tuple<int, int, int>,
			std::vector<std::tuple<int, int, int>>,
			std::greater<std::tuple<int, int, int>>> l_queue;

		//通ったか判定用
		std::vector<std::vector<bool>> l_rootData;
		l_rootData.resize(m_patrolConfig.lock()->GetSizeX());

		//コスト保存用
		size_t l_map_x = m_patrolConfig.lock()->GetSizeX();
		size_t l_map_y = m_patrolConfig.lock()->GetSizeY();
		std::vector<std::vector<int>> l_costs;
		l_costs.resize(l_map_x);
		for (int i = 0; i < l_map_x; ++i) {
			l_costs[i].resize(l_map_y);
			l_rootData[i].resize(l_map_y);
		}

		//始点と終点設定
		int l_base_start_x = arg_checkPoints[p].first;
		int l_base_start_y = arg_checkPoints[p].second;
		int l_base_goal_x, l_base_goal_y;
		if (p < arg_checkPoints.size() - 1) {
			l_base_goal_x =
				arg_checkPoints[p + 1].first;
			l_base_goal_y =
				arg_checkPoints[p + 1].second;
		}
		else {
			l_base_goal_x =
				arg_checkPoints[0].first;
			l_base_goal_y =
				arg_checkPoints[0].second;
		}

		//合計値
		int l_heu = CalcHeuristic(
			l_base_start_x, l_base_start_y,
			l_base_goal_x, l_base_goal_y);

		//スタート地点
		l_queue.push(std::make_tuple(
			l_heu, l_base_start_x, l_base_start_y));
		l_rootData[l_base_start_x][l_base_start_y] = true;

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
				if (!IsIntheArea(l_x, l_y)) {
					continue;
				}

				//検索範囲内(heu+スタート地点cost+進む先cost)
				int l_addCost = l_costs[l_start_x][l_start_y] +
					m_patrolConfig.lock()->GetData(l_x, l_y).cost;
				int l_currentHeu =
					CalcHeuristic(
						l_x, l_base_goal_x,
						l_y, l_base_goal_y) + l_addCost;

				//ゴール
				if (IsGoal(
					l_x, l_y,
					l_base_goal_x, l_base_goal_y)) {
					l_goal_heu = l_currentHeu;
				}

				//進行可能かつ、未探索
				if (m_patrolConfig.lock()->GetData(l_x, l_y).type !=
					PatrolConfig::AstarType::UnMove &&
					!l_rootData[l_x][l_y])
				{
					l_queue.push(
						std::make_tuple(l_currentHeu, l_x, l_y));
					l_costs[l_x][l_y] = l_addCost;
					l_rootData[l_x][l_y] = true;

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
			//スタート(ゴール)地点
			std::priority_queue<
				std::tuple<int, int, int>,
				std::vector<std::tuple<int, int, int>>,
				std::greater<std::tuple<int, int, int>>> l_rootQueue;
			//ルート保存用
			std::vector<std::pair<int, int>> l_chipRoots;

			//探索用キュー
			l_rootQueue.push(std::make_tuple(
				l_goal_heu, l_base_goal_x, l_base_goal_y));
			//ルート保存用
			l_chipRoots.push_back(
				std::make_pair(l_base_goal_x, l_base_goal_y));
			//マップデータ
			m_patrolConfig.lock()->SetType(
				l_base_goal_x,
				l_base_goal_y,
				PatrolConfig::AstarType::Root);
			//色(描画用)
			m_patrolConfig.lock()->SetColor(
				l_base_goal_x,
				l_base_goal_y,
				m_color);
			//探索済の位置確認用
			l_rootData[l_base_goal_x][l_base_goal_y] = false;

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
					if (!IsIntheArea(l_x, l_y)) {
						continue;
					}

					//検索範囲内
					int l_heu = CalcHeuristic(
						l_x, l_base_goal_x,
						l_y, l_base_goal_y);
					int l_addHeu = l_heu + l_costs[l_x][l_y];

					//スタートにゴール
					if (IsGoal(
						l_x, l_y,
						l_base_start_x, l_base_start_y))
					{
						SortRoots(
							l_base_chipRoots,
							l_chipRoots);
						l_isGoal = true;
					}

					//探索した道か否か
					if (l_rootData[l_x][l_y] &&
						l_heu == l_searchCount &&
						l_addHeu == l_goal_heu)
					{
						l_searchCount++;
						m_patrolConfig.lock()->SetType(
							l_x,
							l_y,
							PatrolConfig::AstarType::Root);
						//色(描画用)
						m_patrolConfig.lock()->SetColor(
							l_x,
							l_y,
							m_color);
						l_chipRoots.push_back(std::make_pair(l_x, l_y));
						l_rootQueue.push(std::make_tuple(
							l_heu, l_x, l_y));
						break;
					}
				}
			}
		}

		//音が鳴った場合は片道の探索
		if (arg_isPatrol) {
			break;
		}
	}

	//音が鳴ってない場合は最後の被り部分を削除
	if (!arg_isPatrol) {
		l_base_chipRoots.resize(
			l_base_chipRoots.size() - 1);
	}

	return l_base_chipRoots;
}

void PatrolData::SortRoots(
	std::vector<std::pair<int, int>>& arg_base_roots,
	const std::vector<std::pair<int, int>>& arg_roots)
{
	//逆にして代入
	int l_size = static_cast<int>(arg_roots.size()) - 1;
	for (int i = l_size; i >= 0; --i) {
		arg_base_roots.push_back(arg_roots[i]);
	}
}

std::vector<std::pair<float, float>>
PatrolData::CalcRootPos(
	std::vector<std::pair<int, int>> arg_roots,
	std::pair<float, float> arg_startPos,
	bool arg_isSound)
{
	int l_next = -1;
	int l_start_x = -1;
	int l_start_y = -1;
	int	l_end_x = -1;
	int l_end_y = -1;
	int l_old_x = -1;
	int l_old_y = -1;
	int l_old_dir = -1;
	int l_dir = -1;
	bool l_isStraight = false;
	bool l_oldIsCurve = false;
	bool l_oldIsCheckPoint = false;
	bool l_isCheckPoint = false;

	std::pair<float, float> l_oldStartPos;
	std::pair<float, float> l_oldGoalPos;
	std::vector<std::pair<float, float>> l_rootPos;

	//チップサイズ
	float l_chipSize =
		m_patrolConfig.lock()->GetChipSize();

	//固定
	if (arg_roots.size() == 1) {
		l_rootPos.push_back(
			std::make_pair(
				arg_roots[0].first * l_chipSize,
				arg_roots[0].second * l_chipSize));
		return l_rootPos;
	}

	//余剰の割合
	float l_surplusRate = 0.0f;
	//進む割合
	float l_moveRate = 1.0f /
		(l_chipSize / EnemyConfig::speed);

	//スタート地点
	if (!arg_isSound) {
		m_checkPointDelays.push_back(
			std::make_pair(
				0, 0));
	}

	for (int i = 0; i < arg_roots.size(); ++i)
	{
		l_next = i + 1;
		if (l_next == arg_roots.size()) {
			l_next = 0;
		}

		//過去の方向を保存
		l_old_x = l_start_x;
		l_old_y = l_start_y;
		l_old_dir = l_dir;

		//移動方向算出用
		l_start_x = arg_roots[i].first;
		l_start_y = arg_roots[i].second;
		l_end_x = arg_roots[l_next].first;
		l_end_y = arg_roots[l_next].second;

		//スタートの座標
		std::pair<float, float> l_startPos =
			std::make_pair(
				l_chipSize * l_start_x,
				l_chipSize * l_start_y);
		if (i == 0 && arg_startPos.first > 0.0f) {
			l_startPos = arg_startPos;
		}
		//ゴールの座標
		std::pair<float, float> l_goalPos =
			std::make_pair(
				l_chipSize * l_end_x,
				l_chipSize * l_end_y);

		//横移動
		bool l_isCurve = false;
		if (l_start_x != l_end_x)
		{
			//右
			if (l_start_x < l_end_x) {
				l_dir = EnemyConfig::Direction::RIGHT;
			}
			//左
			else {
				l_dir = EnemyConfig::Direction::LEFT;
			}

			//曲がった場合
			if (l_old_dir == EnemyConfig::Direction::UP ||
				l_old_dir == EnemyConfig::DOWN) {
				l_isStraight = false;
				l_isCurve = true;
			}
		}
		//縦移動
		else
		{
			//上
			if (l_start_y < l_end_y) {
				l_dir = EnemyConfig::Direction::UP;
			}
			//下
			else {
				l_dir = EnemyConfig::Direction::DOWN;
			}

			//曲がった場合
			if (l_old_dir == EnemyConfig::Direction::RIGHT ||
				l_old_dir == EnemyConfig::LEFT) {
				l_isStraight = false;
				l_isCurve = true;
			}
		}

		//計算するマスの残り距離割合
		float l_subRate = 1.0f - l_surplusRate;
		//何フレ移動か
		int l_moveFrame =
			static_cast<int>(l_subRate / l_moveRate) + 1;

		//曲がった場合
		if (l_isCurve && i != 0)
		{
			l_isStraight = false;

			//曲がった場合のスタートの座標
			std::pair<float, float> l_curveStartPos =
				std::make_pair(
					l_chipSize * l_old_x,
					l_chipSize * l_old_y);

			for (int j = 1; j <= l_moveFrame; ++j)
			{
				float l_currentRate_x = l_moveRate * j;
				float l_currentRate_y = l_currentRate_x;
				if (l_dir == 0 || l_dir == 2) {
					l_currentRate_x =
						std::powf(l_currentRate_x, 2.0f);
				}
				else {
					l_currentRate_y =
						std::powf(l_currentRate_y, 2.0f);
				}

				float l_pos_x =
					l_curveStartPos.first +
					(l_goalPos.first - l_curveStartPos.first) *
					l_currentRate_x;
				float l_pos_y =
					l_curveStartPos.second +
					(l_goalPos.second - l_curveStartPos.second) *
					l_currentRate_y;
				l_rootPos.push_back(
					std::make_pair(l_pos_x, l_pos_y));

				//チェックポイント通過(過去)
				if (j == l_moveFrame &&
					IsCheckPoint(l_end_x, l_end_y)) {
					m_checkPointDelays.push_back(
						std::make_pair(
							static_cast<int>(l_rootPos.size()),
							0));
				}
			}

			l_start_x = l_end_x;
			l_start_y = l_end_y;
			l_dir = -1;
		}
		//直線の場合
		else
		{
			//計算するマスの残り距離割合
			float l_subRate = 1.0f - l_surplusRate;
			//何フレ移動か
			int l_moveFrame =
				static_cast<int>(l_subRate / l_moveRate);

			//余剰が発生した場合
			float l_mob = std::fmodf(
				l_subRate, l_moveRate);
			if (l_mob >= 0.001f)
			{
				//オーバーした距離を計算
				float l_overDist =
					l_subRate * l_chipSize + EnemyConfig::speed;
				//オーバーした分の割合
				l_surplusRate =
					(l_overDist - l_chipSize) / l_chipSize;
			}
			else {
				l_surplusRate = 0.0f;
			}

			//チェックポイント通過(過去)
			if (l_oldIsCheckPoint) {
				l_oldIsCheckPoint = false;
				l_isCheckPoint = true;
			}

			//チェックポイント前の確認
			if (IsCheckPoint(l_end_x, l_end_y))
			{
				l_oldIsCheckPoint = true;

				if (!l_oldIsCurve) {
					l_dir = -1;
					l_old_dir = -1;
					l_isCurve = false;
					l_isStraight = true;
				}
			}
			//最後の例外処理
			if (l_next == arg_roots.size() - 1) {
				if (!l_oldIsCurve) {
					l_isStraight = true;
				}
			}

			//座標格納
			if (l_isStraight)
			{
				for (int j = 1; j <= l_moveFrame; ++j)
				{
					float l_currentRate = l_moveRate * j;
					float l_pos_x =
						l_oldStartPos.first +
						(l_oldGoalPos.first - l_oldStartPos.first) *
						l_currentRate;
					float l_pos_y =
						l_oldStartPos.second +
						(l_oldGoalPos.second - l_oldStartPos.second) *
						l_currentRate;
					l_rootPos.push_back(
						std::make_pair(l_pos_x, l_pos_y));

					//チェックポイント通過(過去)
					if (j == l_moveFrame && l_isCheckPoint) {
						m_checkPointDelays.push_back(
							std::make_pair(
								static_cast<int>(l_rootPos.size()),
								0)
						);
						l_isCheckPoint = false;
					}
				}
			}
			else {
				l_isStraight = true;
			}

			l_oldStartPos = l_startPos;
			l_oldGoalPos = l_goalPos;
		}

		//曲がったかの情報の保存
		l_oldIsCurve = l_isCurve;
	}

	return l_rootPos;
}

int PatrolData::CalcHeuristic(
	int arg_start_x, int arg_start_y,
	int arg_goal_x, int arg_goal_y)
{
	return abs(arg_start_x - arg_start_y) +
		abs(arg_goal_x - arg_goal_y);
}

bool PatrolData::IsIntheArea(int arg_x, int arg_y)
{
	if (arg_x < 0 ||
		arg_y < 0 ||
		arg_x >= m_patrolConfig.lock()->GetSizeX() ||
		arg_y >= m_patrolConfig.lock()->GetSizeY())
	{
		return false;
	}
	return true;
}

bool PatrolData::IsGoal(
	int arg_x, int arg_y,
	int arg_goal_x, int arg_goal_y)
{
	if (arg_x == arg_goal_x &&
		arg_y == arg_goal_y) {
		return true;
	}
	return false;
}

bool PatrolData::IsCheckPoint(int arg_goal_x, int arg_goal_y)
{
	for (int i = 0; i < m_checkPoints.size(); ++i)
	{
		if (m_checkPoints[i].first == arg_goal_x)
		{
			if (m_checkPoints[i].second == arg_goal_y)
			{
				return true;
			}
		}
	}
	return false;
}
