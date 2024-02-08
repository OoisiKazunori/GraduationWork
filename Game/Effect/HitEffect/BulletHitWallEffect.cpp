#include "BulletHitWallEffect.h"

BulletHitWallEffect::BulletHitWallEffect()
{
}

void BulletHitWallEffect::Load(DrawingByRasterize& arg_rasterize)
{
	for (auto& obj : m_particle)
	{
		obj.Load(arg_rasterize);
	}
}

void BulletHitWallEffect::Init(const KazMath::Vec3<float>& arg_pos)
{
	std::vector<KazMath::Vec3<float>>v;
	std::vector<float>vRadian;
	int angle = 0;

	const int size = static_cast<int>(m_particle.size());
	for (int i = 0; i < m_particle.size(); ++i)
	{
		int offset = 0;
		const int angle = 90;
		float radian = -KazMath::AngleToRadian(40 + (i + offset) * (angle / size));
		v.emplace_back(KazMath::Vec3<float>(cosf(radian), sinf(radian), KazMath::Rand<float>(1.0f, 0.0f)));
		vRadian.emplace_back(radian);
	}

	int i = 0;
	for (auto& obj : m_particle)
	{
		obj.Finalize();
		//デバック用に一個しか出さない
		obj.Init(arg_pos,
			v[i],
			vRadian[i],
			KazMath::Rand(1.0f, 0.1f)
		);
		++i;
	}
}

void BulletHitWallEffect::Update()
{
	for (auto& obj : m_particle)
	{
		obj.Update();
	}
}

void BulletHitWallEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	for (auto& obj : m_particle)
	{
		obj.Draw(arg_rasterize, arg_blasVector);
	}
}
