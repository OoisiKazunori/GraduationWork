#include "HitWallParticle.h"

HitWallParticle::HitWallParticle() :m_activeFlag(false)
{
}

void HitWallParticle::Load(DrawingByRasterize& arg_rasterize)
{
	for (auto& obj : m_particle)
	{
		obj.Load(arg_rasterize);
	}
}

void HitWallParticle::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>& arg_vel)
{
	m_pos = arg_pos;
	m_vel = arg_vel;
	m_emitterTime = 0;
	m_activeFlag = true;
}

void HitWallParticle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	m_pos += m_vel;
	m_vel.y += -0.01f;

	for (auto& obj : m_particle)
	{
		if (m_emitterTime % 2 != 0 || 120 <= m_emitterTime)
		{
			break;
		}
		if (!obj.IsActive())
		{
			obj.Init(m_pos);
			break;
		}
	}
	for (auto& obj : m_particle)
	{
		obj.Update();
	}
	if (150 <= m_emitterTime)
	{
		m_activeFlag = false;
	}
	++m_emitterTime;
}

void HitWallParticle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	if (!m_activeFlag)
	{
		return;
	}
	for (auto& obj : m_particle)
	{
		obj.Draw(arg_rasterize, arg_blasVector);
	}
}

HitWallParticle::Particle::Particle() :m_activeFlag(false)
{
}

void HitWallParticle::Particle::Load(DrawingByRasterize& arg_rasterize)
{
	m_render.Load(arg_rasterize, "Resource/Effect/Smoke.png");
}

void HitWallParticle::Particle::Init(const KazMath::Vec3<float>& arg_pos)
{
	m_activeFlag = true;
	m_lerpTransform.scale = { 0.01f,0.01f,0.01f };
	m_transform.scale = { 0.0f,0.0f,0.0f };
	m_transform.pos = arg_pos;
	m_lerpTransform.pos = m_transform.pos;
	m_time.Reset(60.0f);
}

void HitWallParticle::Particle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	KazMath::Larp(m_transform.scale, &m_lerpTransform.scale, 0.1f);
	if (m_lerpTransform.scale.x <= 0.0f || m_time.UpdateTimer())
	{
		m_activeFlag = false;
	}
}

void HitWallParticle::Particle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_render.m_tex.Draw3DBillBoard(arg_rasterize, arg_blasVector, m_lerpTransform);
}

bool HitWallParticle::Particle::IsActive()
{
	return m_activeFlag;
}
