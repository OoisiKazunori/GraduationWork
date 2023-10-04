#include "Camera.h"
#include "Input/KeyBoradInputManager.h"
#include "Input/ControllerInputManager.h"
#include "../KazLibrary/Camera/CameraMgr.h"
#include <algorithm>

Camera::Camera()
{
}

void Camera::Init()
{
}

void Camera::Update(KazMath::Vec3<float> arg_playerPos)
{

	//入力を取る。
	Input();

	//注視点を決定
	m_target = arg_playerPos;

	//視点の位置を決定。
	m_eye = m_target - GetCameraVec() * EYE_TARGET_DISTANCE;


	CameraMgr::Instance()->Camera(m_eye, m_target, { 0,1,0 });

}

void Camera::Input()
{

	//左右のカメラ操作
	m_cameraPosQuaternion.Rotation({ 0,1,0 }, KeyBoradInputManager::Instance()->GetMouseVel().x * 0.005f);

	//上下のカメラ操作
	m_cameraXAngle = std::clamp(m_cameraXAngle - KeyBoradInputManager::Instance()->GetMouseVel().y * 0.005f, -1.0f, 1.0f);

}

KazMath::Vec3<float> Camera::GetCameraVec()
{

	KazMath::Transform3D cameraTransform = m_cameraPosQuaternion;
	cameraTransform.Rotation({ 1,0,0 }, m_cameraXAngle);

	return -TransformVec3(DirectX::XMVector3Transform({ 0,0,1 }, DirectX::XMMatrixRotationQuaternion(cameraTransform.quaternion)));
}
