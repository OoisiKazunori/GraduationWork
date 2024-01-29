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
	//�}�b�v�`�b�v����
	std::vector<std::pair<int, int>> l_base_chipRoots;
	l_base_chipRoots = CalcChipRoots(m_checkPoints);

	//�p���P�A���Ȃ���ʉߍ��W�̎Z�o
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

	//�`�b�v�T�C�Y
	float l_chipSize =
		m_patrolConfig.lock()->GetChipSize();

	//�G�ʒu(�I�t�Z�b�g���݂̃f�[�^������ׁA���O)
	l_checkPoints.push_back(
		std::make_pair(
			static_cast<int>((arg_enemyPos.first -
				m_patrolConfig.lock()->GetOffsetX()) / l_chipSize),
			static_cast<int>((arg_enemyPos.second -
				m_patrolConfig.lock()->GetOffsetY()) / l_chipSize)));
	//�������ʒu(��)
	l_checkPoints.push_back(
		std::make_pair(
			static_cast<int>((arg_soundPos.first -
				m_patrolConfig.lock()->GetOffsetX()) / l_chipSize),
			static_cast<int>((arg_soundPos.second -
				m_patrolConfig.lock()->GetOffsetY()) / l_chipSize)));

	l_base_chipRoots = CalcChipRoots(l_checkPoints, true);

	//�p���P�A���Ȃ���ʉߍ��W�̎Z�o
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

	//�Œ�
	if (arg_checkPoints.size() == 1) {
		return l_base_chipRoots;
	}

	//�`�F�b�N�|�C���g����
	for (int p = 0; p < arg_checkPoints.size(); ++p)
	{
		//�L���[
		std::priority_queue<
			std::tuple<int, int, int>,
			std::vector<std::tuple<int, int, int>>,
			std::greater<std::tuple<int, int, int>>> l_queue;

		//�ʂ���������p
		std::vector<std::vector<bool>> l_rootData;
		l_rootData.resize(m_patrolConfig.lock()->GetSizeX());

		//�R�X�g�ۑ��p
		size_t l_map_x = m_patrolConfig.lock()->GetSizeX();
		size_t l_map_y = m_patrolConfig.lock()->GetSizeY();
		std::vector<std::vector<int>> l_costs;
		l_costs.resize(l_map_x);
		for (int i = 0; i < l_map_x; ++i) {
			l_costs[i].resize(l_map_y);
			l_rootData[i].resize(l_map_y);
		}

		//�n�_�ƏI�_�ݒ�
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

		//���v�l
		int l_heu = CalcHeuristic(
			l_base_start_x, l_base_start_y,
			l_base_goal_x, l_base_goal_y);

		//�X�^�[�g�n�_
		l_queue.push(std::make_tuple(
			l_heu, l_base_start_x, l_base_start_y));
		l_rootData[l_base_start_x][l_base_start_y] = true;

		//���p
		int l_dir_x[4] = { 1,0,-1,0 };
		int l_dir_y[4] = { 0,-1,0,1 };

		//Astar
		int l_goal_heu = -1;
		while (l_goal_heu == -1)
		{
			//�����X�^�[�g�ʒu
			int l_start_x = std::get<1>(l_queue.top());
			int l_start_y = std::get<2>(l_queue.top());

			//�擪���폜
			l_queue.pop();

			for (int i = 0; i < 4; ++i)
			{
				//������}�X
				int l_x = l_start_x + l_dir_x[i];
				int l_y = l_start_y + l_dir_y[i];

				//�}�b�v�O
				if (!IsIntheArea(l_x, l_y)) {
					continue;
				}

				//�����͈͓�(heu+�X�^�[�g�n�_cost+�i�ސ�cost)
				int l_addCost = l_costs[l_start_x][l_start_y] +
					m_patrolConfig.lock()->GetData(l_x, l_y).cost;
				int l_currentHeu =
					CalcHeuristic(
						l_x, l_base_goal_x,
						l_y, l_base_goal_y) + l_addCost;

				//�S�[��
				if (IsGoal(
					l_x, l_y,
					l_base_goal_x, l_base_goal_y)) {
					l_goal_heu = l_currentHeu;
				}

				//�i�s�\���A���T��
				if (m_patrolConfig.lock()->GetData(l_x, l_y).type !=
					PatrolConfig::AstarType::UnMove &&
					!l_rootData[l_x][l_y])
				{
					l_queue.push(
						std::make_tuple(l_currentHeu, l_x, l_y));
					l_costs[l_x][l_y] = l_addCost;
					l_rootData[l_x][l_y] = true;

					//�S�[����
					if (l_goal_heu != -1) {
						break;
					}
				}
			}

			//��`�F�b�N
			if (l_queue.empty()) {
				break;
			}
		}

		//�S�[�������ꍇ
		if (l_goal_heu != -1)
		{
			//�X�^�[�g(�S�[��)�n�_
			std::priority_queue<
				std::tuple<int, int, int>,
				std::vector<std::tuple<int, int, int>>,
				std::greater<std::tuple<int, int, int>>> l_rootQueue;
			//���[�g�ۑ��p
			std::vector<std::pair<int, int>> l_chipRoots;

			//�T���p�L���[
			l_rootQueue.push(std::make_tuple(
				l_goal_heu, l_base_goal_x, l_base_goal_y));
			//���[�g�ۑ��p
			l_chipRoots.push_back(
				std::make_pair(l_base_goal_x, l_base_goal_y));
			//�}�b�v�f�[�^
			m_patrolConfig.lock()->SetType(
				l_base_goal_x,
				l_base_goal_y,
				PatrolConfig::AstarType::Root);
			//�F(�`��p)
			m_patrolConfig.lock()->SetColor(
				l_base_goal_x,
				l_base_goal_y,
				m_color);
			//�T���ς̈ʒu�m�F�p
			l_rootData[l_base_goal_x][l_base_goal_y] = false;

			//�t�T��
			int l_searchCount = 1;
			bool l_isGoal = false;
			while (!l_isGoal)
			{
				//�����X�^�[�g�ʒu
				int l_start_x = std::get<1>(l_rootQueue.top());
				int l_start_y = std::get<2>(l_rootQueue.top());

				//�擪���폜
				l_rootQueue.pop();

				for (int i = 0; i < 4; ++i)
				{
					//������}�X
					int l_x = l_start_x + l_dir_x[i];
					int l_y = l_start_y + l_dir_y[i];

					//�}�b�v�O
					if (!IsIntheArea(l_x, l_y)) {
						continue;
					}

					//�����͈͓�
					int l_heu = CalcHeuristic(
						l_x, l_base_goal_x,
						l_y, l_base_goal_y);
					int l_addHeu = l_heu + l_costs[l_x][l_y];

					//�X�^�[�g�ɃS�[��
					if (IsGoal(
						l_x, l_y,
						l_base_start_x, l_base_start_y))
					{
						SortRoots(
							l_base_chipRoots,
							l_chipRoots);
						l_isGoal = true;
					}

					//�T�����������ۂ�
					if (l_rootData[l_x][l_y] &&
						l_heu == l_searchCount &&
						l_addHeu == l_goal_heu)
					{
						l_searchCount++;
						m_patrolConfig.lock()->SetType(
							l_x,
							l_y,
							PatrolConfig::AstarType::Root);
						//�F(�`��p)
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

		//���������ꍇ�͕Г��̒T��
		if (arg_isPatrol) {
			break;
		}
	}

	//�������ĂȂ��ꍇ�͍Ō�̔�蕔�����폜
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
	//�t�ɂ��đ��
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

	//�`�b�v�T�C�Y
	float l_chipSize =
		m_patrolConfig.lock()->GetChipSize();

	//�Œ�
	if (arg_roots.size() == 1) {
		l_rootPos.push_back(
			std::make_pair(
				arg_roots[0].first * l_chipSize,
				arg_roots[0].second * l_chipSize));
		return l_rootPos;
	}

	//�]��̊���
	float l_surplusRate = 0.0f;
	//�i�ފ���
	float l_moveRate = 1.0f /
		(l_chipSize / EnemyConfig::speed);

	//�X�^�[�g�n�_
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

		//�ߋ��̕�����ۑ�
		l_old_x = l_start_x;
		l_old_y = l_start_y;
		l_old_dir = l_dir;

		//�ړ������Z�o�p
		l_start_x = arg_roots[i].first;
		l_start_y = arg_roots[i].second;
		l_end_x = arg_roots[l_next].first;
		l_end_y = arg_roots[l_next].second;

		//�X�^�[�g�̍��W
		std::pair<float, float> l_startPos =
			std::make_pair(
				l_chipSize * l_start_x,
				l_chipSize * l_start_y);
		if (i == 0 && arg_startPos.first > 0.0f) {
			l_startPos = arg_startPos;
		}
		//�S�[���̍��W
		std::pair<float, float> l_goalPos =
			std::make_pair(
				l_chipSize * l_end_x,
				l_chipSize * l_end_y);

		//���ړ�
		bool l_isCurve = false;
		if (l_start_x != l_end_x)
		{
			//�E
			if (l_start_x < l_end_x) {
				l_dir = EnemyConfig::Direction::RIGHT;
			}
			//��
			else {
				l_dir = EnemyConfig::Direction::LEFT;
			}

			//�Ȃ������ꍇ
			if (l_old_dir == EnemyConfig::Direction::UP ||
				l_old_dir == EnemyConfig::DOWN) {
				l_isStraight = false;
				l_isCurve = true;
			}
		}
		//�c�ړ�
		else
		{
			//��
			if (l_start_y < l_end_y) {
				l_dir = EnemyConfig::Direction::UP;
			}
			//��
			else {
				l_dir = EnemyConfig::Direction::DOWN;
			}

			//�Ȃ������ꍇ
			if (l_old_dir == EnemyConfig::Direction::RIGHT ||
				l_old_dir == EnemyConfig::LEFT) {
				l_isStraight = false;
				l_isCurve = true;
			}
		}

		//�v�Z����}�X�̎c�苗������
		float l_subRate = 1.0f - l_surplusRate;
		//���t���ړ���
		int l_moveFrame =
			static_cast<int>(l_subRate / l_moveRate) + 1;

		//�Ȃ������ꍇ
		if (l_isCurve && i != 0)
		{
			l_isStraight = false;

			//�Ȃ������ꍇ�̃X�^�[�g�̍��W
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

				//�`�F�b�N�|�C���g�ʉ�(�ߋ�)
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
		//�����̏ꍇ
		else
		{
			//�v�Z����}�X�̎c�苗������
			float l_subRate = 1.0f - l_surplusRate;
			//���t���ړ���
			int l_moveFrame =
				static_cast<int>(l_subRate / l_moveRate);

			//�]�肪���������ꍇ
			float l_mob = std::fmodf(
				l_subRate, l_moveRate);
			if (l_mob >= 0.001f)
			{
				//�I�[�o�[�����������v�Z
				float l_overDist =
					l_subRate * l_chipSize + EnemyConfig::speed;
				//�I�[�o�[�������̊���
				l_surplusRate =
					(l_overDist - l_chipSize) / l_chipSize;
			}
			else {
				l_surplusRate = 0.0f;
			}

			//�`�F�b�N�|�C���g�ʉ�(�ߋ�)
			if (l_oldIsCheckPoint) {
				l_oldIsCheckPoint = false;
				l_isCheckPoint = true;
			}

			//�`�F�b�N�|�C���g�O�̊m�F
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
			//�Ō�̗�O����
			if (l_next == arg_roots.size() - 1) {
				if (!l_oldIsCurve) {
					l_isStraight = true;
				}
			}

			//���W�i�[
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

					//�`�F�b�N�|�C���g�ʉ�(�ߋ�)
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

		//�Ȃ��������̏��̕ۑ�
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
