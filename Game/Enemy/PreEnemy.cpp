#include "PreEnemy.h"
#include "../Echo/EchoArray.h"
#include "../Game/Collision/MeshCollision.h"

PreEnemy::PreEnemy(DrawingByRasterize& arg_rasterize) :
	m_modelRender(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf")
{
}

void PreEnemy::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (m_inEcho)
	{
		m_modelRender.Draw(arg_rasterize, arg_blasVec, m_modelTransform);
	}
}

void PreEnemy::CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{
	//全てのEchoとチェック
	m_inEcho = false;
	for (auto& index : EchoArray::Instance()->GetEcho()) {

		//エコーが生成されていなかったら。
		if (!index.GetIsActive()) continue;
		if (index.GetNowRadius() <= 0.1f) continue;

		//まずは球で当たり判定を行う。
		KazMath::Vec3<float> echoVec = m_modelTransform.pos - index.GetPos();
		float distance = echoVec.Length();
		if (index.GetNowRadius() <= distance) continue;

		//次にレイを飛ばして当たり判定を行う。
		MeshCollision::CheckHitResult result = arg_stageMeshCollision.lock()->CheckHitRay(index.GetPos(), echoVec.GetNormal());

		//当たっていたら
		if (!result.m_isHit || (result.m_isHit && distance <= fabs(result.m_distance))) {

			m_inEcho = true;
			break;

		}

	}

}
