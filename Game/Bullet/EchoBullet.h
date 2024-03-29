#pragma once
#include <Render/BasicDraw.h>
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Sound/SoundManager.h"

class MeshCollision;

class EchoBullet {

private:

	KazMath::Vec3<float> m_dir;			//弾を射出する方向。
	bool m_isActive;					//有効化されているかのフラグ
	bool m_isCollision;					//当たり判定を行うことができるか？のフラグ。軽量化で一回しか当たり判定を行わないために使用する。
	BasicDraw::BasicModelRender m_model;
	KazMath::Transform3D m_transform;		//モデルの描画に使用するトランスフォーム情報
	const float BULLET_SPEED = 20.0f;

	int m_echoCount;
	const int ECHO_COUNT = 3;
	float m_echoSpan;
	const float ECHO_SPAN = 60.0f;
	int m_disappearTimer;
	const int DESAPPEAR_TIMER = 60;

	SoundData m_echoSE;

public:

	EchoBullet(DrawingByRasterize& arg_rasterize);
	void Init();
	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);
	void Update(std::list<std::shared_ptr<MeshCollision>> arg_stageColliders);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; };

	bool CheckMeshCollision(std::weak_ptr<MeshCollision> arg_meshCollision);

};