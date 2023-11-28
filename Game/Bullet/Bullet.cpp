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

	//�܂������蔻����s���Ă��Ȃ������瓖���蔻����v�Z�B
	if (m_isCollision) {

		for (auto itr = arg_stageColliders.begin(); itr != arg_stageColliders.end(); ++itr) {

			const float RAY_LENGTH = KazMath::Vec3<float>(m_collisionEndPos - m_collisionStartPos).Length();
			MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(m_collisionStartPos, KazMath::Vec3<float>(m_collisionEndPos - m_collisionStartPos).GetNormal());
			if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

				//���������n�_��ۑ��B
				m_collisionEndPos = rayResult.m_position;

			}

		}

		m_isCollision = false;

	}
	//�����蔻�肪�I����Ă�����B
	else {

		//�e�𓮂����B
		m_bulletPos += m_dir * BULLET_SPEED;

		//�e�����������I�u�W�F�N�g�ɓ��B���Ă�����B
		if (KazMath::Vec3<float>(m_collisionEndPos - m_collisionStartPos).Length() <= KazMath::Vec3<float>(m_bulletPos - m_collisionStartPos).Length()) {

			EchoArray::Instance()->Generate(m_collisionEndPos, 60.0f, KazMath::Vec3<float>(1.0f, 1.0f, 1.0f));

			Init();

		}


	}

	//����ɏ�����܂ł̃^�C�}�[���X�V�B
	m_disappearTimer = std::clamp(m_disappearTimer + 1, 0, DISAPPEAR_TIMER);
	if (DISAPPEAR_TIMER <= m_disappearTimer) {

		Init();

	}

}

void Bullet::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {

	m_line.m_render.Draw(arg_rasterize, arg_blasVec, m_bulletPos, m_bulletPos + m_dir * BULLET_LENGTH, KazMath::Color(255, 255, 255, 255));

}