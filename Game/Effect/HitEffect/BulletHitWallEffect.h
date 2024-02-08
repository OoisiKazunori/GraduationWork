#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Raytracing/BlasVector.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/Effect/HitEffect/HitWallParticle.h"

class BulletHitWallEffect
{
public:
	BulletHitWallEffect();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_pos);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector);

private:
	std::array<HitWallParticle,6> m_particle;

};

