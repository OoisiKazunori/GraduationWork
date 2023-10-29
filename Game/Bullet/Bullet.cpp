#include "Bullet.h"
#include "../Collision/MeshCollision.h"
#include "../Echo/EchoArray.h"

Bullet::Bullet(DrawingByRasterize& arg_rasterize) :
	m_line(arg_rasterize) {

	Init();

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
	m_startPos = arg_pos;
	m_dir = arg_dir;
	m_endPos = m_startPos + m_dir * 100000.0f;

}

void Bullet::Update(std::weak_ptr<MeshCollision> arg_meshCollision) {

	//まだ当たり判定を行っていなかったら当たり判定を計算。
	if (m_isCollision) {

		const float RAY_LENGTH = KazMath::Vec3<float>(m_endPos - m_startPos).Length();
		MeshCollision::CheckHitResult rayResult = arg_meshCollision.lock()->CheckHitRay(m_startPos, KazMath::Vec3<float>(m_endPos - m_startPos).GetNormal());
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

			//当たった地点を保存。
			m_endPos = rayResult.m_position;

			EchoArray::Instance()->Generate(m_endPos, 30.0f, KazMath::Vec3<float>(1.0f, 0.0f, 0.0f));

		}

		m_isCollision = false;

	}

	//消えるまでのタイマーを更新。
	m_disappearTimer = std::clamp(m_disappearTimer + 1, 0, DISAPPEAR_TIMER);
	if (DISAPPEAR_TIMER <= m_disappearTimer) {

		Init();

	}

}

void Bullet::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {

	m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_startPos, m_endPos, KazMath::Color(255, 255, 255, 255));

}