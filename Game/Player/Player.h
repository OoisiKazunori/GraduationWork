#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class MeshCollision;

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//使用するモデル
	KazMath::Transform3D m_transform;		//モデルの描画に使用するトランスフォーム情報
	KazMath::Vec3<float> m_prevPos;

	bool m_onGround;
	bool m_isADS;		//銃を構えている状態か？

	float m_gravity;
	const float GRAVITY = 0.05f;

	//移動速度
	const float MOVE_SPEED_STAND = 0.5f;
	const float MOVE_SPEED_SQUAT = 0.3f;
	const float MOVE_SPEED_CREEPING = 0.15f;

	//姿勢のステータス
	enum class PlayerAttitude {
		STAND,
		SQUAT,
		CREEPING,
	}m_playerAttitude;

public:

	Player(DrawingByRasterize& arg_rasterize);

	void Init();

	void Update(KazMath::Transform3D arg_cameraQuaternion, std::weak_ptr<MeshCollision> arg_stageMeshCollision);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }
	bool GetIsADS() { return m_isADS; }

private:

	void Input(KazMath::Transform3D arg_cameraQuaternion);
	void Rotate();
	void Collision(std::weak_ptr<MeshCollision> arg_meshCollision);
	float GetMoveSpeed();

};