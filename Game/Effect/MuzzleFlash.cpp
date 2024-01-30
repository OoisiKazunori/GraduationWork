#include "MuzzleFlash.h"

MuzzleFlash::MuzzleFlash() :m_activeFlag(false)
{
}

void MuzzleFlash::Load(DrawingByRasterize& arg_rasterize)
{
	m_muzzleTex.m_tex.Load(arg_rasterize, "Resource/Test/white1x1.png", false);
}

void MuzzleFlash::Init(const KazMath::Vec3<float>& arg_emittPos)
{
	m_activeFlag = true;
	m_transform.scale = { KazMath::Rand<float>(0.5f,0.1f),KazMath::Rand<float>(0.5f,0.1f),1.0f };
	m_transform.Rotation({ 0.0f,0.0f,1.0f }, KazMath::AngleToRadian(KazMath::Rand<float>(360.0f, 0.0f)));
	m_transform.pos = arg_emittPos;
}

void MuzzleFlash::Finalize()
{
	m_activeFlag = false;
}

void MuzzleFlash::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
}

void MuzzleFlash::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& blas)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_muzzleTex.m_tex.Draw3DBillBoard(arg_rasterize, blas, m_transform, KazMath::Color(255, 0, 0, 255));
}

bool MuzzleFlash::IsActive()
{
	return m_activeFlag;
}
