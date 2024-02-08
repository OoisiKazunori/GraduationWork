#include "ThrowableObject.h"
#include "../Game/Collision/MeshCollision.h"
#include "../Echo/EchoArray.h"
#include "../KazLibrary/Easing/easing.h"

ThrowableObject::ThrowableObject(DrawingByRasterize& arg_rasterize) :
	m_predictedModel(arg_rasterize, "Resource/Bullet/", "EchoBullet.gltf"),
	m_model(arg_rasterize, "Resource/Weapon/Rock/", "Rock.gltf")
{


	m_objectSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Object.wav");
	m_objectSE.volume = DEFAULT_SE_VOLUME;

	Init();

}

void ThrowableObject::Init()
{

	m_isActive = false;

}

void ThrowableObject::Generate(KazMath::Transform3D arg_playerTransform, KazMath::Vec3<float> arg_throwVec, float arg_vel, bool arg_isPredictedObject)
{

	m_transform = arg_playerTransform;

	//�v���C���[�̃g�����X�t�H�[����ō����ɂ��炷�B
	m_transform.pos -= m_transform.GetUp() * 2.0f;

	m_throwVector = arg_throwVec;
	m_gravity = 0.0f;
	m_vel = arg_vel;
	m_isActive = true;
	m_isPredictedObject = arg_isPredictedObject;

	//�\�����p�̃I�u�W�F�N�g���ǂ����ŃT�C�Y��ς���B
	if (arg_isPredictedObject) {

		m_transform.scale = KazMath::Vec3<float>(0.05f, 0.05f, 0.05f);

	}
	else {

		m_transform.scale = KazMath::Vec3<float>(2.0f, 2.0f, 2.0f);

	}

	m_exitTimer = 0;

}

void ThrowableObject::Update(std::list<std::shared_ptr<MeshCollision>> f_stageColliders, KazMath::Vec3<float> arg_playerPos)
{

	//�O�t���[���̍��W
	KazMath::Vec3<float> prevPos = m_transform.pos;

	//vel�ɒl���������B
	m_transform.pos += m_throwVector * m_vel;

	//�d�͂�������B
	m_gravity += ADD_GRAVITY;
	m_transform.pos += KazMath::Vec3<float>(0.0f, -1.0f, 0.0f) * m_gravity;

	//�\����p�̃I�u�W�F�N�g����Ȃ������瓖���蔻����s���B
	if (!m_isPredictedObject) {

		for (auto itr = f_stageColliders.begin(); itr != f_stageColliders.end(); ++itr) {

			MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(m_transform.pos, KazMath::Vec3<float>(m_transform.pos - prevPos).GetNormal());
			if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= KazMath::Vec3<float>(m_transform.pos - prevPos).Length()) {

				EchoArray::Instance()->Generate(m_transform.pos, 15.0f, Echo::COLOR::WHITE);
				Init();


				//�����ɂ���ĉ�������������B
				float distance = KazMath::Vec3<float>(arg_playerPos - m_transform.pos).Length();
				float range = 1.0f - std::clamp(distance / SOUND_RANGE, 0.0f, 1.0f);
				range = EasingMaker(In, Quad, range);

				m_objectSE.volume = DEFAULT_SE_VOLUME * range;
				SoundManager::Instance()->SoundPlayerWave(m_objectSE, 0);

			}

		}

	}

	//���u���ŏ�����܂ł̃^�C�}�[���X�V�B
	++m_exitTimer;
	if (90 <= m_exitTimer) {
		Init();
	}

}

void ThrowableObject::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (m_isPredictedObject) {
		m_predictedModel.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
	}
	else {
		m_model.m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
	}

}
