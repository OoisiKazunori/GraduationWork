#include "SmokeEmitter.h"

SmokeEmitter::SmokeEmitter()
{
}

void SmokeEmitter::Load(DrawingByRasterize& arg_rasterize)
{
	for (auto& obj : m_smokeParticleArray)
	{
		obj.Load(arg_rasterize);
	}
}

void SmokeEmitter::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_range)
{
	//一定範囲に煙パーティクルをばらまく
	for (auto& obj : m_smokeParticleArray)
	{
		const float scale = KazMath::Rand<float>(0.01f, 0.005f);
		KazMath::Vec3<float>pos(arg_pos);
		pos.x += KazMath::Rand<float>(arg_range.x, -arg_range.x);
		pos.y += KazMath::Rand<float>(arg_range.y, -arg_range.y);
		pos.z += KazMath::Rand<float>(arg_range.z, -arg_range.z);
		obj.Init(pos, KazMath::Vec3<float>(scale, scale, scale), KazMath::Rand<float>(50.0f, 30.0f));
	}
}

void SmokeEmitter::Update()
{
//if ()
//{
//
//}
//
//static int num = 0;
//for (int i = 0; i < m_smokeEmittTimeArray.size(); ++i)
//{
//	if (m_smokeEmittTimeArray[i] <= num && !m_smokeParticleArray[i].IsActive())
//	{
//		const float scale = KazMath::Rand<float>(0.01f, 0.005f);
//		KazMath::Vec3<float>pos(arg_pos);
//		pos.x += KazMath::Rand<float>(arg_range.x, -arg_range.x);
//		pos.y += KazMath::Rand<float>(arg_range.y, -arg_range.y);
//		pos.z += KazMath::Rand<float>(arg_range.z, -arg_range.z);
//		obj.Init(pos, KazMath::Vec3<float>(scale, scale, scale), KazMath::Rand<float>(50.0f, 30.0f));
//
//		m_smokeParticleArray[i].Init();
//	}
//}
//++num;


	for (auto& obj : m_smokeParticleArray)
	{
		obj.Update();
	}
}

void SmokeEmitter::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	for (auto& obj : m_smokeParticleArray)
	{
		obj.Draw(arg_rasterize, arg_blas);
	}
}
