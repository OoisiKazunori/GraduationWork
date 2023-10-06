#pragma once
#include "../KazLibrary/Math/KazMath.h"

class Camera {

private:

	KazMath::Vec3<float> m_eye;
	KazMath::Vec3<float> m_target;

	//プレイヤー中心でカメラの位置を決めるための姿勢。
	KazMath::Transform3D m_cameraPosQuaternion;
	float m_cameraXAngle;		//上下方向のカメラの角度。上限と下限を切り替えるために切り分けました。

	//注視点から視点までの距離
	const float EYE_TARGET_DISTANCE = 50.0f;

	//カメラのX方向の回転量の初期値
	const float CAMERA_X_ANGLE_INIT_AMOUNT = -0.72f;


public:

	Camera();

	void Init();

	void Update(KazMath::Vec3<float> arg_playerPos);

	KazMath::Transform3D GetCameraPosQaternion();

private:

	void Input();

	//XMVECTORをVec3<float>に変換
	KazMath::Vec3<float> TransformVec3(DirectX::XMVECTOR arg_input) {
		return KazMath::Vec3<float>(arg_input.m128_f32[0], arg_input.m128_f32[1], arg_input.m128_f32[2]);
	}

	//カメラの視線ベクトルを求める。
	KazMath::Vec3<float> GetCameraVec();

};