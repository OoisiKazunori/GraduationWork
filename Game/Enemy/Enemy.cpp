#include "Enemy.h"
#include "EnemyConfig.h"
#include "../Footprint/FootprintMgr.h"
#include "../Game/Bullet/BulletMgr.h"
#include <limits>
#include "../Player/PlayerStatus.h"
#include "../Effect/StopMgr.h"

Enemy::Enemy()
{
	//m_trans.scale /= 2;
	m_state = State::Patrol;
	m_delayNum = 0;
	//m_count = 0;
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

	m_enemyShotSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Shot_Player.wav");
	m_enemyShotSE.volume = DEFAULT_SHOT_VOLUME;

	m_footprintSpan = 0;

}

Enemy::~Enemy()
{
	bool debug = false;
}

void Enemy::SetData(
	DrawingByRasterize& arg_rasterize)
{
	if (m_positions.size() > 1)
	{
		//���f���f�[�^���
		m_enemyBox =
			std::make_unique<BasicDraw::BasicModelRender>(
				arg_rasterize,
				"Resource/Enemy/",
				"Move_Turret.gltf"
			);
	}
	else
	{
		//���f���f�[�^���
		m_enemyBox =
			std::make_unique<BasicDraw::BasicModelRender>(
				arg_rasterize,
				"Resource/Enemy/",
				"Gun.gltf"
			);

		m_pedestal =
			std::make_unique<BasicDraw::BasicModelRender>(
				arg_rasterize,
				"Resource/Enemy/",
				"Pedestal.gltf"
			);
	}

	m_meshCol = std::make_shared<MeshCollision>();
	m_meshCol->Setting(
		m_enemyBox.get()->m_model.
		m_modelInfo->modelData[0].vertexData,
		m_trans);


	for (auto& index : m_line) {
		index.Generate(arg_rasterize);
	}
	m_reaction.Load(arg_rasterize);
	m_shotDelay = 0;
	m_appearTimer = 0;

	m_inform.Load(arg_rasterize);

	m_gunEffect =
		std::make_shared<TurretFireEffect>(arg_rasterize);
}

void Enemy::Init(
	std::list<KazMath::Transform3D> arg_enemyList)
{
	m_positions = arg_enemyList;
	for (auto itr = m_positions.begin();
		itr != m_positions.end(); ++itr)
	{
		m_trans.pos = itr->pos;
		m_trans.scale = { 2.5f,2.5f,2.5f };
		break;
	}
	CalcMoveVec();

	m_state = State::Patrol;
	m_delayNum = 0;
	//m_count = 0;
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

	m_checkEyeDelay = MAX_EYE_DELAY;

	m_shotDelay = 0;
	m_isInSightFlag = false;

	m_footprintSpan = 0;

	m_shotDelay = 0;
	m_appearTimer = 0;

	m_currentPoint = 0;
	m_radian = 0.0f;

	m_sightRotation = 0;
}

