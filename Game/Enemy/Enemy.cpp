#include "Enemy.h"
#include "EnemyConfig.h"
#include "../Game/Bullet/BulletMgr.h"
#include "../Footprint/FootprintMgr.h"
#include "../Game/Effect/TurretFireEffect.h"

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
	m_enemyShotSE.volume = 0.05f;

	m_footprintSpan = 0;

}

Enemy::~Enemy()
{
	bool debug = false;
}

void Enemy::SetData(
	DrawingByRasterize& arg_rasterize)
{
	//モデルデータ代入
	m_enemyBox =
		std::make_unique<BasicDraw::BasicModelRender>(
			arg_rasterize,
			"Resource/Enemy/",
			"Turret.gltf"
		);

	m_meshCol = std::make_shared<MeshCollision>();
	m_meshCol->Setting(
		m_enemyBox.get()->m_model.
		m_modelInfo->modelData[0].vertexData,
		m_trans);


	m_line.Generate(arg_rasterize);
	m_reaction.Load(arg_rasterize);
	m_shotDelay = 0;
	m_appearTimer = 0;

	m_inform.Load(arg_rasterize);
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
}

void Enemy::Update(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Transform3D arg_playerTransform,
	std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{
	//前フレーム座標(移動する前の座標)を保存。
	m_prevPos = m_trans.pos;

	//死んだら終了
	if (m_state == State::Death)
	{
		return;
	}

	//プレイヤーXZ座標
	std::pair<float, float> l_pPos =
		std::make_pair(arg_playerTransform.pos.x, arg_playerTransform.pos.z);

	//視線範囲内か
	m_isCombat = false;
	if (CheckDistXZ(
		l_pPos, EnemyConfig::eyeCheckDist) &&
		CheckEye(arg_playerTransform.pos, arg_stageColliders))
	{
		m_checkEyeDelay--;

		//一定時間範囲内だったら
		if (m_checkEyeDelay <= 0)
		{
			m_rate = MAX_RATE;
			m_checkEyeDelay = MAX_EYE_DELAY;
			//発見時
			if (m_state != State::Combat)
			{
				m_isCombat = true;
				m_state = State::Combat;
			}
		}
		m_isInSightFlag = true;
	}
	else
	{
		m_isInSightFlag = false;
		m_checkEyeDelay = MAX_EYE_DELAY;
	}

	//巡回
	if (m_state == State::Patrol &&
		!m_isInSightFlag)
	{
		//チェックポイント
		if (m_isCheckPoint)
		{
			m_delay++;
			if (m_delay ==
				CHECK_POINT_DELAY)
			{
				m_delay = 0;
				m_isCheckPoint = false;
			}
		}

		//通常
		else
		{
			Move();
		}
	}

	//戦闘中
	else if (m_state == State::Combat)
	{
		//視線範囲内なら向きながら射撃
		if (CheckEye(arg_playerTransform.pos, arg_stageColliders))
		{
			//プレイヤー方向
			m_trans.quaternion = CalMoveQuaternion(arg_playerTransform.pos, m_trans.pos);

			//射撃
			++m_shotDelay;
			if (SHOT_DELAY < m_shotDelay) {

				arg_bulletMgr.lock()->
					GenerateEnemyBullet(m_trans.pos, m_trans.GetFront());
				m_shotDelay = 0;
				SoundManager::Instance()->
					SoundPlayerWave(m_enemyShotSE, 0);
			}
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

	//回転
	if (m_trans.pos != m_prevPos) {
		DirectX::XMVECTOR l_quaternion =
			CalMoveQuaternion(m_trans.pos, m_prevPos);
		m_trans.quaternion = l_quaternion;
	}


	//if (m_state == State::Patrol ||
	//	m_state == State::Warning) {
	//	//RotateEye();
	//}

	//重力(仮)
	if (!m_onGround) {
		m_gravity -= GRAVITY;
	}
	else {
		m_gravity = 0;
	}
	//m_trans.pos.y += m_gravity;

	m_oldPos = m_trans.pos;

	//判定(メッシュ)
	//Collision(arg_stageColliders, arg_bulletMgr);

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



	//足跡(タイヤ痕)を書き込む。
	WriteFootprint();



	m_trans.GetFront();
	m_inform.Update(m_trans.pos, arg_playerTransform);

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

	if (m_state != State::Death)
	{
		KazMath::Color l_player = { 172, 50, 50, 255 };

		m_enemyBox->m_model.DrawRasterize(
			arg_rasterize,
			m_trans,
			l_player);
	}

	m_inform.Draw(arg_rasterize, arg_blasVec);

}

void Enemy::CalcMoveVec()
{
	if (m_positions.size() <= 1) { return; }


	KazMath::Vec3<float> l_firstPos;
	KazMath::Vec3<float> l_basePos;
	int l_count = 0;

	for (auto itr = m_positions.begin();
		itr != m_positions.end(); ++itr)
	{
		//最初
		if (l_count == 0) {
			l_firstPos = itr->pos;
		}

		//今の場所
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

	//移動した量
	float moveLength = KazMath::Vec3<float>(KazMath::Vec3<float>(m_trans.pos.x, 0.0f, m_trans.pos.z) - KazMath::Vec3<float>(m_prevPos.x, 0.0f, m_prevPos.z)).Length();

	//移動した量が0だったら処理を飛ばす。
	if (moveLength < 0.01f) return;

	//足跡を一定周期で出すためのタイマーを更新。
	++m_footprintSpan;
	const int FOOTPRINTSPAN = 2;
	if (m_footprintSpan < FOOTPRINTSPAN) return;

	//このFで足跡を出すので、スパンを初期化する。
	m_footprintSpan = 0;

	//地面に移動。
	KazMath::Transform3D footprintTransform = m_trans;
	footprintTransform.pos.y += 0.5f;

	//移動した方向から回転を計算する。上ベクトルは一旦固定。
	KazMath::Vec3<float> axisX = KazMath::Vec3<float>(KazMath::Vec3<float>(m_trans.pos.x, 0.0f, m_trans.pos.z) - KazMath::Vec3<float>(m_prevPos.x, 0.0f, m_prevPos.z)).GetNormal();
	KazMath::Vec3<float> axisY = KazMath::Vec3<float>(0.0f, 1.0f, 0.0f);
	KazMath::Vec3<float> axisZ = axisX.Cross(axisY);
	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixIdentity();
	rotationMat.r[0] = { axisX.x, axisX.y, axisX.z, 0.0f };
	rotationMat.r[1] = { axisY.x, axisY.y, axisY.z, 0.0f };
	rotationMat.r[2] = { axisZ.x, axisZ.y, axisZ.z, 0.0f };
	footprintTransform.quaternion = DirectX::XMQuaternionRotationMatrix(rotationMat);

	//足跡を生成。
	FootprintMgr::Instance()->Generate(footprintTransform);

	//足跡の位置をずらす。
	footprintTransform.pos += KazMath::Vec3<float>(KazMath::Vec3<float>(m_trans.pos.x, 0.0f, m_trans.pos.z));

}

void Enemy::Move()
{
	//m_trans.pos.y = 23.0f;
	if (m_positions.size() <= 1) { return; }

	std::pair<float, float> l_checkPos =
		std::make_pair(m_nextPos.x, m_nextPos.z);

	//着く場合
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
	//通常移動
	else {
		m_trans.pos += m_moveVec * EnemyConfig::speed;
	}
}

DirectX::XMVECTOR Enemy::CalMoveQuaternion(
	KazMath::Vec3<float> arg_pos,
	KazMath::Vec3<float> arg_prevPos)
{
	//Y軸方向の移動量を書き消す。
	arg_pos.y = 0;
	arg_prevPos.y = 0;

	if (KazMath::Vec3<float>(arg_pos - arg_prevPos).Length() <= 0.01f) {
		return m_trans.quaternion;
	}

	//動いた方向に傾ける。
	KazMath::Vec3<float> moveVec = arg_pos - arg_prevPos;
	//動いた方向ベクトルを正規化する。
	moveVec.Normalize();

	//デフォルトの正面ベクトルからの回転量を求める。
	float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(moveVec));

	//今向いている方向がデフォルトの方向だったらゼロ除算が発生するので、ベクトルを仮置きで置く。
	KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(moveVec);
	if (cross.Length() <= 0) {
		cross = { 0,1,0 };
	}

	//クォータニオンを求める。
	return DirectX::XMQuaternionRotationAxis({
		cross.x,
		cross.y,
		cross.z }, angle);
}

void Enemy::RotateEye()
{
	//進行先
	//m_oldQuaternion = m_trans.quaternion;

	//30度
	m_angle += 0.01f;
	float l_rad = DirectX::XMConvertToRadians(30.0f);
	l_rad *= std::sinf(m_angle);

	//ラジアンを加算
	KazMath::Transform3D l_trans = m_trans;
	l_trans.Rotation(
		KazMath::Vec3<float>(0, 1, 0),
		l_rad);

	m_trans.quaternion = l_trans.quaternion;
}

//-----判定系-----//
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

	//地面と当たり判定を行う。
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
			//押し戻し。
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH + GROUND_RAY_OFFSET -
					rayResult.m_distance);
			m_onGround = true;
		}

		//当たり判定を計算。
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			m_trans.GetFront());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//押し戻し。
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}

		//後ろ方向
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			-m_trans.GetFront());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//押し戻し。
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}

		//右方向
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			m_trans.GetRight());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//押し戻し。
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}

		//左方向
		rayResult = (*itr)->CheckHitRay(
			m_trans.pos,
			-m_trans.GetRight());
		if (rayResult.m_isHit &&
			0.0f < rayResult.m_distance &&
			rayResult.m_distance <= RAY_LENGTH)
		{
			//押し戻し。
			m_trans.pos +=
				rayResult.m_normal *
				(RAY_LENGTH - rayResult.m_distance);
		}
	}

	//敵本体と弾の当たり判定を行う。
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

	//メッシュ判定
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

		//当たらなかった場合
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