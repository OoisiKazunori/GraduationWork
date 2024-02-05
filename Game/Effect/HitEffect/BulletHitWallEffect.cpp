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
	int angle = 0;
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(0)), KazMath::AngleToRadian(0), 1.0f));
	//v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(360)), KazMath::AngleToRadian(360), 1.0f));

	for (int i = 0; i < 10; ++i)
	{
		v.emplace_back(KazMath::Vec3<float>(cosf(KazMath::AngleToRadian(i * (360 / 10))), sinf(KazMath::AngleToRadian(i * (360 / 10))), KazMath::Rand<float>(1.0f,0.0f)));
	}

	int i = 0;
	for (auto& obj : m_particle)
	{
		obj.Finalize();
		//デバック用に一個しか出さない
		obj.Init(arg_pos,
			v[i]);
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
