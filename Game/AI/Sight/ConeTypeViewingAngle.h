#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Game/AI/Sight/ISight.h"

class ConeTypeViewingAngle
{
public:
	ConeTypeViewingAngle();
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

	//‹–ìŠp‚Ì‰¡•‚Æ‰œ•
	float m_rangeScale, m_lengthScale;

	const float MIN_RANGE = 5.0f;		//“G‚Ì¶‰E‚Ì”ÍˆÍÅ¬
	const float MAX_RANGE = 10.0f;		//“G‚Ì¶‰E‚Ì”ÍˆÍÅ‘å
	const float FAR_LENGTH = 25.0f;		//‹–ì‚ÌÅ‘å‹——£
	const float MIDDLE_LENGTH = 15.0f;	//‹–ì‚Ì”ÍˆÍ‚ªÅ‘å‚Ü‚ÅL‚­‚È‚é‚Ü‚Å‚Ì‹——£

};

