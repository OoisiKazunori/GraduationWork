#include "Enemy.h"
#include "EnemyConfig.h"
#include"../Game/Collision/MeshCollision.h"

Enemy::Enemy()
{
	//m_trans.scale /= 2;
	m_state = State::Patrol;
	m_delayNum = 0;
	m_count = 0;
	m_delay = 0;
	m_isCheckPoint = false;
	m_onGround = false;

	m_checkSoundCount = 0;
	m_isReturn = false;
	m_oldPos = { -1.0f,-1.0f,-1.0f };

	m_offset_x = 0.0f;
	m_offset_y = 0.0f;

	m_gravity = 0.0f;

	m_hp = MAX_HP;
	m_rate = MAX_RATE;
	m_angle = 0.0f;
}

Enemy::~Enemy()
{
}

void Enemy::SetData(
	DrawingByRasterize& arg_rasterize)
{
	////���f���f�[�^���
	//m_enemyBox =
	//	std::make_unique<BasicDraw::BasicModelRender>(
	//		arg_rasterize,
	//		"Resource/cubeFrame/",
	//		"cubeFrame.gltf"
	//		);

	//���f���f�[�^���
	m_enemyBox =
		std::make_unique<BasicDraw::BasicModelRender>(
			arg_rasterize,
			"Resource/Test/Virus/",
			"virus_cur.gltf"
			);

	m_meshCol = std::make_shared<MeshCollision>();
	m_meshCol->Setting(
		m_enemyBox.get()->m_model.
		m_modelInfo->modelData[0].vertexData,
		m_trans);


	m_line.Generate(arg_rasterize);

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
	m_state = State::Patrol;
	m_delayNum = 0;
	m_count = 0;
	m_delay = 0;
	m_isCheckPoint = false;
	m_onGround = false;

	m_checkSoundCount = 0;
	m_isReturn = false;
	m_oldPos = { -1.0f,-1.0f,-1.0f };

	m_offset_x = 0.0f;
	m_offset_y = 0.0f;

	m_gravity = 0.0f;

	m_hp = MAX_HP;
	m_rate = MAX_RATE;
	m_angle = 0.0f;
}

void Enemy::Update(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	KazMath::Vec3<float> arg_playerPos)
{
	//�v���C���[XZ���W
	std::pair<float, float> l_pPos =
		std::make_pair(arg_playerPos.x, arg_playerPos.z);

	//����(�ʏ�or�x��)
	if (m_state == State::Patrol ||
		m_state == State::Warning)
	{
		//���������ꍇ
		if (CheckDistXZ(
			l_pPos, EnemyConfig::soundCheckDist) && false)
		{
			if (!m_isReturn) {
				m_checkSoundCount++;

				if (m_checkSoundCount ==
					m_checkSoundPos.size() - 1) {
					m_isReturn = true;
				}
			}
			else {
				m_checkSoundCount--;

				if (m_checkSoundCount == 0) {
					m_state = State::Patrol;
					m_isReturn = false;
				}
			}

			m_trans.pos = {
					m_checkSoundPos[m_checkSoundCount].first,
					0.0f,
					m_checkSoundPos[m_checkSoundCount].second
			};
		}

		//�`�F�b�N�|�C���g
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
				m_trans.pos.y,
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

			//��
			m_trans.pos.x = m_trans.pos.x + m_offset_x;
			m_trans.pos.z = m_trans.pos.z + m_offset_y;
		}
	}

	//�퓬��
	else if (m_state == State::Combat)
	{
		//�����͈͓��Ȃ�����Ȃ���ˌ�
		if (CheckEye(arg_playerPos, arg_stageColliders))
		{
			//�v���C���[����
			m_trans.quaternion = CalMoveQuaternion(arg_playerPos, m_trans.pos);

			//�ˌ�
		}

		else
		{
			m_rate--;
			if (m_rate < 0)
			{
				m_state = State::Patrol;
				m_rate = MAX_RATE;
			}
		}
	}

	//�����͈͓���
	m_isCombat = false;
	if (CheckDistXZ(
		l_pPos, EnemyConfig::eyeCheckDist) &&
		CheckEye(arg_playerPos, arg_stageColliders))
	{
		m_isCombat = true;
		m_state = State::Combat;
		m_rate = MAX_RATE;
	}

	//��]
	if (m_oldPos.x >= 0.0f)
	{
		if (m_trans.pos != m_oldPos) {
			DirectX::XMVECTOR l_quaternion =
				CalMoveQuaternion(m_trans.pos, m_oldPos);
			m_trans.quaternion = l_quaternion;
		}
	}

	if (m_state == State::Patrol ||
		m_state == State::Warning) {
		//RotateEye();
	}

	//�d��(��)
	if (m_rootPos.size() > 0)
	{
		//�d�͂�������B
		if (!m_onGround) {
			m_gravity -= GRAVITY;
		}
		else {
			m_gravity = 0;
		}
		//m_trans.pos.y += m_gravity;

		m_oldPos = m_trans.pos;
	}

	//����(���b�V��)
	Collision(arg_stageColliders);


	//��UY�Œ�
	m_trans.pos.y = -43.0f;

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

	if (m_isCombat) {
		m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_trans.pos, m_trans.pos + m_trans.GetFront() * 20.0f, KazMath::Color(255, 0, 0, 255));
	}
	else {
		m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_trans.pos, m_trans.pos + m_trans.GetFront() * 20.0f, KazMath::Color(255, 255, 255, 255));
	}

}

