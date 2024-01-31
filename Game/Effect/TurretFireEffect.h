#pragma once
#include"BulletFireParticle.h"
#include"MuzzleFlash.h"
#include<array>

class TurretFireEffect
{
public:
	TurretFireEffect(DrawingByRasterize& arg_rasterize);

	void Init(const KazMath::Vec3<float>* arg_pos,float arg_radian,float arg_shotTimer);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);

private:
	//弾丸の線
	std::array<BulletFireParticle, 50>m_fireEffectArray;
	//マズルフラッシュ
	std::array<MuzzleFlash, 10>m_muzzleFlashArray;

	const KazMath::Vec3<float>*m_emittPos;
	int m_emittTimer;
	float m_radian;

	KazMath::Timer m_shotTimer;
	bool m_isActiveFlag;
};

