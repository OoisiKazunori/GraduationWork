#include "EnemyManager.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include"../MapLoader/MapLoader.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	//�G������
	for (int i = 0; i < m_enemys.size(); ++i)
	{
		m_enemys[i].Init();
		m_patrolDatas[i].Init();
	}

	//�����|�C���g�ʒu
	/*m_patrolDatas[0].AddCheckPoint(0, 0);
	m_patrolDatas[0].AddCheckPoint(3, 0);
	m_patrolDatas[0].AddCheckPoint(3, 3);
	m_patrolDatas[0].AddCheckPoint(0, 3);

	m_patrolDatas[1].AddCheckPoint(4, 4);
	m_patrolDatas[1].AddCheckPoint(6, 4);
	m_patrolDatas[1].AddCheckPoint(6, 1);
	m_patrolDatas[1].AddCheckPoint(5, 0);

	m_patrolDatas[2].AddCheckPoint(7, 6);
	m_patrolDatas[2].AddCheckPoint(7, 9);*/

	m_patrolDatas[0].SetColor(KazMath::Color(255, 255, 255, 255));
	m_patrolDatas[1].SetColor(KazMath::Color(255, 255, 0, 255));
	m_patrolDatas[2].SetColor(KazMath::Color(0, 255, 0, 255));

	//����
	m_patrolDraw.Init();
}

void EnemyManager::SetMapData(
	int arg_stageNum,
	std::list<EnemyData> arg_mapDatas,
	DrawingByRasterize& arg_rasterize)
{
	bool l_isAdd = false;
	int l_enemyNum = 0;

	//�`�F�b�N�|�C���g
	for (auto i = arg_mapDatas.begin(); i != arg_mapDatas.end(); ++i)
	{
		for (auto j = i->m_position.begin(); j != i->m_position.end(); ++j)
		{
			//�`�F�b�N�|�C���g�I��
			if (j->x == -1) {
				l_isAdd = false;
				break;
			}

			//�G�̔z��ԍ����Z
			if (!l_isAdd) {
				l_isAdd = true;
				l_enemyNum++;
				m_patrolDatas.push_back(PatrolData());
				m_enemys.push_back(Enemy());
			}

			m_patrolDatas[l_enemyNum - 1].
				AddCheckPoint(j->x, j->y);
		}
	}

	//�}�b�v�T�C�Y
	m_config = std::make_shared<PatrolConfig>(
		MapManager::GetMapSizeData(arg_stageNum).x,
		MapManager::GetMapSizeData(arg_stageNum).y,
		2.0f);

	//�}�b�v�f�[�^
	std::list<std::list<int>> l_mapChips =
		MapManager::GetMapChips(arg_stageNum);
	int l_x = 0;
	for (auto i = l_mapChips.begin(); i != l_mapChips.end(); ++i)
	{
		int l_y = 0;

		for (auto j = i->begin(); j != i->end(); ++j)
		{
			if (*j == 0) {
				m_config->SetType(l_x, l_y,
					PatrolConfig::AstarType::Move);
			}
			else  if (*j == 1) {
				m_config->SetType(l_x, l_y,
					PatrolConfig::AstarType::UnMove);
			}

			l_y++;
		}

		l_x++;
	}

	//�}�b�v�f�[�^�����Ă��珉����
	size_t l_offset_x = m_config.get()->GetOffsetX();
	size_t l_offset_y = m_config.get()->GetOffsetY();
	for (int i = 0; i < l_enemyNum; ++i)
	{
		m_enemys[i].SetData(arg_rasterize);
		m_enemys[i].AddOffset(std::make_pair(
			l_offset_x,
			l_offset_y));
		m_patrolDatas[i].SetData(m_config);
	}
	m_patrolDraw.SetData(arg_rasterize, m_config);
}

void EnemyManager::Update(
	std::weak_ptr<MeshCollision> arg_meshCollision)
{
	bool isInput = false;
	if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_E))
	{
		isInput = true;
	}

	for (int i = 0; i < m_enemys.size(); ++i)
	{
		if (isInput) {
			std::pair<float, float> ePos = {
				m_enemys[i].GetPos().x,
				m_enemys[i].GetPos().z };
			std::pair<float, float> sPos = { 0.0f,0.0f };
			std::vector<std::pair<float, float>>
				l_checkSoundPos =
				m_patrolDatas[i].CheckSound(ePos, sPos);
			m_enemys[i].SetState(Enemy::State::Warning);
			m_enemys[i].SetCheckSoundPos(l_checkSoundPos);
		}

		m_patrolDatas[i].Update();
		m_enemys[i].SetRootPos(m_patrolDatas[i].GetRootPos());
		m_enemys[i].SetCheckPointDelay(
			m_patrolDatas[i].GetCheckPointDelay());
		m_enemys[i].Update(arg_meshCollision);
	}

	//�ړ�
	//m_patrolDraw.SetRootPos(m_patrolData[0].GetRootPos());

	//����
	m_patrolDraw.Update();
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
	m_patrolDraw.Draw(
		arg_rasterize,
		arg_blasVec);
}