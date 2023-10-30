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

	//�����蔻�肪�܂��I����ĂȂ�������
	if (m_isCollision) {

		//�i�s�����Ƀ��C���΂��ē����蔻����s���B
		MeshCollision::CheckHitResult rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos, m_dir);
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= BULLET_SPEED) {

			//�������Ă����炻�̈ʒu�ɒe��ݒ肵�ăG�R�[���o����ԂɑJ�ڂ�����B
			m_transform.pos = rayResult.m_position;

			//�܂��͍ŏ��ɃG�R�[���o���B
			EchoArray::Instance()->Generate(m_transform.pos, 40.0f, KazMath::Vec3<float>(1.0f, 0.0f, 0.0f));
			--m_echoCount;

			m_isCollision = false;

		}
		else {

			//�e���ړ�������B
			m_transform.pos += m_dir * BULLET_SPEED;

			//��莞�Ԓe���ǂ�ɂ�������Ȃ������珈�����΂��B
			++m_disappearTimer;
			if (DESAPPEAR_TIMER < m_disappearTimer) {

				Init();

			}

		}

	}
	else {

		//���Ԋu�ŃG�R�[���o���B�G�R�[����-1�ɂȂ����珉��������B
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
