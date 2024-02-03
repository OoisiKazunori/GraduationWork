#include "BulletFireParticle.h"

BulletFireParticle::BulletFireParticle() :m_activeFlag(false)
{
}

void BulletFireParticle::Load(DrawingByRasterize& arg_rasterize)
{
	m_model.Load(arg_rasterize, "Resource/Test/Box/", "Gard_Hp_Box.gltf");
}

void BulletFireParticle::Init(
	const KazMath::Vec3<float>& arg_emittPos,
	float arg_radian,
	const KazMath::Vec3<float>& arg_size,
	float arg_speed)
{
	Finalize();
	m_activeFlag = true;
	m_radian = arg_radian;
	m_transform.pos = arg_emittPos;
	m_transform.scale = arg_size;
	m_baseScale = arg_size.z * 3.0f;

	int angle = KazMath::RadianToAngle(arg_radian);
	float sign = 1.0f;
	if (0 <= angle && angle <= 90)
	{
		sign *= -1.0f;
	}
	if (270.0f <= angle && angle <= 360.0f)
	{
		sign *= -1.0f;
	}

	//ç∂âEê›íË
	{
		float range = 10.0f;
		float xAngle = KazMath::Rand<float>(range, -range);
		float offsetRadian = KazMath::AngleToRadian(xAngle);
		float xRadian = offsetRadian + arg_radian;
		m_vel += KazMath::Vec3<float>(cosf(xRadian), 0.0f, sinf(xRadian));
		m_transform.Rotation({ 0.0f,1.0f,0.0f }, -xRadian + KazMath::AngleToRadian(90.0f));
	}
	{
		//è„â∫ê›íË
		float range = 10.0f;
		float yAngle = KazMath::Rand<float>(range, -range);
		float yRadian = KazMath::AngleToRadian(yAngle);
		m_transform.Rotation({ 0.0f,0.0f,1.0f }, yRadian * sign);
		m_vel += KazMath::Vec3<float>(0.0f, sinf(-yRadian), 0.0f);
	}

	//à⁄ìÆó ê›íË
	m_vel *= arg_speed;

	m_timer.Reset(120.0f);
}

void BulletFireParticle::Finalize()
{
	m_activeFlag = false;
	m_radian = 0.0f;
	m_transform.pos = {};
	m_transform.scale = { 0.1f,0.1f,2.0f };
	m_transform.quaternion = DirectX::XMQuaternionIdentity();
	m_vel = {};
}

void BulletFireParticle::Update()
{
	if (!m_activeFlag)
	{
		return;
	}
	if (m_timer.UpdateTimer())
	{
		m_activeFlag = false;
	}
	m_transform.pos += m_vel;
	m_transform.scale.z = m_baseScale * m_timer.GetTimeRate();
}

void BulletFireParticle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	if (!m_activeFlag)
	{
		return;
	}
	m_model.m_model.Draw(arg_rasterize, arg_blas, m_transform, KazMath::Color(255, 0, 0, 255));
}

bool BulletFireParticle::IsActive()
{
	return m_activeFlag;
}
