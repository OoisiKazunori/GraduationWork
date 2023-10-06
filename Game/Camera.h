#pragma once
#include "../KazLibrary/Math/KazMath.h"

class Camera {

private:

	KazMath::Vec3<float> m_eye;
	KazMath::Vec3<float> m_target;

	//�v���C���[���S�ŃJ�����̈ʒu�����߂邽�߂̎p���B
	KazMath::Transform3D m_cameraPosQuaternion;
	float m_cameraXAngle;		//�㉺�����̃J�����̊p�x�B����Ɖ�����؂�ւ��邽�߂ɐ؂蕪���܂����B

	//�����_���王�_�܂ł̋���
	const float EYE_TARGET_DISTANCE = 50.0f;

	//�J������X�����̉�]�ʂ̏����l
	const float CAMERA_X_ANGLE_INIT_AMOUNT = -0.72f;


public:

	Camera();

	void Init();

	void Update(KazMath::Vec3<float> arg_playerPos);

	KazMath::Transform3D GetCameraPosQaternion();

private:

	void Input();

	//XMVECTOR��Vec3<float>�ɕϊ�
	KazMath::Vec3<float> TransformVec3(DirectX::XMVECTOR arg_input) {
		return KazMath::Vec3<float>(arg_input.m128_f32[0], arg_input.m128_f32[1], arg_input.m128_f32[2]);
	}

	//�J�����̎����x�N�g�������߂�B
	KazMath::Vec3<float> GetCameraVec();

};