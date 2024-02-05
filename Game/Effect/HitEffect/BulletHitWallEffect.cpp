#include "BulletHitWallEffect.h"

BulletHitWallEffect::BulletHitWallEffect()
{
}

void BulletHitWallEffect::Load(DrawingByRasterize& arg_rasterize)
{
	for (auto& obj : m_particle)
	{
		obj.Load(arg_rasterize);
	}
}

void BulletHitWallEffect::Init(const KazMath::Vec3<float>& arg_pos)
{
	for (auto& obj : m_particle)
	{
		//デバック用に一個しか出さない
		obj.Init(arg_pos, KazMath::Vec3<float>(0.0f,0.0f,1.0f));
		break;
	}
}

void BulletHitWallEffect::Update()
{
	for (auto& obj : m_particle)
	{
		obj.Update();
	}
}

void BulletHitWallEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	for (auto& obj : m_particle)
	{
		obj.Draw(arg_rasterize, arg_blasVector);
	}
}
