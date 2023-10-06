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

void Camera::Update(KazMath::Vec3<float> arg_playerPos, std::weak_ptr<MeshCollision> arg_stageMeshCollision)
{

	//���͂����B
	Input();

	//�����_������
	m_target = arg_playerPos;

	//���_�̈ʒu������B
	m_eye = m_target - GetCameraVec() * EYE_TARGET_DISTANCE;

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
