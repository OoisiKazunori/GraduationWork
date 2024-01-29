#pragma once
#include"BulletFireParticle.h"
#include"MuzzleFlash.h"
#include<array>

class TurretFireEffect
{
public:
	TurretFireEffect();

	void Init();
	void Update(const KazMath::Vec3<float>& arg_pos);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);

private:
	//�e�ۂ̐�
	std::array<BulletFireParticle, 10>m_fireEffectArray;
	//�}�Y���t���b�V��
	std::array<MuzzleFlash, 10>m_muzzleFlashArray;
};

