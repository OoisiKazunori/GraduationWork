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

	m_footprintSpan = 0;

	m_edgeColor = ACTIVE_COLOR;

	//新規
	m_moveVec = { 0.0f,0.0f,0.0f };
	m_nextPos = { 0.0f,0.0f,0.0f };
	m_firePos = { 0.0f,0.0f,0.0f };
	m_currentPoint = 0;
	m_radian = 0.0f;
	m_checkEyeDelay = 0.0f;
	m_distRate = 1.0f;
	m_isGunEffect = false;
	m_isHit = false;
	m_isRotate = false;

	//音
	m_sounds[0] = SoundManager::Instance()->
		SoundLoadWave("Resource/Sound/Enemy/Turret_SE_1_.wav");
	m_sounds[1] = SoundManager::Instance()->
		SoundLoadWave("Resource/Sound/Enemy/Turret_SE_2_.wav");
	m_sounds[2] = SoundManager::Instance()->
		SoundLoadWave("Resource/Sound/Enemy/Turret_SE_3_.wav");
	m_sounds[3] = SoundManager::Instance()->
		SoundLoadWave("Resource/Sound/Enemy/Turret_SE_4_.wav");
	m_sounds[4] = SoundManager::Instance()->
		SoundLoadWave("Resource/Sound/Enemy/Hit_SE_1_.wav");
	for (int i = 0; i < m_sounds.size(); ++i)
	{
		m_sounds[i].volume = DEFAULT_SHOT_VOLUME;

		SoundManager::Instance()->SoundPlayerWave(
			m_sounds[i],
			0
		);
		m_sounds[i].source->Stop();
		m_isSounds[i] = false;
	}
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
			"Gun.gltf"
		);

	if (!IsFixedTurret())
	{
		m_pedestal =
			std::make_unique<BasicDraw::BasicModelRender>(
				arg_rasterize,
				"Resource/Enemy/",
				"Move_Pedestal.gltf"
			);
	}
	else
	{
		m_pedestal =
			std::make_unique<BasicDraw::BasicModelRender>(
				arg_rasterize,
				"Resource/Enemy/",
				"Pedestal.gltf"
			);
	}
	m_collision =
		std::make_unique<BasicDraw::BasicModelRender>(
			arg_rasterize,
			"Resource/Enemy/",
			"Collision.gltf"
		);


	for (auto& index : m_collision.get()->m_model.m_modelInfo->modelData) {

		m_meshCol.emplace_back(std::make_shared<MeshCollision>());
		m_meshCol.back()->Setting(index.vertexData, m_trans);
		m_meshColVertex.emplace_back(index.vertexData);

	}



	//m_line.Generate(arg_rasterize);
	for (auto& index : m_line) {
		index.Generate(arg_rasterize);
	}
	m_reaction.Load(arg_rasterize);
	m_shotDelay = 0;
	m_appearTimer = 0;

	m_inform.Load(arg_rasterize);

	m_gunEffect =
		std::make_shared<TurretFireEffect>(arg_rasterize);
	m_smokeEffect =
		std::make_shared<SmokeEmitter>();
	m_smokeEffect->Load(arg_rasterize);
	m_hitEffect.Load(arg_rasterize);
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
	m_trans.quaternion =
		CalMoveQuaternion(m_nextPos, m_trans.pos);

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

	m_shotDelay = 0;
	m_isInSightFlag = false;

	m_footprintSpan = 0;

	m_shotDelay = 0;
	m_appearTimer = 0;

	m_sightRotation = 0;

	//新規
	m_firePos = m_trans.pos;
	m_currentPoint = 0;
	m_radian = 0.0f;
	m_checkEyeDelay = 0.0f;
	m_distRate = 1.0f;
	m_isGunEffect = false;
	m_isHit = false;
	m_isRotate = false;

	SoundManager::Instance()->SoundPlayerWave(
		m_sounds[Sounds::Turret_SE_2],
		XAUDIO2_LOOP_INFINITE);
	m_sounds[Sounds::Turret_SE_2].source->SetVolume(0.0f);
	m_isSounds[Sounds::Turret_SE_2] = true;

	m_inform.Init();

	m_stelsKillFlag = false;
}

