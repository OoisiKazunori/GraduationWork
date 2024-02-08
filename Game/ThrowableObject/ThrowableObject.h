#pragma once
#include "../KazLibrary/Math/KazMath.h"
#include "../KazLibrary/Render/BasicDraw.h"
#include "../KazLibrary/Sound/SoundManager.h"

class MeshCollision;

class ThrowableObject {

private:

	BasicDraw::BasicModelRender m_model;
	BasicDraw::BasicModelRender m_predictedModel;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_throwVector;
	float m_gravity;
	float m_vel;
	bool m_isActive;
	bool m_isPredictedObject;

	SoundData m_objectSE;
	const float DEFAULT_SE_VOLUME = 0.1f;
	const float SOUND_RANGE = 400.0f;	//音の聞こえる範囲。

	//仮置き変数
	int m_exitTimer;

	//重力加算量
	const float ADD_GRAVITY = 0.05f;
	//移動速度を下げる量
	const float SUB_VEL = 0.0f;

public:

	ThrowableObject(DrawingByRasterize& arg_rasterize);
	void Init();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="arg_playerTransform">生成地点</param>
	/// <param name="arg_throwVec">投げる方向</param>
	/// <param name="arg_isPredictedObject">このオブジェクトが予測線用オブジェクトかどうか</param>
	void Generate(KazMath::Transform3D arg_playerTransform, KazMath::Vec3<float> arg_throwVec, float arg_vel, bool arg_isPredictedObject);

	void Update(std::list<std::shared_ptr<MeshCollision>> f_stageColliders, KazMath::Vec3<float> arg_playerPos);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool GetIsActive() { return m_isActive; }

};