void Enemy::Update(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Transform3D arg_playerTransform,
	std::weak_ptr<MeshCollision> arg_stageMeshCollision,
	HPUI& arg_hpUI)
{
	//���񂾂�I��
	if (IsDeath()) {
		Death();
		return;
	}

	//�v���C���[XZ���W
	std::pair<float, float> l_pPos =
		std::make_pair(
			arg_playerTransform.pos.x,
			arg_playerTransform.pos.z);

	//�O�t���[�����W(�ړ�����O�̍��W)��ۑ��B
	m_prevPos = m_trans.pos;

	//�����͈͓���
	m_isCombat = false;
	if (IsInSight(arg_playerTransform.pos, arg_stageColliders))
	{
		m_checkEyeDelay--;
		m_isInSightFlag = true;
		m_radian = atan2(
			m_trans.GetFront().z, m_trans.GetFront().x);

		RotateEye(arg_playerTransform.pos);

		//��莞�Ԕ͈͓���������
		if (m_checkEyeDelay <= 0)
		{
			m_rate = MAX_RATE;
			m_checkEyeDelay = MAX_EYE_DELAY;

			//���ʃx�N�g�����p�x��
			m_gunEffect->Init(
				&m_trans.pos,
				&m_radian,
				static_cast<float>(MAX_RATE));

			//������
			if (m_state != State::Combat)
			{
				m_isCombat = true;
				m_state = State::Combat;
			}
		}
	}
	else
	{
		m_isInSightFlag = false;
		m_checkEyeDelay = MAX_EYE_DELAY;

		if (IsFixedTurret()) {
			float l_rad =
				DirectX::XMConvertToRadians(2.0f);
			m_trans.Rotation(
				KazMath::Vec3<float>(0, 1, 0),
				l_rad);
		}
	}

	//����
	if (m_state == State::Patrol &&
		!m_isInSightFlag) {
		Patrol();
	}

	//�퓬��
	else if (m_state == State::Combat) {
		Combat(arg_bulletMgr, arg_playerTransform.pos);
	}

	//�ړ�����������(�ړ��^���b�g����)
	if (!IsFixedTurret()) {
		if (m_trans.pos != m_prevPos) {
			DirectX::XMVECTOR l_quaternion =
				CalMoveQuaternion(m_trans.pos, m_prevPos);
			m_trans.quaternion = l_quaternion;
		}
	}

	m_oldPos = m_trans.pos;

	//����(���b�V��)
	//Collision(arg_stageColliders, arg_bulletMgr);

	//�e���o
	m_gunEffect->Update();

	if (m_state != m_oldState)
	{
		switch (m_state)
		{
		case State::Patrol:
			break;
		case State::Warning:
			m_reaction.Init(EnemyReaction::WARING, { 0.0f,1.0f,0.0f }, KazMath::Color(255, 255, 255, 255));
			break;
		case State::Combat:
			m_reaction.Init(EnemyReaction::COMBAT, { 0.0f,1.0f,0.0f }, KazMath::Color(255, 255, 255, 255));
			break;
		case State::Holdup:
			break;
		case State::Death:
			break;
		default:
			break;
		}
	}
	m_oldState = m_state;

	m_reaction.Update(m_trans.pos + KazMath::Vec3<float>(0.0f, 5.0f, 0.0f));

	if (m_inEcho)
	{
		++m_appearTimer;
	}

	if (APPEAR_TIMER <= m_appearTimer)
	{
		m_appearTimer = 0;
		m_inEcho = false;
	}

	//����(�^�C����)���������ށB
	WriteFootprint();

	m_trans.GetFront();
	m_inform.Update(
		m_trans.pos,
		arg_playerTransform,
		m_state == State::Combat);

	//���ʕ����Ƀ��C���΂��āA�Ə����p�̃f�[�^���v�Z����B
	float minLength = (std::numeric_limits<float>::max)();
	bool isHit = false;
	KazMath::Transform3D checkHitTransform = m_trans;
	checkHitTransform.pos.y += 4.0f;
	for (auto itr = arg_stageColliders.begin(); itr != arg_stageColliders.end(); ++itr)
	{
		MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(checkHitTransform.pos, checkHitTransform.GetFront());

		//�ǂ����ɓ��������ꍇ
		if (!(rayResult.m_isHit && 0 < rayResult.m_distance && rayResult.m_distance < EnemyConfig::eyeCheckDist)) continue;

		//�������ׂ�
		if (minLength < rayResult.m_distance) continue;

		minLength = rayResult.m_distance;
		isHit = true;

	}
	if (isHit) {

		m_lineOfSightPos[0] = checkHitTransform.pos;
		m_lineOfSightPos[0].y -= 0.5f;
		m_lineOfSightPos[1] = checkHitTransform.pos + m_trans.GetFront() * minLength;
		m_lineOfSightPos[1].y -= 0.5f;

	}
	else {

		m_lineOfSightPos[0] = checkHitTransform.pos;
		m_lineOfSightPos[0].y -= 0.5f;
		m_lineOfSightPos[1] = checkHitTransform.pos + m_trans.GetFront() * EnemyConfig::eyeCheckDist;
		m_lineOfSightPos[1].y -= 0.5f;

	}
	m_sightRotation += 0.3f;

	//�퓬���Ő����̎��]�ɓG���v���C���[��������B
	float shortestDistance = KazMath::Vec3<float>(m_lineOfSightPos[1] - m_lineOfSightPos[0]).GetNormal().Dot(arg_playerTransform.pos - m_lineOfSightPos[0]) / KazMath::Vec3<float>(m_lineOfSightPos[1] - m_lineOfSightPos[0]).Length();
	float sphereDistance = ((KazMath::Vec3<float>(m_lineOfSightPos[1] - m_lineOfSightPos[0]) * shortestDistance) - KazMath::Vec3<float>(arg_playerTransform.pos - m_lineOfSightPos[0])).Length();
	if (m_state == State::Combat && sphereDistance < 4.0f && 0.0f < shortestDistance && shortestDistance < 1.0f) {
		arg_hpUI.HitDamage(1, 1);
	}

	m_inform.Update(m_trans.pos, arg_playerTransform, m_state == State::Combat);

}

