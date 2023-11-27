#include "ThrowableObject.h"
#include "../Game/Collision/MeshCollision.h"
#include "../Echo/EchoArray.h"

ThrowableObject::ThrowableObject(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Bullet/", "EchoBullet.gltf")
{

	Init();

}

void ThrowableObject::Init()
{

	m_isActive = false;

}

void ThrowableObject::Generate(KazMath::Transform3D arg_playerTransform, KazMath::Vec3<float> arg_throwVec, float arg_vel, bool arg_isPredictedObject)
{

	m_transform = arg_playerTransform;

	//プレイヤーのトランスフォーム基準で左下にずらす。
	m_transform.pos -= m_transform.GetUp() * 2.0f;

	m_throwVector = arg_throwVec;
	m_gravity = 0.0f;
	m_vel = arg_vel;
	m_isActive = true;
	m_isPredictedObject = arg_isPredictedObject;

	//予測線用のオブジェクトかどうかでサイズを変える。
	if (arg_isPredictedObject) {

		m_transform.scale = KazMath::Vec3<float>(0.05f, 0.05f, 0.05f);

	}
	else {

		m_transform.scale = KazMath::Vec3<float>(1.0f, 1.0f, 1.0f);

	}

	m_exitTimer = 0;

}

void ThrowableObject::Update(std::list<std::shared_ptr<MeshCollision>> f_stageColliders)
{

	//前フレームの座標
	KazMath::Vec3<float> prevPos = m_transform.pos;

	//velに値分動かす。
	m_transform.pos += m_throwVector * m_vel;

	//重力をかける。
	m_gravity += ADD_GRAVITY;
	m_transform.pos += KazMath::Vec3<float>(0.0f, -1.0f, 0.0f) * m_gravity;

	//予測専用のオブジェクトじゃなかったら当たり判定を行う。
	if (!m_isPredictedObject) {

		for (auto itr = f_stageColliders.begin(); itr != f_stageColliders.end(); ++itr) {

			MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(m_transform.pos, KazMath::Vec3<float>(m_transform.pos - prevPos).GetNormal());
			if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= KazMath::Vec3<float>(m_transform.pos - prevPos).Length()) {

				EchoArray::Instance()->Generate(m_transform.pos, 40.0f, KazMath::Vec3<float>(1.0f, 1.0f, 1.0f));
				Init();

			}

		}

	}

	//仮置きで消えるまでのタイマーを更新。
	++m_exitTimer;
	if (90 <= m_exitTimer) {
		Init();
	}

}

void ThrowableObject::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

}
