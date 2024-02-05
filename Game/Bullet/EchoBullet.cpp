#include "EchoBullet.h"
#include "../Collision/MeshCollision.h"
#include "../Echo/EchoArray.h"
#include "../Effect/StopMgr.h"

EchoBullet::EchoBullet(DrawingByRasterize& arg_rasterize) :
	m_model(arg_rasterize, "Resource/Weapon/EchoBullet/", "Bullet.gltf") {

	m_echoSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/EchoBullet.wav");
	m_echoSE.volume = 0.05f;

	m_transform.scale = KazMath::Vec3<float>(10.0f, 10.0f, 10.0f);

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

void EchoBullet::Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders)
{

	if (!m_isActive) return;

	//�����蔻�肪�܂��I����ĂȂ�������
	if (m_isCollision) {

		//�e�𓮂����B
		m_transform.pos += m_dir * BULLET_SPEED * StopMgr::Instance()->GetGameSpeed();

		bool isHit = false;
		for (auto itr = arg_stageColliders.begin(); itr != arg_stageColliders.end(); ++itr) {

			//�i�s�����Ƀ��C���΂��ē����蔻����s���B
			MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(m_transform.pos, m_dir);
			if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= BULLET_SPEED) {

				//�������Ă����炻�̈ʒu�ɒe��ݒ肵�ăG�R�[���o����ԂɑJ�ڂ�����B
				m_transform.pos = rayResult.m_position;

				//�܂��͍ŏ��ɃG�R�[���o���B
				EchoArray::Instance()->Generate(m_transform.pos, 40.0f, Echo::COLOR::WHITE);
				--m_echoCount;

				SoundManager::Instance()->SoundPlayerWave(m_echoSE, 0);

				m_isCollision = false;
				isHit = true;



				Init();

			}

		}

	}
	else {

		//���Ԋu�ŃG�R�[���o���B�G�R�[����-1�ɂȂ����珉��������B
		m_echoSpan += 1.0f * StopMgr::Instance()->GetGameSpeed();
		if (ECHO_SPAN <= m_echoSpan) {

			EchoArray::Instance()->Generate(m_transform.pos, 40.0f, Echo::COLOR::WHITE);
			--m_echoCount;
			m_echoSpan = 0.0f;

			SoundManager::Instance()->SoundPlayerWave(m_echoSE, 0);

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

bool EchoBullet::CheckMeshCollision(std::weak_ptr<MeshCollision> arg_meshCollision)
{

	MeshCollision::CheckHitResult rayResult = arg_meshCollision.lock()->CheckHitRay(m_transform.pos, m_dir);
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= BULLET_SPEED) {

		EchoArray::Instance()->Generate(m_transform.pos, 40.0f, Echo::COLOR::WHITE);
		Init();
		return true;

	}

	return false;

}
