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

void SmokeEmitter::Init(const EmittData& arg_emittData)
{
	m_emittPos = arg_emittData.m_emittPos;
	m_range = arg_emittData.m_range;
	m_emittTimer = 0;
	m_emittMaxTimer = arg_emittData.m_smokeTime;
	m_color = arg_emittData.m_color;
	for (int i = 0; i < m_smokeEmittTimeArray.size(); ++i)
	{
		m_smokeEmittTimeArray[i] = KazMath::Rand<int>(m_emittMaxTimer, 0);
	}
	m_loopFlag = arg_emittData.m_loopFlag;

	m_minScale = arg_emittData.m_minScale;
	m_maxScale = arg_emittData.m_maxScale;

	m_minActiveTime = arg_emittData.m_minActiveTime;
	m_maxActiveTime = arg_emittData.m_maxActiveTime;
}

void SmokeEmitter::Update()
{
	//ループ処理
	if (m_loopFlag && m_emittMaxTimer <= m_emittTimer)
	{
		m_emittTimer = 0;
	}

	//一定範囲に一定時間、煙パーティクルをばらまく
	for (int i = 0; i < m_smokeEmittTimeArray.size(); ++i)
	{
		const bool emittFlag = m_smokeEmittTimeArray[i] <= m_emittTimer && !m_smokeParticleArray[i].IsActive();
		const bool inTimeFlag = m_emittTimer <= m_emittMaxTimer;

		if (emittFlag && inTimeFlag)
		{
			const float scale = KazMath::Rand<float>(m_maxScale, m_minScale);
			KazMath::Vec3<float>pos(m_emittPos);
			pos.x += KazMath::Rand<float>(m_range.x, -m_range.x);
			pos.y += KazMath::Rand<float>(m_range.y, -m_range.y);
			pos.z += KazMath::Rand<float>(m_range.z, -m_range.z);
			m_smokeParticleArray[i].Init(pos, KazMath::Vec3<float>(scale, scale, scale), KazMath::Rand<float>(m_maxActiveTime, m_minActiveTime), m_color);
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
