#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Game/AI/Sight/ISight.h"

class BoxTypeViewingAngle
{
public:
	BoxTypeViewingAngle();
	bool Collision(const KazMath::Vec3<float>& arg_playerPos, const KazMath::Vec3<float>& arg_enemyPos, const DirectX::XMVECTOR& arg_quaternion);

	const std::array<KazMath::Vec2<float>*, 12>& GetPointPosArray()
	{
		return m_getPointPosArray;
	}
	SightCollision m_sight;
private:
	enum
	{
		LEFT, RIGHT
	};
	std::array<KazMath::Vec2<float>, 2>m_baseNearPointPosArray;
	std::array<KazMath::Vec2<float>, 2>m_baseMiddlePointPosArray;
	std::array<KazMath::Vec2<float>, 2>m_baseFarPointPosArray;
	std::array<KazMath::Vec2<float>*, 12>m_getPointPosArray;

	//視野角の横幅と奥幅
	float m_rangeScale, m_lengthScale;

	const float MIN_RANGE = 15.0f;		//敵の左右の範囲最小
	const float MIDDLE_RANGE = 40.0f;
	const float MAX_RANGE = 15.0f;		//敵の左右の範囲最大
	const float FAR_LENGTH = 80.0f;		//視野の最大距離
	const float MIDDLE_LENGTH = 40.0f;	//視野の範囲が最大まで広くなるまでの距離

};

