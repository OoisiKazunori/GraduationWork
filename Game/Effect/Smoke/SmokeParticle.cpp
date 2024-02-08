#include "SmokeParticle.h"
#include"../KazLibrary/Easing/easing.h"

SmokeParticle::SmokeParticle() :m_activeFlag(false)
{
}

void SmokeParticle::Load(DrawingByRasterize& arg_rasterize)
{
	m_smokeRender.Load(arg_rasterize, "Resource/Effect/Smoke.png", false);
}

void SmokeParticle::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_scale, float arg_emittTimer, const KazMath::Color& arg_color)
{
	m_transform.pos = arg_pos;
	m_transform.scale = arg_scale;
	m_baseTransform.pos = arg_pos;
	m_baseTransform.scale = arg_scale;

	m_timer.Reset(arg_emittTimer);
	m_activeFlag = true;

	m_speed = KazMath::Rand<float>(0.1f, 0.01f);

	m_angle = 0;
	m_angleVel = KazMath::Rand<float>(0.05f, 0.01f);

	if (KazMath::Rand<int>(2, 0) == 0)
	{
		m_xVel = 1.0f;
	}
	else
	{
		m_xVel = -1.0f;
	}

	m_color = arg_color;

	m_xEaseTime.Reset(60.0f);
}

void SmokeParticle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}

	//m_transform.pos.x = m_baseTransform.pos.x + sinf(m_timer.GetTimeRate() * (DirectX::XM_2PI / 180.0f)) * 10.0f;
	m_xEaseTime.UpdateTimer();
	m_transform.pos.x = m_baseTransform.pos.x + EasingMaker(Out, Exp, m_xEaseTime.GetTimeRate()) * (0.5f * m_xVel);

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
	m_smokeRender.m_tex.Draw3DBillBoard(arg_rasterize, arg_blas, m_transform, m_color);
}

const bool& SmokeParticle::IsActive()
{
	return m_activeFlag;
}
