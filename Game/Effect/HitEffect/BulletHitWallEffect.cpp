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
	for (auto& obj : m_hitCircle)
	{
		obj.Load(arg_rasterize);
	}
}

void BulletHitWallEffect::Init(const KazMath::Vec3<float>& arg_pos, const KazMath::Vec3<float>* arg_playerPos, const KazMath::Color& arg_color)
{
	std::vector<KazMath::Vec3<float>>v;
	std::vector<float>vRadian;
	int angle = 0;

	const int size = static_cast<int>(m_particle.size());
	for (int i = 0; i < m_particle.size(); ++i)
	{
		const int offset = KazMath::Rand<int>(10, -10);
		const int angle = 90;
		float radian = -KazMath::AngleToRadian(i * (angle / size)) - KazMath::AngleToRadian(40) + KazMath::AngleToRadian(offset);
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
			KazMath::Rand(1.0f, 0.8f)
		);
		++i;
	}

	m_hitCircle[0].Init(arg_pos, arg_playerPos, 10.0f, arg_color);
	m_hitCircle[1].Init(arg_pos, arg_playerPos, 40.0f, arg_color);

}

void BulletHitWallEffect::Update()
{
	//for (auto &obj : m_particle)
	//{
	//	obj.Update();
	//}
	for (auto& obj : m_hitCircle)
	{
		obj.Update();
	}
}

void BulletHitWallEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVector)
{
	//for (auto &obj : m_particle)
	//{
	//	obj.Draw(arg_rasterize, arg_blasVector);
	//}
	for (auto& obj : m_hitCircle)
	{
		obj.Draw(arg_rasterize, arg_blasVector);
	}
}