void Enemy::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	m_reaction.Draw(arg_rasterize, arg_blasVec);
#ifdef DEBUG
	if (!m_inEcho)
	{
		return;
	}
#else
#endif // DEBUG

	m_enemyBox->m_model.DrawRasterize(
		arg_rasterize,
		m_trans, KazMath::Color(172, 50, 50, 255));

	if (IsFixedTurret())
	{
		KazMath::Transform3D l_trans;
		l_trans.scale = m_trans.scale;
		l_trans.pos = m_trans.pos;

		m_pedestal->m_model.DrawRasterize(
			arg_rasterize,
			l_trans, KazMath::Color(172, 50, 50, 255));
	}

	m_inform.Draw(arg_rasterize, arg_blasVec);

	m_gunEffect->Draw(arg_rasterize, arg_blasVec);

	if (0 < KazMath::Vec3<float>(m_lineOfSightPos[0] - m_lineOfSightPos[1]).Length()) {
		KazMath::Transform3D transform = m_trans;
		transform.Rotation(transform.GetFront(), m_sightRotation);
		KazMath::Vec3<float> right = transform.GetRight() * 0.05f;
		KazMath::Vec3<float> up = transform.GetUp() * 0.05f;
		m_line[0].m_render.Draw(arg_rasterize, arg_blasVec, m_lineOfSightPos[0] + right + up, m_lineOfSightPos[1] + right + up, KazMath::Color(172, 50, 50, 255));
		m_line[1].m_render.Draw(arg_rasterize, arg_blasVec, m_lineOfSightPos[0] - right + up, m_lineOfSightPos[1] - right + up, KazMath::Color(172, 50, 50, 255));
		m_line[2].m_render.Draw(arg_rasterize, arg_blasVec, m_lineOfSightPos[0] - right - up, m_lineOfSightPos[1] - right - up, KazMath::Color(172, 50, 50, 255));
		m_line[3].m_render.Draw(arg_rasterize, arg_blasVec, m_lineOfSightPos[0] + right - up, m_lineOfSightPos[1] + right - up, KazMath::Color(172, 50, 50, 255));
	}

}

bool Enemy::IsInSight(
	KazMath::Vec3<float>& arg_playerPos,
	std::list<std::shared_ptr<MeshCollision>>
	& arg_stageColliders)
{
	//�v���C���[XZ���W
	std::pair<float, float> l_pPos =
		std::make_pair(arg_playerPos.x, arg_playerPos.z);

	if (CheckDistXZ(
		l_pPos, EnemyConfig::eyeCheckDist) &&
		CheckEye(arg_playerPos, arg_stageColliders)) {
		return true;
	}
	return false;
}

