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
		const int angle = 180;
		float radian = -KazMath::AngleToRadian(i * (angle / size));
		v.emplace_back(KazMath::Vec3<float>(cosf(radian), sinf(radian), KazMath::Rand<float>(1.0f, 0.0f)));
		vRadian.emplace_back(radian);
	}

	int i = 0;
	for (auto& obj : m_particle)
	{
		obj.Finalize();
		KazMath::Vec2<float>posXZ(arg_pos.x, arg_pos.z);
		KazMath::Vec2<float>rPos(posXZ + v[i].ConvertVec2() * 5.0f);
		KazMath::Vec2<float>vec = posXZ - rPos;
		vec.Normalize();
		float radius = atan2(vec.y, vec.x);

		obj.Init(arg_pos,
			v[i],
			radius,
			KazMath::Rand(1.0f, 0.5f)
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
