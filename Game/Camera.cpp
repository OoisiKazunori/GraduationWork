#include "Camera.h"
#include "Input/KeyBoradInputManager.h"
#include "Input/ControllerInputManager.h"
#include "../KazLibrary/Camera/CameraMgr.h"
#include "../Game/Collision/MeshCollision.h"
#include <algorithm>

Camera::Camera()
{

	m_cameraXAngle = CAMERA_X_ANGLE_INIT_AMOUNT;

}

void Camera::Init()
{

	m_cameraXAngle = CAMERA_X_ANGLE_INIT_AMOUNT;

}

void Camera::Update(KazMath::Transform3D arg_playerTransform, std::weak_ptr<MeshCollision> arg_stageMeshCollision, bool arg_isADS)
{

	//���͂����B
	Input();

	//�e���\���Ă����Ԃ�������
	if (arg_isADS) {

		const float TARGET_DISTNACE = 10.0f;

		//�����_������
		m_target += ((arg_playerTransform.pos + GetCameraVec() * TARGET_DISTNACE) - m_target) / 3.0f;

		//���_�̈ʒu������B
		m_eye += ((arg_playerTransform.pos + GetCameraVec() * (TARGET_DISTNACE / 2.0f)) - m_eye) / 3.0f;

	}
	else {

		//�����_������
		m_target += (arg_playerTransform.pos - m_target) / 3.0f;

		//���_�̈ʒu������B
		m_eye += (m_target - GetCameraVec() * EYE_TARGET_DISTANCE - m_eye) / 3.0f;

	}

	//�v���C���[����J���������Ƀ��C���΂��ē����蔻����s���B
	const float RAY_LENGTH = KazMath::Vec3<float>(m_eye - m_target).Length();
	MeshCollision::CheckHitResult rayResult = arg_stageMeshCollision.lock()->CheckHitRay(m_target, KazMath::Vec3<float>(m_eye - m_target).GetNormal());
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//�����߂��B
		m_eye = rayResult.m_position + rayResult.m_normal;

	}


	CameraMgr::Instance()->Camera(m_eye, m_target, { 0,1,0 });

}

KazMath::Transform3D Camera::GetCameraPosQaternion()
{
	KazMath::Transform3D cameraTransform = m_cameraPosQuaternion;
	cameraTransform.Rotation(cameraTransform.GetRight(), m_cameraXAngle);
	return cameraTransform;
}

void Camera::Input()
{

	//���E�̃J��������
	m_cameraPosQuaternion.Rotation({ 0,1,0 }, KeyBoradInputManager::Instance()->GetMouseVel().x * 0.001f);

	//�㉺�̃J��������
	m_cameraXAngle = std::clamp(m_cameraXAngle - KeyBoradInputManager::Instance()->GetMouseVel().y * 0.001f, -1.0f, 1.0f);

}

KazMath::Vec3<float> Camera::GetCameraVec()
{
	return -TransformVec3(DirectX::XMVector3Transform({ 0,0,1 }, DirectX::XMMatrixRotationQuaternion(GetCameraPosQaternion().quaternion)));
}
