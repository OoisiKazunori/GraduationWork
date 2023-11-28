#include "PreEnemy.h"
#include "../Echo/EchoArray.h"
#include "../Game/Collision/MeshCollision.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Debug/DebugKey.h"

PreEnemy::PreEnemy(DrawingByRasterize& arg_rasterize) :
	m_modelRender(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf"),
	m_reactionRender(arg_rasterize)
{
}

void PreEnemy::Update()
{
	if (DebugKey::Instance()->DebugKeyTrigger(DIK_7, "EnemyReaction", "DIK_7"))
	{
		m_reactionRender.Init(0, KazMath::Vec3<float>(0.0f, 1.0f, 0.0f));
	}
	m_reactionRender.Update(m_modelTransform.pos);
}

void PreEnemy::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (m_inEcho)
	{
		//m_modelRender.Draw(arg_rasterize, arg_blasVec, m_modelTransform);
	}

	m_reactionRender.Draw(arg_rasterize, arg_blasVec);
}

void PreEnemy::CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{
	//�S�Ă�Echo�ƃ`�F�b�N
	m_inEcho = false;
	for (auto& index : EchoArray::Instance()->GetEcho()) {

		//�G�R�[����������Ă��Ȃ�������B
		if (!index.GetIsActive()) continue;
		if (index.GetNowRadius() <= 0.1f) continue;

		//�܂��͋��œ����蔻����s���B
		KazMath::Vec3<float> echoVec = m_modelTransform.pos - index.GetPos();
		float distance = echoVec.Length();
		if (index.GetNowRadius() <= distance) continue;

		//���Ƀ��C���΂��ē����蔻����s���B
		MeshCollision::CheckHitResult result = arg_stageMeshCollision.lock()->CheckHitRay(index.GetPos(), echoVec.GetNormal());

		//�������Ă�����
		if (!result.m_isHit || (result.m_isHit && distance <= fabs(result.m_distance))) {

			m_inEcho = true;
			break;

		}

	}

}
