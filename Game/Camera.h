#pragma once
#include "../KazLibrary/Math/KazMath.h"

class MeshCollision;

class Camera {

private:

	KazMath::Vec3<float> m_eye;
	KazMath::Vec3<float> m_target;

	//プレイヤー中心でカメラの位置を決めるための姿勢。
	KazMath::Transform3D m_shotQuaternion;
	float m_cameraXAngle;		//上下方向のカメラの角度。上限と下限を切り替えるために切り分けました。
	bool m_isOldADS;
	float m_ADSEasingTimer;
	const float ADS_EASING_TIMER = 10.0f;

	//注視点から視点までの距離
	const float EYE_TARGET_DISTANCE = 20.0f;

	//カメラのX方向の回転量の初期値
	const float CAMERA_X_ANGLE_INIT_AMOUNT = -0.72f;


public:

	Camera();

	void Init();

	void Update(KazMath::Transform3D arg_playerTransform, std::weak_ptr<MeshCollision> arg_stageMeshCollision, bool arg_isADS);

	KazMath::Transform3D GetShotQuaternion();
	KazMath::Vec3<float> GetEyePos() { return m_eye; }

	static float CameraSensitivity;
	static bool isFlip;
private:

	void Input();

	//XMVECTORをVec3<float>に変換
	KazMath::Vec3<float> TransformVec3(DirectX::XMVECTOR arg_input) {
		return KazMath::Vec3<float>(arg_input.m128_f32[0], arg_input.m128_f32[1], arg_input.m128_f32[2]);
	}

	//カメラの視線ベクトルを求める。
	KazMath::Vec3<float> GetCameraVec();

};