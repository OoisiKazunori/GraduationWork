//#pragma once
//#include "../KazLibrary/Math/KazMath.h"
//#include "../KazLibrary/Render/BasicDraw.h"
//
//class ThrowableObject {
//
//private:
//
//	BasicDraw::BasicModelRender m_model;
//	KazMath::Transform3D m_transform;
//	KazMath::Vec3<float> m_throwVector;
//	float m_gravity;
//	float m_vel;
//	bool m_isActive;
//
//	//重力加算量
//	const float ADD_GRAVITY = 0.1f;
//	//移動速度を下げる量
//	const float SUB_VEL = 0.2f;
//
//public:
//
//	ThrowableObject(DrawingByRasterize& arg_rasterize);
//	void Init();
//	void Update(std::weak_ptr<MeshCollision> arg_meshCollision);
//	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
//
//	bool GetIsActive() { return m_isActive; }
//
//};