void Enemy::CalcMoveVec()
{
	if (IsFixedTurret()) { return; }
	//m_inform.Draw(arg_rasterize);

	KazMath::Vec3<float> l_firstPos;
	KazMath::Vec3<float> l_basePos;
	int l_count = 0;

	for (auto itr = m_positions.begin();
		itr != m_positions.end(); ++itr)
	{
		//�ŏ�
		if (l_count == 0) {
			l_firstPos = itr->pos;
		}

		//���̏ꏊ
		if (l_count == m_currentPoint)
		{
			l_basePos = itr->pos;
			if (l_count + 1 == m_positions.size()) {
				m_nextPos = l_firstPos;
			}
			else {
				++itr;
				m_nextPos = itr->pos;
			}
			break;
		}
		l_count++;
	}

	KazMath::Vec3<float> l_vec =
		m_nextPos - l_basePos;
	float l_mag = sqrtf(
		powf(l_vec.x, 2.0f) +
		powf(l_vec.z, 2.0f));

	m_moveVec = l_vec / l_mag;
}

void Enemy::WriteFootprint()
{

	//�ړ�������
	float moveLength = KazMath::Vec3<float>(KazMath::Vec3<float>(m_trans.pos.x, 0.0f, m_trans.pos.z) - KazMath::Vec3<float>(m_prevPos.x, 0.0f, m_prevPos.z)).Length();

	//�ړ������ʂ�0�������珈�����΂��B
	if (moveLength < 0.01f) return;

	//���Ղ��������ŏo�����߂̃^�C�}�[���X�V�B
	++m_footprintSpan;
	const int FOOTPRINTSPAN = 2;
	if (m_footprintSpan < FOOTPRINTSPAN) return;

	//����F�ő��Ղ��o���̂ŁA�X�p��������������B
	m_footprintSpan = 0;

	//�n�ʂɈړ��B
	KazMath::Transform3D footprintTransform = m_trans;
	footprintTransform.pos.y += 0.5f;

	//�ړ��������������]���v�Z����B��x�N�g���͈�U�Œ�B
	KazMath::Vec3<float> axisX = KazMath::Vec3<float>(KazMath::Vec3<float>(m_trans.pos.x, 0.0f, m_trans.pos.z) - KazMath::Vec3<float>(m_prevPos.x, 0.0f, m_prevPos.z)).GetNormal();
	KazMath::Vec3<float> axisY = KazMath::Vec3<float>(0.0f, 1.0f, 0.0f);
	KazMath::Vec3<float> axisZ = axisX.Cross(axisY);
	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixIdentity();
	rotationMat.r[0] = { axisX.x, axisX.y, axisX.z, 0.0f };
	rotationMat.r[1] = { axisY.x, axisY.y, axisY.z, 0.0f };
	rotationMat.r[2] = { axisZ.x, axisZ.y, axisZ.z, 0.0f };
	footprintTransform.quaternion = DirectX::XMQuaternionRotationMatrix(rotationMat);

	//���Ղ𐶐��B
	FootprintMgr::Instance()->Generate(footprintTransform);

	//���Ղ̈ʒu�����炷�B
	footprintTransform.pos += KazMath::Vec3<float>(KazMath::Vec3<float>(m_trans.pos.x, 0.0f, m_trans.pos.z));

}

void Enemy::Move()
{
	if (IsFixedTurret()) { return; }

	std::pair<float, float> l_checkPos =
		std::make_pair(m_nextPos.x, m_nextPos.z);

	//�����ꍇ
	if (CheckDistXZ(l_checkPos, EnemyConfig::speed))
	{
		m_trans.pos = m_nextPos;

		m_currentPoint++;
		if (m_currentPoint > m_positions.size() - 1) {
			m_currentPoint = 0;
		}
		m_isCheckPoint = true;
		CalcMoveVec();
	}
	//�ʏ�ړ�
	else {
		m_trans.pos += m_moveVec * EnemyConfig::speed;
	}
}

