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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="arg_pos">発生地点</param>
	/// <param name="arg_range">発生範囲</param>
	/// <param name="arg_smokeTimer">発生時間</param>
	void Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_range,int arg_smokeTimer);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);

private:
	std::array<SmokeParticle, 50>m_smokeParticleArray;
	std::array<int, 50>m_smokeEmittTimeArray;
	KazMath::Vec3<float>m_emittPos;
	KazMath::Vec3<float>m_range;
	int m_emittTimer, m_emittMaxTimer;
};

