#include "SmokeParticle.h"

SmokeParticle::SmokeParticle() :m_activeFlag(false)
{
}

void SmokeParticle::Load(DrawingByRasterize& arg_rasterize)
{
	m_smokeRender.m_tex.Load(arg_rasterize, "Resource/Effect/Smoke.png", false);
}

void SmokeParticle::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_scale, float arg_emittTimer)
{
	m_transform.pos = arg_pos;
	m_transform.scale = arg_scale;
	m_baseTransform.pos = arg_pos;
	m_baseTransform.scale = arg_scale;

	m_timer.Reset(arg_emittTimer);
	m_activeFlag = true;

	m_speed = KazMath::Rand<float>(0.1f, 0.01f);

	m_angle = 0;
	m_angleVel = KazMath::Rand<float>(0.5f, 0.1f);
}

void SmokeParticle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	//è„è∏Ç∆âÒì]
	m_transform.pos.y += m_speed;
	m_angle += m_angleVel;
	m_transform.Rotation({ 0.0f,0.0f,1.0f }, KazMath::AngleToRadian(m_angle));
	//èkè¨äJén
	const float startShirinkScale = 0.2f;
	if (m_timer.GetInverseTimeRate() <= startShirinkScale)
	{
		m_transform.scale = m_baseTransform.scale * (m_timer.GetInverseTimeRate() / startShirinkScale);
	}
	//éûä‘êÿÇÍÇÕèIóπ
	m_activeFlag = !m_timer.UpdateTimer();
}

void SmokeParticle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_smokeRender.m_tex.Draw3DBillBoard(arg_rasterize, arg_blas, m_transform, KazMath::Color(100, 100, 100, 255));
}

const bool& SmokeParticle::IsActive()
{
	return m_activeFlag;
}
