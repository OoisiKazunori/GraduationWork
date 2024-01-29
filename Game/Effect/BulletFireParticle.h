#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Render/BasicDraw.h"

class BulletFireParticle
{
public:
	BulletFireParticle();

	void Init(const KazMath::Vec3<float>& arg_emittPos);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize,Raytracing::BlasVector& arg_blas);

	bool IsActive();
private:
	KazMath::Transform3D m_transform;
	BasicDraw::BasicModelRender m_model;
	bool m_activeFlag;

};

