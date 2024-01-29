#include "TurretFireEffect.h"

TurretFireEffect::TurretFireEffect()
{
}

void TurretFireEffect::Init()
{
}

void TurretFireEffect::Update(const KazMath::Vec3<float>& arg_pos)
{
	for (auto& obj : m_fireEffectArray)
	{
		if (!obj.IsActive())
		{
			obj.Init(arg_pos);
		}
		obj.Update();
	}

	//for (auto& obj : m_muzzleFlashArray)
	//{
	//	if (obj.IsActive())
	//	{
	//		obj.Init(arg_pos);
	//	}
	//	obj.Update();
	//}
}

void TurretFireEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	for (auto& obj : m_fireEffectArray)
	{
		obj.Draw(arg_rasterize, arg_blas);
	}
}
