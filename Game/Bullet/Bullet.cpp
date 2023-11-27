#include "Bullet.h"
#include "../Collision/MeshCollision.h"
#include "../Echo/EchoArray.h"

Bullet::Bullet(DrawingByRasterize& arg_rasterize)
{

	Init();

	m_line.Generate(arg_rasterize);

}

void Bullet::Init() {

	m_disappearTimer = 0;
	m_isActive = false;
	m_isCollision = false;

}

void Bullet::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir) {

	m_disappearTimer = 0;
	m_isActive = true;
	m_isCollision = true;
	m_collisionStartPos = arg_pos;
	m_dir = arg_dir;
	m_collisionEndPos = m_collisionStartPos + m_dir * 100000.0f;
	m_bulletPos = arg_pos + m_dir * 5.0f;

}

void Bullet::Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders) {

	if (!m_isActive) return;

	//まだ当たり判定を行っていなかったら当たり判定を計算。
	if (m_isCollision) {

		for (auto itr = arg_stageColliders.begin(); itr != arg_stageColliders.end(); ++itr) {

			const float RAY_LENGTH = KazMath::Vec3<float>(m_collisionEndPos - m_collisionStartPos).Length();
			MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(m_collisionStartPos, KazMath::Vec3<float>(m_collisionEndPos - m_collisionStartPos).GetNormal());
			if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

				//当たった地点を保存。
				m_collisionEndPos = rayResult.m_position;

			}

		}

		m_isCollision = false;

	}
	//当たり判定が終わっていたら。
	else {

		//弾を動かす。
		m_bulletPos += m_dir * BULLET_SPEED;

		//弾が当たったオブジェクトに到達していたら。
		if (KazMath::Vec3<float>(m_collisionEndPos - m_collisionStartPos).Length() <= KazMath::Vec3<float>(m_bulletPos - m_collisionStartPos).Length()) {

			EchoArray::Instance()->Generate(m_collisionEndPos, 60.0f, KazMath::Vec3<float>(1.0f, 1.0f, 1.0f));

			Init();

		}


	}

	//勝手に消えるまでのタイマーを更新。
	m_disappearTimer = std::clamp(m_disappearTimer + 1, 0, DISAPPEAR_TIMER);
	if (DISAPPEAR_TIMER <= m_disappearTimer) {

		Init();

	}

}

void Bullet::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {

	m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_bulletPos, m_bulletPos + m_dir * BULLET_LENGTH, KazMath::Color(255, 255, 255, 255));

}