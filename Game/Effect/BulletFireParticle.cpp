#include "BulletFireParticle.h"

BulletFireParticle::BulletFireParticle() :m_activeFlag(false)
{
}

void BulletFireParticle::Init(const KazMath::Vec3<float>& arg_emittPos)
{
	m_activeFlag = true;
}

void BulletFireParticle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	m_transform.quaternion = DirectX::XMQuaternionRotationAxis();
}

void BulletFireParticle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_model.m_model.Draw(arg_rasterize, arg_blas, m_transform);
}

bool BulletFireParticle::IsActive()
{
	return m_activeFlag;
}
