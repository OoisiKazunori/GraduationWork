#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Raytracing/BlasVector.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/Effect/HitEffect/HitWallParticle.h"
#include"../Game/Effect/HitEffect/HitCircle.h"

class BulletHitWallEffect
{
public:
	BulletHitWallEffect();

	void Load(DrawingByRasterize& arg_rasterize);
	void Init(const KazMath::Vec3<float>& arg_pos);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector);

private:
	std::array<HitWallParticle,3> m_particle;
	std::array<HitCircle,2> m_hitCircle;

};

