#pragma once
#include"../KazLibrary/Math/KazMath.h"

/// <summary>
/// ‹ŠE‚Ì’ŠÛƒNƒ‰ƒX
/// </summary>
class SightCollision
{
public:

	void AddPoint(const KazMath::Vec2<float>& arg_pointPos);
	void Parent(const KazMath::Vec2<float>& arg_enemyPos, const DirectX::XMVECTOR& arg_quaternion);
	bool Collision(const KazMath::Vec3<float>& arg_playerPos);


	std::vector<KazMath::Vec2<float>>& GetSightVertexPointPosArray()
	{
		return m_sightPointArray;
	}

private:
	std::vector<KazMath::Vec2<float>>m_sightPointArray;
	std::vector<KazMath::Vec2<float>>m_basePointPosArray;
};

