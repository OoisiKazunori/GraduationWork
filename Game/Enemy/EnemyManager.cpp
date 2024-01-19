#include "EnemyManager.h"
#include "EnemyConfig.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include"../MapLoader/MapLoader.h"
#include "../Echo/EchoArray.h"

EnemyManager::EnemyManager()
{
	m_checkSound =
		SoundManager::Instance()->SoundLoadWave("Resource/Sound/Discovery.wav");
	m_checkSound.volume = 0.05f;
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
		//m_patrolDatas[i].Init();
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
	//m_patrolDatas[1].SetColor(KazMath::Color(255, 255, 0, 255));
	//m_patrolDatas[2].SetColor(KazMath::Color(0, 255, 0, 255));

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
	for (auto i = arg_mapDatas.begin();
		i != arg_mapDatas.end(); ++i)
	{
		for (auto j = i->m_position.begin();
			j != i->m_position.end(); ++j)
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
		4.84f);

	//�}�b�v�f�[�^
	std::list<std::list<int>> l_mapChips =
		MapManager::GetMapChips(arg_stageNum);

	int l_x = 0;
	int l_y = 0;
	for (auto i = l_mapChips.begin(); i != l_mapChips.end(); ++i)
	{
		l_x = 0;

		for (auto j = i->begin(); j != i->end(); ++j)
		{
			if (*j == 1) {
				m_config->SetType(l_x, l_y,
					PatrolConfig::AstarType::UnMove);
			}
			else {
				m_config->SetType(l_x, l_y,
					PatrolConfig::AstarType::Move);
			}

			l_x++;
		}

		l_y++;
	}

	//�}�b�v�f�[�^�����Ă��珉����
	float l_offset_x = m_config.get()->GetOffsetX();
	float l_offset_y = m_config.get()->GetOffsetY();
	for (int i = 0; i < l_enemyNum; ++i)
	{
		m_enemys[i].SetData(arg_rasterize);
		m_enemys[i].SetOffset(std::make_pair(
			l_offset_x,
			l_offset_y));
		m_patrolDatas[i].SetData(m_config);
		m_patrolDatas[i].Init();
	}
	m_patrolDraw.SetData(arg_rasterize, m_config);
}

void EnemyManager::Update(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Vec3<float> arg_playerPos,
	std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{
	bool isInput = false;
	if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_E))
	{
		isInput = true;
	}

	float l_offset_x = m_config.get()->GetOffsetX();
	float l_offset_y = m_config.get()->GetOffsetY();

	std::pair<float, float> active_sPos = { 0.0f,0.0f };
	float active_sDist = 0.0f;

	for (int i = 0; i < m_enemys.size(); ++i)
	{
		std::pair<float, float> ePos = {
				m_enemys[i].GetPos().x,
				m_enemys[i].GetPos().z
		};

		//Echo����
		bool l_isSound = false;
		for (auto& echo : EchoArray::Instance()->GetEcho())
		{
			//�A�N�e�B�u�Ȃ画��
			if (echo.GetIsActive())
			{
				//����(x��z�ł����������ĂȂ�)
				std::pair<float, float> sPos = {
					echo.GetPos().x,
					echo.GetPos().z
				};

				float l_dist = CircleCol2D(
					ePos, EnemyConfig::soundCheckDist,
					sPos, echo.GetNowRadius());
				if (active_sDist < l_dist)
				{
					l_isSound = true;
					active_sPos = sPos;
				}
			}
		}

		//��
		if (l_isSound || isInput) {
			std::vector<std::pair<float, float>>
				l_checkSoundPos =
				m_patrolDatas[i].CheckSound(ePos, active_sPos);
			//m_enemys[i].SetState(Enemy::State::Warning);
			m_enemys[i].SetCheckSoundPos(l_checkSoundPos);
		}

		//�ŒZ�����̉��f�[�^���Z�b�g
		std::vector<std::pair<float, float>>
			l_checkSoundPos =
			m_patrolDatas[i].CheckSound(
				ePos, active_sPos);
		//m_enemys[i].SetState(Enemy::State::Warning);
		m_enemys[i].SetCheckSoundPos(l_checkSoundPos);

		//�I�t�Z�b�g
		m_enemys[i].SetOffset(std::make_pair(
			l_offset_x,
			l_offset_y));

		m_patrolDatas[i].Update();
		m_enemys[i].SetRootPos(m_patrolDatas[i].GetRootPos());
		m_enemys[i].SetCheckPointDelay(
			m_patrolDatas[i].GetCheckPointDelay());

		m_enemys[i].CheckInEcho(arg_stageMeshCollision);

		m_enemys[i].Update(
			arg_stageColliders,
			arg_bulletMgr,
			arg_playerPos,
			arg_stageMeshCollision);

		//������
		if (m_enemys[i].IsDiscovery()) {
			SoundManager::Instance()->
				SoundPlayerWave(m_checkSound, 0);
		}
	}

	//����
	m_config->Update();
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
	/*m_patrolDraw.Draw(
		arg_rasterize,
		arg_blasVec);*/
}

float EnemyManager::CircleCol2D(
	std::pair<float, float> arg_pos1,
	float arg_rad1,
	std::pair<float, float> arg_pos2,
	float arg_rad2)
{
	//2�_�Ԃ̋���
	float l_dist = std::sqrtf(
		powf(
			(arg_pos1.first - arg_pos2.first), 2.0f) +
		powf(
			(arg_pos1.second - arg_pos2.second), 2.0f)
	);

	//���a�̍��v
	float l_addRad = arg_rad1 + arg_rad2;

	if (l_dist < l_addRad) { return l_dist; }

	return -1.0f;
}
