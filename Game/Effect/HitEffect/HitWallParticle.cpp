#include "HitWallParticle.h"

HitWallParticle::HitWallParticle() :m_activeFlag(false)
{
}

void HitWallParticle::Load(DrawingByRasterize& arg_rasterize)
{
	m_render.Load(arg_rasterize, "Resource/Effect/Fire.png");
}

void HitWallParticle::Init(const KazMath::Vec3<float> &arg_pos, const KazMath::Vec3<float> &arg_vel, float arg_radian, float arg_fallSpeed)
{
	m_pos = arg_pos;
	m_vel = arg_vel * 1.0f;
	const float SCALE = 0.05f;
	m_transform.scale = { SCALE,SCALE,SCALE };
	m_transform.Rotation({ 0.0f,0.0f,1.0f }, -arg_radian + KazMath::AngleToRadian(90.0f));
	m_lerpTransform = m_transform;
	m_activeFlag = true;
	m_fallSpeed = arg_fallSpeed;
	m_time.Reset(10.0f);
}

void HitWallParticle::Finalize()
{
	m_transform.quaternion = DirectX::XMQuaternionIdentity();
}

void HitWallParticle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	m_pos += m_vel * m_fallSpeed;

	//m_vel.y += -m_fallSpeed;
	m_transform.scale = m_lerpTransform.scale * m_time.GetInverseTimeRate();
	m_transform.pos = m_pos;
	if (m_time.UpdateTimer())
	{
		m_activeFlag = false;
	}
}

void HitWallParticle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_render.m_tex.Draw3DBillBoard(arg_rasterize, arg_blasVector, m_transform);
}