void Enemy::Patrol()
{
	//�`�F�b�N�|�C���g
	if (m_isCheckPoint)
	{
		m_delay++;
		RotateEye(m_nextPos);

		if (m_delay ==
			CHECK_POINT_DELAY)
		{
			m_delay = 0;
			m_isCheckPoint = false;
		}
	}

	//�ʏ�
	else
	{
		Move();
	}
}

void Enemy::Combat(
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Vec3<float> arg_playerPos)
{
	if (!m_isInSightFlag)
	{
		m_rate--;
		if (m_rate < 0)
		{
			m_state = State::Patrol;
			m_rate = MAX_RATE;
		}
	}
	else {
		//�v���C���[����
		RotateEye(arg_playerPos);

		m_rate = MAX_RATE;
		m_shotDelay++;
	}

	//�ˌ�
	if (SHOT_DELAY < m_shotDelay) {

		arg_bulletMgr.lock()->
			GenerateEnemyBullet(m_trans.pos, m_trans.GetFront());
		m_shotDelay = 0;

		//�����ɂ���ĉ�������������B
		float distance = KazMath::Vec3<float>(arg_playerPos - m_trans.pos).Length();
		float range = 1.0f - std::clamp(distance / SOUND_RANGE, 0.0f, 1.0f);
		range = EasingMaker(In, Quad, range);

		m_enemyShotSE.volume = DEFAULT_SHOT_VOLUME * range;
		SoundManager::Instance()->
			SoundPlayerWave(m_enemyShotSE, 0);
	}
}

void Enemy::Death()
{

}

DirectX::XMVECTOR Enemy::CalMoveQuaternion(
	KazMath::Vec3<float> arg_pos,
	KazMath::Vec3<float> arg_prevPos)
{
	//Y�������̈ړ��ʂ����������B
	arg_pos.y = 0;
	arg_prevPos.y = 0;

	if (KazMath::Vec3<float>(arg_pos - arg_prevPos).Length() <= 0.01f) {
		return m_trans.quaternion;
	}

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

void Enemy::RotateEye(
	KazMath::Vec3<float>& arg_playerPos)
{
	//�G����v���C���[����
	KazMath::Vec3<float> l_vec =
		arg_playerPos - m_trans.pos;
	float l_mag = sqrtf(
		powf(l_vec.x, 2.0f) +
		powf(l_vec.z, 2.0f));
	KazMath::Vec3<float> l_trans = l_vec / l_mag;
	//2�̃x�N�g���̊O��(xz���ʂ���y�Ɍ��ʂ��o��)
	KazMath::Vec3<float> l_cross =
		m_trans.GetFront().Cross(l_trans);
	//2�̃x�N�g���̓���(1.0f���Ɛ���)
	if (m_trans.GetFront().Dot(l_trans) < 1.0f)
	{
		float l_rad =
			DirectX::XMConvertToRadians(0.8f);

		if (l_cross.y < 0.0f) {
			l_rad *= -1.0f;
			m_trans.Rotation(
				KazMath::Vec3<float>(0, 1, 0),
				l_rad);
		}
		else {
			m_trans.Rotation(
				KazMath::Vec3<float>(0, 1, 0),
				l_rad);
		}
	}
}

//-----����n-----//
void Enemy::Collision(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr)
{

	m_meshCol->Setting(
		m_enemyBox.get()->m_model.
		m_modelInfo->modelData[0].vertexData,
		m_trans);

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

	//�G�{�̂ƒe�̓����蔻����s���B
	int hitCount = arg_bulletMgr.lock()->CheckMeshCollision(m_meshCol, true);
	if (0 < hitCount) {
		--m_hp;
		//m_hp -= 50;
	}

	if (m_hp <= 0) {
		m_state = State::Death;
	}

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