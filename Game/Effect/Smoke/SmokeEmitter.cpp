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

void SmokeEmitter::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_range, int arg_smokeTimer)
{
	m_emittPos = arg_pos;
	m_range = arg_range;
	m_emittTimer = 0;
	m_emittMaxTimer = arg_smokeTimer;

	for (int i = 0; i < m_smokeEmittTimeArray.size(); ++i)
	{
		m_smokeEmittTimeArray[i] = KazMath::Rand<int>(arg_smokeTimer, 0);
	}
}

void SmokeEmitter::Update()
{
	//一定範囲に一定時間、煙パーティクルをばらまく
	for (int i = 0; i < m_smokeEmittTimeArray.size(); ++i)
	{
		if (m_smokeEmittTimeArray[i] <= m_emittTimer && m_emittTimer <= m_emittMaxTimer && !m_smokeParticleArray[i].IsActive())
		{
			const float scale = KazMath::Rand<float>(0.01f, 0.005f);
			KazMath::Vec3<float>pos(m_emittPos);
			pos.x += KazMath::Rand<float>(m_range.x, -m_range.x);
			pos.y += KazMath::Rand<float>(m_range.y, -m_range.y);
			pos.z += KazMath::Rand<float>(m_range.z, -m_range.z);
			m_smokeParticleArray[i].Init(pos, KazMath::Vec3<float>(scale, scale, scale), KazMath::Rand<float>(50.0f, 30.0f));
		}
	}
	++m_emittTimer;

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
