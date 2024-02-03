#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"SmokeParticle.h"

class SmokeEmitter
{
public:
	SmokeEmitter();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_range);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);

private:
	std::array<SmokeParticle, 50>m_smokeParticleArray;
	std::array<int, 50>m_smokeEmittTimeArray;
	KazMath::Vec3<float>m_emittPos;
};

