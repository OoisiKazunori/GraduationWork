#include "Camera.h"
#include "Input/KeyBoradInputManager.h"
#include "Input/ControllerInputManager.h"
#include "../KazLibrary/Camera/CameraMgr.h"
#include "../Game/Collision/MeshCollision.h"
#include "../KazLibrary/Buffer/GBufferMgr.h"
#include <algorithm>

float Camera::CameraSensitivity = 1.0f;
bool  Camera::isFlip = false;

Camera::Camera()
{

	m_cameraXAngle = CAMERA_X_ANGLE_INIT_AMOUNT;

}

void Camera::Init()
{

	m_cameraXAngle = CAMERA_X_ANGLE_INIT_AMOUNT;
	m_isOldADS = false;
	m_ADSEasingTimer = 0.0f;
	m_titleCameraMove = 0.0f;

}

void Camera::Update(KazMath::Transform3D arg_playerTransform, std::weak_ptr<MeshCollision> arg_stageMeshCollision, bool arg_isADS, bool arg_isTitle)
{

	//入力を取る。
	Input(arg_isTitle);

	//ADSした瞬間 or やめた瞬間だったら
	if ((arg_isADS && !m_isOldADS) || (!arg_isADS && m_isOldADS)) {

		m_ADSEasingTimer = 0.0f;

	}

	//銃を構えている状態だったら
	if (arg_isADS) {

		const float TARGET_DISTNACE = 10.0f;

		//注視点を決定
		m_target = arg_playerTransform.pos + GetCameraVec() * TARGET_DISTNACE;

		//視点の位置を決定。
		KazMath::Vec3<float> cameraPos = arg_playerTransform.pos - GetCameraVec() * (TARGET_DISTNACE / 2.0f);
		cameraPos -= m_shotQuaternion.GetRight() * (TARGET_DISTNACE / 2.0f);
		cameraPos -= m_shotQuaternion.GetUp() * (TARGET_DISTNACE / 10.0f);
		m_eye += (cameraPos - m_eye) / 3.0f;

	}
	else {

		//注視点を決定
		m_target += (arg_playerTransform.pos - m_target) * 0.8f;

		//視点の位置を決定。
		m_eye += (m_target - GetCameraVec() * EYE_TARGET_DISTANCE - m_eye) / 10.0f;

		//プレイヤーからカメラ方向にレイを飛ばして当たり判定を行う。
		const float RAY_LENGTH = KazMath::Vec3<float>(m_eye - m_target).Length();
		MeshCollision::CheckHitResult rayResult = arg_stageMeshCollision.lock()->CheckHitRay(m_target, KazMath::Vec3<float>(m_eye - m_target).GetNormal());
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

			//押し戻し。
			m_eye = rayResult.m_position + rayResult.m_normal;

		}

	}

	//FPS視点にする。
	m_eye = arg_playerTransform.pos;
	m_target = m_eye + arg_playerTransform.GetFront();


	m_isOldADS = arg_isADS;

	CameraMgr::Instance()->Camera(m_eye, m_target, { 0,1,0 });

	GBufferMgr::Instance()->SetCameraPos(m_eye.ConvertXMFLOAT3(), CameraMgr::Instance()->GetViewMatrix(0), CameraMgr::Instance()->GetPerspectiveMatProjection(0));

}


KazMath::Transform3D Camera::GetShotQuaternion()
{
	KazMath::Transform3D cameraTransform = m_shotQuaternion;
	cameraTransform.Rotation(cameraTransform.GetRight(), m_cameraXAngle);
	cameraTransform.Rotation({ 0,1,0 }, DirectX::XM_PI);

	////ADSしていると撃つ方向とカメラの中心がずれるので、それを補正する。
	//if (m_isOldADS) {

	//	cameraTransform.quaternion = DirectX::XMQuaternionMultiply(cameraTransform.quaternion, DirectX::XMQuaternionRotationAxis({ 0,1,0 }, -DirectX::XMConvertToRadians(15.0f)));

	//}

	return cameraTransform;
}
void Camera::Input(bool arg_isTitle)
{

	//タイトル画面だったら
	if (arg_isTitle) {

		float inputMove = KeyBoradInputManager::Instance()->GetMouseVel().x * (0.001f * CameraSensitivity);

		//回転量を制限する。
		m_titleCameraMove += inputMove;
		m_titleCameraMove = std::clamp(m_titleCameraMove, -TITLE_CAMERA_MOVE, TITLE_CAMERA_MOVE);

		m_shotQuaternion.quaternion = DirectX::XMQuaternionIdentity();
		m_shotQuaternion.Rotation({ 0,1,0 }, m_titleCameraMove);

	}
	else {


		//左右のカメラ操作
		m_shotQuaternion.Rotation({ 0,1,0 }, KeyBoradInputManager::Instance()->GetMouseVel().x * (0.001f * CameraSensitivity));

	}


	//上下のカメラ操作
	if (!isFlip)
	{
		m_cameraXAngle = std::clamp(m_cameraXAngle + KeyBoradInputManager::Instance()->GetMouseVel().y * (0.001f * CameraSensitivity), -1.0f, 1.0f);
	}
	else
	{
		m_cameraXAngle = std::clamp(m_cameraXAngle - KeyBoradInputManager::Instance()->GetMouseVel().y * (0.001f * CameraSensitivity), -1.0f, 1.0f);
	}
}

KazMath::Vec3<float> Camera::GetCameraVec()
{
	return -TransformVec3(DirectX::XMVector3Transform({ 0,0,1 }, DirectX::XMMatrixRotationQuaternion(GetShotQuaternion().quaternion)));
}
