#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Render/BasicDraw.h"

class MuzzleFlash
{
public:
	MuzzleFlash();

	void Init(const KazMath::Vec3<float>& arg_emittPos);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& blas);

	bool IsActive();
private:
	KazMath::Transform3D m_transform;
	BasicDraw::BasicTextureRender m_model;
};