DirectX::XMVECTOR Enemy::CalMoveQuaternion(
	KazMath::Vec3<float> arg_pos,
	KazMath::Vec3<float> arg_prevPos)
{
	//�����������ɌX����B
	KazMath::Vec3<float> moveVec = arg_pos - arg_prevPos;
	//�����������x�N�g���𐳋K������B
	moveVec.Normalize();

	//�f�t�H���g�̐��ʃx�N�g������̉�]�ʂ����߂�B
	float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(moveVec));

	//�������Ă���������f�t�H���g�̕�����������[�����Z����������̂ŁA�x�N�g�������u���Œu���B
	KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(moveVec);
	if (cross.Length() <= 0) {
		cross = { 0,1,0 };
	}

	//�N�H�[�^�j�I�������߂�B
	return DirectX::XMQuaternionRotationAxis({
		cross.x,
		cross.y,
		cross.z }, angle);
}

void Enemy::Collision(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders)
{
	const float RAY_LENGTH = 8.0f;

	//�n�ʂƓ����蔻����s���B
	m_onGround = false;

	KazMath::Vec3<float> aaa = -m_trans.GetUp();

	const float GROUND_RAY_OFFSET = 5.0f;
	for (auto itr = arg_stageColliders.begin();
		itr != arg_stageColliders.end(); ++itr) {

		MeshCollision::CheckHitResult rayResult =
			(*itr)->CheckHitRay(
				m_trans.pos +
				m_trans.GetUp() *
				GROUND_RAY_OFFSET,
				-m_trans.GetUp());

		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <=
			RAY_LENGTH + GROUND_RAY_OFFSET)
		{
			//�����߂��B
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH + GROUND_RAY_OFFSET -
					rayResult.m_distance);
			m_onGround = true;
		}

		//�����蔻����v�Z�B
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			m_trans.GetFront());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//�����߂��B
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}

		//������
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			-m_trans.GetFront());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//�����߂��B
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}

		//�E����
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			m_trans.GetRight());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//�����߂��B
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}

		//������
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			-m_trans.GetRight());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//�����߂��B
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}
	}
}

void Enemy::RotateEye()
{
	//�i�s��
	//m_oldQuaternion = m_trans.quaternion;

	//30�x
	float l_rad = 30.0f;
	l_rad = l_rad * (3.14f / 180.0f);
	l_rad *= std::sinf(m_angle);
	m_angle += 0.01f;

	//���W�A�������Z
	KazMath::Transform3D l_trans = m_trans;
	l_trans.Rotation(
		KazMath::Vec3<float>(0, 1, 0),
		l_rad);

	m_trans.quaternion = l_trans.quaternion;
}

bool Enemy::CheckDistXZ(
	std::pair<float, float> arg_checkPos, float arg_dist)
{
	float l_dist = std::sqrtf(
		std::powf(m_trans.pos.x - arg_checkPos.first, 2.0f) +
		std::powf(m_trans.pos.z - arg_checkPos.second, 2.0f));

	if (l_dist < arg_dist) {
		return true;
	}
	return false;
}

bool Enemy::CheckEye(
	KazMath::Vec3<float> arg_playerPos,
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders)
{
	//-1~1
	float l_angleOfView = m_trans.GetFront().Dot(
		(arg_playerPos - m_trans.pos).GetNormal());

	if (l_angleOfView < 0.5f) {

		return false;

	}

	//���b�V������
	bool isHit = false;
	for (auto itr = arg_stageColliders.begin();
		itr != arg_stageColliders.end(); ++itr)
	{
		const float RAY_LENGTH =
			KazMath::Vec3<float>(
				arg_playerPos -
				m_trans.pos).Length();

		MeshCollision::CheckHitResult rayResult =
			(*itr)->CheckHitRay(
				m_trans.pos,
				KazMath::Vec3<float>(arg_playerPos - m_trans.pos).GetNormal());

		//������Ȃ������ꍇ
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			isHit = true;
		}
		else
		{
		}
	
	}

	return !isHit;

}