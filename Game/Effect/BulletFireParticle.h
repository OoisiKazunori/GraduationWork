#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../KazLibrary/Math/Timer.h"

class BulletFireParticle
{
public:
	BulletFireParticle();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_emittPos,float arg_radian);
	void Finalize();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize,Raytracing::BlasVector& arg_blas);

	bool IsActive();
private:
	KazMath::Vec3<float>m_vel;
	KazMath::Transform3D m_transform;
	BasicDraw::BasicModelRender m_model;
	bool m_activeFlag;
	float m_radian;
	KazMath::Timer m_timer;
};

