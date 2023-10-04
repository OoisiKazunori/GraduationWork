#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//使用するモデル
	KazMath::Transform3D m_transform;		//モデルの描画に使用するトランスフォーム情報
	KazMath::Vec3<float> m_prevPos;

	//移動速度
	const float MOVE_SPEED = 0.5f;

	//姿勢のステータス
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