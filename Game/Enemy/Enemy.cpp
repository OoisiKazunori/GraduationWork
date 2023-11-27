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
}

Enemy::~Enemy()
{
}

void Enemy::SetData(
	DrawingByRasterize& arg_rasterize)
{
	////モデルデータ代入
	//m_enemyBox =
	//	std::make_unique<BasicDraw::BasicModelRender>(
	//		arg_rasterize,
	//		"Resource/cubeFrame/",
	//		"cubeFrame.gltf"
	//		);

	//モデルデータ代入
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

void Enemy::Collision(
	std::weak_ptr<MeshCollision> arg_meshCollision)
{
	const float RAY_LENGTH = 5.0f;

	//地面と当たり判定を行う。
	m_onGround = false;
	const float GROUND_RAY_OFFSET = 5.0f;
	MeshCollision::CheckHitResult rayResult =
		arg_meshCollision.lock()->CheckHitRay(
			m_trans.pos +
			m_trans.GetUp() *
			GROUND_RAY_OFFSET, -m_trans.GetUp());

	if (rayResult.m_isHit &&
		0.0f < rayResult.m_distance &&
		rayResult.m_distance <= RAY_LENGTH + GROUND_RAY_OFFSET) {

		//押し戻し。
		m_trans.pos +=
			rayResult.m_normal *
			(RAY_LENGTH + GROUND_RAY_OFFSET - rayResult.m_distance);
		m_onGround = true;
	}

	//当たり判定を計算。
	rayResult = arg_meshCollision.lock()->CheckHitRay(
		m_trans.pos,
		m_trans.GetFront());
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_trans.pos +=
			rayResult.m_normal *
			(RAY_LENGTH - rayResult.m_distance);
	}

	//右方向
	rayResult = arg_meshCollision.lock()->CheckHitRay(
		m_trans.pos,
		m_trans.GetRight());
	if (rayResult.m_isHit &&
		0.0f < rayResult.m_distance &&
		rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_trans.pos +=
			rayResult.m_normal *
			(RAY_LENGTH - rayResult.m_distance);
	}

	//左方向
	rayResult = arg_meshCollision.lock()->CheckHitRay(
		m_trans.pos,
		-m_trans.GetRight());
	if (rayResult.m_isHit &&
		0.0f < rayResult.m_distance &&
		rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_trans.pos +=
			rayResult.m_normal *
			(RAY_LENGTH - rayResult.m_distance);
	}
}

void Enemy::Init()
{
}

void Enemy::Update(
	std::weak_ptr<MeshCollision> arg_meshCollision)
{
	if (m_state == State::Warning)
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

	else
	{
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
				0.0f,
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
		}
	}

	if (m_oldPos.x >= 0.0f) {
		if (m_trans.pos != m_oldPos) {
			DirectX::XMVECTOR l_quaternion =
				CalMoveQuaternion(m_trans.pos, m_oldPos);
			m_trans.quaternion = l_quaternion;
		}
	}

	m_oldPos = m_trans.pos;

	Collision(arg_meshCollision);
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
}

DirectX::XMVECTOR Enemy::CalMoveQuaternion(
	KazMath::Vec3<float> arg_pos,
	KazMath::Vec3<float> arg_prevPos)
{
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
