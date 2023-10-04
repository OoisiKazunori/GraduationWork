#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//�g�p���郂�f��
	KazMath::Transform3D m_transform;		//���f���̕`��Ɏg�p����g�����X�t�H�[�����
	KazMath::Vec3<float> m_prevPos;

	//�ړ����x
	const float MOVE_SPEED = 0.5f;

	//�p���̃X�e�[�^�X
	enum class PlayerAttitude {
		STAND,
		SQUAT,
		CREEPING,
	}m_playerAttitude;

public:

	Player(DrawingByRasterize& arg_rasterize);

	void Init();

	void Update(KazMath::Transform3D arg_cameraQuaternion);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }

private:

	void Input(KazMath::Transform3D arg_cameraQuaternion);
	void Rotate();

};