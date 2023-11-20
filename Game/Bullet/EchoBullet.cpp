#include "EchoBullet.h"
#include "../Collision/MeshCollision.h"
#include "../Echo/EchoArray.h"

EchoBullet::EchoBullet(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Bullet/", "EchoBullet.gltf") {
}

void EchoBullet::Init()
{
	m_isActive = false;
	m_isCollision = false;
	m_echoCount = 0;
	m_echoSpan = 0.0f;
}

void EchoBullet::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir)
{
	m_isActive = true;
	m_isCollision = true;
	m_transform.pos = arg_pos;
	m_dir = arg_dir;
	m_echoCount = ECHO_COUNT;
	m_echoSpan = 0.0f;
	m_disappearTimer = 0;
}

void EchoBullet::Update(std::weak_ptr<MeshCollision> arg_meshCollision)
{

	//当たり判定がまだ終わってなかったら
	if (m_isCollision) {

		//進行方向にレイを飛ばして当たり判定を行う。
		MeshCollision::CheckHitResult rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos, m_dir);
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= BULLET_SPEED) {

			//当たっていたらその位置に弾を設定してエコーを出す状態に遷移させる。
			m_transform.pos = rayResult.m_position;

			//まずは最初にエコーを出す。
			EchoArray::Instance()->Generate(m_transform.pos, 40.0f, KazMath::Vec3<float>(1.0f, 0.0f, 0.0f));
			--m_echoCount;

			m_isCollision = false;

		}
		else {

			//弾を移動させる。
			m_transform.pos += m_dir * BULLET_SPEED;

			//一定時間弾がどれにも当たらなかったら処理を飛ばす。
			++m_disappearTimer;
			if (DESAPPEAR_TIMER < m_disappearTimer) {

				Init();

			}

		}

	}
	else {

		//一定間隔でエコーを出す。エコー数が-1になったら初期化する。
		++m_echoSpan;
		if (ECHO_SPAN <= m_echoSpan) {

			EchoArray::Instance()->Generate(m_transform.pos, 40.0f, KazMath::Vec3<float>(1.0f, 0.0f, 0.0f));
			--m_echoCount;
			m_echoSpan = 0.0f;

			if (m_echoCount <= 0) {

				Init();

			}

		}


	}

}

void EchoBullet::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

}