void Enemy::Update(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Transform3D &arg_playerTransform,
	std::weak_ptr<MeshCollision> arg_stageMeshCollision,
	HPUI& arg_hpUI)
{
	//銃演出
	m_gunEffect->Update();

	//体力減ったら
	if (m_hp != m_oldHp)
	{
		m_hitEffect.Init(m_trans.pos + KazMath::Vec3<float>(0.0f, 2.5f, 0.0f), &arg_playerTransform.pos, KazMath::Color(255, 0, 0, 255));
	}
	m_oldHp = m_hp;
	m_hitEffect.Update();

	//死んだら終了
	if (IsDeath()) {
		Death();

		//死んだときの色に近づける。
		m_edgeColor.color.x += static_cast<int>((DEAD_COLOR.color.x - m_edgeColor.color.x) / 10.0f);
		m_edgeColor.color.y += static_cast<int>((DEAD_COLOR.color.y - m_edgeColor.color.y) / 10.0f);
		m_edgeColor.color.z += static_cast<int>((DEAD_COLOR.color.z - m_edgeColor.color.z) / 10.0f);

		m_inform.Update(m_trans.pos, arg_playerTransform, false);

		return;
	}

	//プレイヤーXZ座標
	std::pair<float, float> l_pPos =
		std::make_pair(
			arg_playerTransform.pos.x,
			arg_playerTransform.pos.z);

	//前フレーム座標(移動する前の座標)を保存。
	m_prevPos = m_trans.pos;

	//視線範囲内か
	m_isCombat = false;
	m_radian = atan2(
		m_trans.GetFront().z, m_trans.GetFront().x);

	//戦闘中で線分の周転に敵がプレイヤーが居たら。
	float shortestDistance = KazMath::Vec3<float>(m_lineOfSightPos[1] - m_lineOfSightPos[0]).GetNormal().Dot(arg_playerTransform.pos - m_lineOfSightPos[0]) / KazMath::Vec3<float>(m_lineOfSightPos[1] - m_lineOfSightPos[0]).Length();
	float sphereDistance = ((KazMath::Vec3<float>(m_lineOfSightPos[1] - m_lineOfSightPos[0]) * shortestDistance) - KazMath::Vec3<float>(arg_playerTransform.pos - m_lineOfSightPos[0])).Length();
	if (m_state == State::Combat && sphereDistance < 4.0f && 0.0f < shortestDistance && shortestDistance < 1.0f)
	{
		arg_hpUI.HitDamage(1, 1);
	}

	//音量変更
	float distance =
		KazMath::Vec3<float>(
			arg_playerTransform.pos - m_trans.pos).Length();
	float range = 1.0f - std::clamp(distance / SOUND_RANGE, 0.0f, 1.0f);
	range = EasingMaker(In, Quad, range);
	for (int i = 0; i < m_sounds.size(); ++i)
	{
		float l_vol = DEFAULT_SHOT_VOLUME * range;
		if (arg_hpUI.GetHP() <= 0) {
			l_vol = 0.0f;
		}
		m_sounds[i].source->SetVolume(l_vol);
	}

	//判定
	if (IsInSight(arg_playerTransform.pos, arg_stageColliders))
	{
		m_checkEyeDelay += 1.0f;
		m_isInSightFlag = true;
		m_isRotate = true;

		RotateEye(arg_playerTransform.pos);
		if (!m_isSounds[Sounds::Turret_SE_3]) {
			SoundManager::Instance()->SoundPlayerWave(
				m_sounds[Sounds::Turret_SE_3],
				XAUDIO2_LOOP_INFINITE);
			m_isSounds[Sounds::Turret_SE_3] = true;
		}

		//一定時間範囲内だったら
		if (m_checkEyeDelay >=
			MAX_EYE_DELAY * m_distRate)
		{
			m_rate = MAX_RATE;
			m_checkEyeDelay = 0;

			//正面ベクトルを角度に
			m_firePos = m_trans.pos;
			m_firePos.y = m_trans.pos.y + 3.0f;
			m_gunEffect->Init(
				&m_firePos,
				&m_radian,
				static_cast<float>(MAX_EYE_DELAY));
			m_isGunEffect = true;

			//発見時
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
		m_checkEyeDelay = 0.0f;
		m_distRate = 1.0f;

		m_gunEffect->Stop();
		m_isGunEffect = false;

		if (m_isSounds[Sounds::Turret_SE_3]) {
			m_sounds[Sounds::Turret_SE_3].source->Stop();
			m_isSounds[Sounds::Turret_SE_3] = false;
		}

		if (IsFixedTurret()) {
			float l_rad =
				DirectX::XMConvertToRadians(2.0f);
			m_trans.Rotation(
				KazMath::Vec3<float>(0, 1, 0),
				l_rad);
		}
	}

	//巡回時
	if (m_state == State::Patrol &&
		!m_isInSightFlag) {
		Patrol();
	}

	//戦闘時
	else if (m_state == State::Combat) {
		Combat(arg_bulletMgr, arg_playerTransform.pos);
	}

	m_oldPos = m_trans.pos;
	m_pedestalTrans = m_trans;

	//判定(メッシュ)
	Collision(arg_stageColliders, arg_bulletMgr);

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
	m_inform.Update(
		m_trans.pos,
		arg_playerTransform,
		m_state == State::Combat);

	//正面方向にレイを飛ばして、照準線用のデータを計算する。
	float minLength = (std::numeric_limits<float>::max)();
	bool isHit = false;
	KazMath::Transform3D checkHitTransform = m_trans;
	checkHitTransform.pos.y += 4.0f;
	for (auto itr = arg_stageColliders.begin(); itr != arg_stageColliders.end(); ++itr)
	{
		MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(checkHitTransform.pos, checkHitTransform.GetFront());

		//どこかに当たった場合
		if (!(rayResult.m_isHit && 0 < rayResult.m_distance && rayResult.m_distance < EnemyConfig::eyeCheckDist)) continue;

		//距離を比べる
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

	m_inform.Update(m_trans.pos, arg_playerTransform, m_state == State::Combat);

}

void Enemy::Draw(
	DrawingByRasterize& arg_rasterize,
	Raytracing::BlasVector& arg_blasVec)
{
	if (m_state == State::Combat) {
		m_reaction.Draw(arg_rasterize, arg_blasVec);
	}

#ifdef DEBUG
	if (!m_inEcho)
	{
		return;
	}
#else
#endif // DEBUG

	if (IsDeath())
	{
		bool debug = false;
	}

	m_enemyBox->m_model.DrawRasterize(
		arg_rasterize,
		m_trans, m_edgeColor);

	KazMath::Transform3D l_trans;
	l_trans.scale = m_trans.scale;
	l_trans.pos = m_trans.pos;
	if (!IsFixedTurret()) {
		l_trans.quaternion = m_pedestalTrans.quaternion;
	}
	m_pedestal->m_model.DrawRasterize(
		arg_rasterize,
		l_trans, m_edgeColor);

	m_inform.Draw(arg_rasterize, arg_blasVec);
	m_gunEffect->Draw(arg_rasterize, arg_blasVec);
	m_hitEffect.Draw(arg_rasterize, arg_blasVec);

	if (m_state == State::Death) {
		m_smokeEffect->Draw(arg_rasterize, arg_blasVec);
	}
	else {
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

	//m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_lineOfSightPos[0], m_lineOfSightPos[1], KazMath::Color(172, 50, 50, 255));
}

bool Enemy::IsInSight(
	KazMath::Vec3<float>& arg_playerPos,
	std::list<std::shared_ptr<MeshCollision>>
	& arg_stageColliders)
{
	//プレイヤーXZ座標
	std::pair<float, float> l_pPos =
		std::make_pair(arg_playerPos.x, arg_playerPos.z);

	bool l_isCheckDist = false;
	float l_dist = std::sqrtf(
		std::powf(m_trans.pos.x - l_pPos.first, 2.0f) +
		std::powf(m_trans.pos.z - l_pPos.second, 2.0f));

	if (l_dist < EnemyConfig::eyeCheckDist) {
		l_isCheckDist = true;
	}

	if (l_isCheckDist &&
		CheckEye(arg_playerPos, arg_stageColliders)) {
		m_distRate = l_dist / EnemyConfig::eyeCheckDist;
		return true;
	}
	return false;
}

void Enemy::Kill()
{

	m_hp = 0;
	SoundManager::Instance()->
		SoundPlayerWave(
			m_sounds[Sounds::Hit_SE_1], 0);

}

void Enemy::StelsDamage(int arg_damage)
{
	if (!m_isSounds[Sounds::Turret_SE_3]) {
		SoundManager::Instance()->SoundPlayerWave(
			m_sounds[Sounds::Turret_SE_3],
			XAUDIO2_LOOP_INFINITE);
		m_isSounds[Sounds::Turret_SE_3] = true;
	}
	m_state = State::Combat;

	m_hp = std::clamp(m_hp - arg_damage, 0, 1000);
	SoundManager::Instance()->
		SoundPlayerWave(
			m_sounds[Sounds::Hit_SE_1], 0);

	m_stelsKillFlag = true;
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
	if (IsFixedTurret()) { return; }

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

void Enemy::Patrol()
{
	//チェックポイント
	if (m_isCheckPoint)
	{
		m_delay++;
		RotateEye(m_nextPos);
		m_stelsKillFlag = false;

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
		if (m_isRotate) {
			m_isRotate = RotateEye(m_nextPos);
		}
		else {
			Move();
		}
	}
}

void Enemy::Combat(
	std::weak_ptr<BulletMgr> arg_bulletMgr,
	KazMath::Vec3<float> arg_playerPos)
{
	//プレイヤー方向(撃たないけど見続ける)
	RotateEye(arg_playerPos);
	m_stelsKillFlag = true;
	m_isRotate = true;

	if (!m_isInSightFlag)
	{
		m_rate--;
		if (m_rate < 0)
		{
			m_state = State::Patrol;
			m_rate = MAX_RATE;
			m_isHit = false;
			m_stelsKillFlag = false;
		}

		//音
		if (m_isSounds[Sounds::Turret_SE_1]) {
			m_sounds[Sounds::Turret_SE_1].source->Stop();
			m_isSounds[Sounds::Turret_SE_1] = false;
		}
	}
	else {
		m_rate = MAX_RATE;
		m_shotDelay++;

		if (m_isHit) {
			m_isCombat = true;
			m_isHit = false;
		}

		//射撃演出
		if (!m_isGunEffect) {
			m_gunEffect->Init(
				&m_firePos,
				&m_radian,
				static_cast<float>(MAX_EYE_DELAY));
			m_isGunEffect = true;
		}

		//音
		if (!m_isSounds[Sounds::Turret_SE_1]) {
			SoundManager::Instance()->SoundPlayerWave(
				m_sounds[Sounds::Turret_SE_1],
				XAUDIO2_LOOP_INFINITE);
			m_isSounds[Sounds::Turret_SE_1] = true;
		}
	}

	//射撃
	if (SHOT_DELAY < m_shotDelay) {

		//arg_bulletMgr.lock()->
		//    GenerateEnemyBullet(m_trans.pos, m_trans.GetFront());
		m_shotDelay = 0;
	}
}

void Enemy::Death()
{
	m_smokeEffect->Update();

	if (m_deathTurretTimer < 1.0f)
	{
		float l_frame = 1.0f / 60.0f;
		m_deathTurretTimer += l_frame;
		if (m_deathTurretTimer > 1.0f) {
			m_deathTurretTimer = 1.0f;
		}

		float l_rad = DirectX::XMConvertToRadians(1.7f);
		l_rad *= EasingMaker(In, Quad, m_deathTurretTimer);
		m_trans.Rotation(
			m_pedestalTrans.GetRight(),
			l_rad);
	}

	if (IsDead())
	{
		m_sounds[Sounds::Turret_SE_3].source->Stop();
		m_isSounds[Sounds::Turret_SE_3] = false;
		m_sounds[Sounds::Turret_SE_1].source->Stop();
		m_isSounds[Sounds::Turret_SE_1] = false;
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

bool Enemy::RotateEye(
	KazMath::Vec3<float>& arg_playerPos)
{
	//敵からプレイヤー方向
	KazMath::Vec3<float> l_vec =
		arg_playerPos - m_trans.pos;
	float l_mag = sqrtf(
		powf(l_vec.x, 2.0f) +
		powf(l_vec.z, 2.0f));
	KazMath::Vec3<float> l_trans = l_vec / l_mag;
	//2つのベクトルの外積(xz平面だとyに結果が出る)
	KazMath::Vec3<float> l_cross =
		m_trans.GetFront().Cross(l_trans);
	//2つのベクトルの内積(1.0fだと正面)
	float l_dot = m_trans.GetFront().Dot(l_trans);
	if (l_dot < 0.99999f)
	{
		float add = 1.0f;
		if (m_stelsKillFlag)
		{
			add = 5.0f;
		}

		l_dot = acosf(l_dot);
		float l_rad =
			std::clamp(
				DirectX::XMConvertToRadians(add),
				0.0f,
				l_dot);

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

		return true;
	}

	return false;
}

//-----判定系-----//
void Enemy::Collision(
	std::list<std::shared_ptr<MeshCollision>>
	arg_stageColliders,
	std::weak_ptr<BulletMgr> arg_bulletMgr)
{
	const int MESH_COUNT = static_cast<int>(m_meshCol.size());
	for (int index = 0; index < MESH_COUNT; ++index) {
		m_meshCol[index]->Setting(m_meshColVertex[index], m_trans);
	}

	//const float RAY_LENGTH = 8.0f;

	////地面と当たり判定を行う。
	//m_onGround = false;

	//KazMath::Vec3<float> aaa = -m_trans.GetUp();

	//const float GROUND_RAY_OFFSET = 5.0f;
	//for (auto itr = arg_stageColliders.begin();
	//	itr != arg_stageColliders.end(); ++itr) {

	//	MeshCollision::CheckHitResult rayResult =
	//		(*itr)->CheckHitRay(
	//			m_trans.pos +
	//			m_trans.GetUp() *
	//			GROUND_RAY_OFFSET,
	//			-m_trans.GetUp());

	//	if (rayResult.m_isHit &&
	//		0.0f < rayResult.m_distance &&
	//		rayResult.m_distance <=
	//		RAY_LENGTH + GROUND_RAY_OFFSET)
	//	{
	//		//押し戻し。
	//		m_trans.pos +=
	//			rayResult.m_normal *
	//			(RAY_LENGTH + GROUND_RAY_OFFSET -
	//				rayResult.m_distance);
	//		m_onGround = true;
	//	}

	//	//当たり判定を計算。
	//	rayResult = (*itr)->CheckHitRay(
	//		m_trans.pos,
	//		m_trans.GetFront());
	//	if (rayResult.m_isHit &&
	//		0.0f < rayResult.m_distance &&
	//		rayResult.m_distance <= RAY_LENGTH)
	//	{
	//		//押し戻し。
	//		m_trans.pos +=
	//			rayResult.m_normal *
	//			(RAY_LENGTH - rayResult.m_distance);
	//	}

	//	//後ろ方向
	//	rayResult = (*itr)->CheckHitRay(
	//		m_trans.pos,
	//		-m_trans.GetFront());
	//	if (rayResult.m_isHit &&
	//		0.0f < rayResult.m_distance &&
	//		rayResult.m_distance <= RAY_LENGTH)
	//	{
	//		//押し戻し。
	//		m_trans.pos +=
	//			rayResult.m_normal *
	//			(RAY_LENGTH - rayResult.m_distance);
	//	}

	//	//右方向
	//	rayResult = (*itr)->CheckHitRay(
	//		m_trans.pos,
	//		m_trans.GetRight());
	//	if (rayResult.m_isHit &&
	//		0.0f < rayResult.m_distance &&
	//		rayResult.m_distance <= RAY_LENGTH)
	//	{
	//		//押し戻し。
	//		m_trans.pos +=
	//			rayResult.m_normal *
	//			(RAY_LENGTH - rayResult.m_distance);
	//	}

	//	//左方向
	//	rayResult = (*itr)->CheckHitRay(
	//		m_trans.pos,
	//		-m_trans.GetRight());
	//	if (rayResult.m_isHit &&
	//		0.0f < rayResult.m_distance &&
	//		rayResult.m_distance <= RAY_LENGTH)
	//	{
	//		//押し戻し。
	//		m_trans.pos +=
	//			rayResult.m_normal *
	//			(RAY_LENGTH - rayResult.m_distance);
	//	}
	//}

	//敵本体と弾の当たり判定を行う。
	int hitCount = 0;

	for (auto& index : m_meshCol) {

		hitCount += arg_bulletMgr.lock()->CheckMeshCollision(index, true);

	}
	if (0 < hitCount) {
		--m_hp;

		if (m_state == State::Patrol) {
			m_isHit = true;
		}
		m_state = State::Combat;

		SoundManager::Instance()->
			SoundPlayerWave(
				m_sounds[Sounds::Hit_SE_1], 0);
	}

	//死亡
	if (m_hp <= 0) {
		m_firePos = m_trans.pos;
		m_firePos.y = m_trans.pos.y + 3.0f;

		SmokeEmitter::EmittData l_emittData;
		l_emittData.m_emittPos = m_firePos;
		l_emittData.m_range = { 1.0f,0.1f,1.0f };
		l_emittData.m_smokeTime = 6000;
		m_smokeEffect->Init(l_emittData);
		m_state = State::Death;

		m_sounds[Sounds::Turret_SE_2].source->Stop();
		m_isSounds[Sounds::Turret_SE_2] = false;
		SoundManager::Instance()->
			SoundPlayerWave(
				m_sounds[Sounds::Turret_SE_4], 0);
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