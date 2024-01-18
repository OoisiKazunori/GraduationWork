#include "ConeTypeViewingAngle.h"

ConeTypeViewingAngle::ConeTypeViewingAngle() :m_rangeScale(0.5f), m_lengthScale(0.5f)
{
	//near
	m_baseNearPointPosArray[LEFT] = KazMath::Vec2<float>(-MIN_RANGE * m_rangeScale, 0.0f);
	m_baseNearPointPosArray[RIGHT] = KazMath::Vec2<float>(MIN_RANGE * m_rangeScale, 0.0f);
	//middle
	m_baseMiddlePointPosArray[LEFT] = KazMath::Vec2<float>(-MAX_RANGE * m_rangeScale, MIDDLE_LENGTH);
	m_baseMiddlePointPosArray[RIGHT] = KazMath::Vec2<float>(MAX_RANGE * m_rangeScale, MIDDLE_LENGTH);
	//far
	m_baseFarPointPosArray[LEFT] = KazMath::Vec2<float>(-MAX_RANGE * m_rangeScale, FAR_LENGTH);
	m_baseFarPointPosArray[RIGHT] = KazMath::Vec2<float>(MAX_RANGE * m_rangeScale, FAR_LENGTH);


	m_getPointPosArray[0] = &m_nearPointPosArray[LEFT];
	m_getPointPosArray[1] = &m_nearPointPosArray[RIGHT];
	m_getPointPosArray[2] = &m_nearPointPosArray[RIGHT];
	m_getPointPosArray[3] = &m_middlePointPosArray[RIGHT];
	m_getPointPosArray[4] = &m_middlePointPosArray[RIGHT];
	m_getPointPosArray[5] = &m_farPointPosArray[RIGHT];
	m_getPointPosArray[6] = &m_farPointPosArray[RIGHT];
	m_getPointPosArray[7] = &m_farPointPosArray[LEFT];
	m_getPointPosArray[8] = &m_farPointPosArray[LEFT];
	m_getPointPosArray[9] = &m_middlePointPosArray[LEFT];
	m_getPointPosArray[10] = &m_middlePointPosArray[LEFT];
	m_getPointPosArray[11] = &m_nearPointPosArray[LEFT];

	m_getBasePointPosArray[0] = &m_baseNearPointPosArray[LEFT];
	m_getBasePointPosArray[1] = &m_baseNearPointPosArray[RIGHT];
	m_getBasePointPosArray[2] = &m_baseNearPointPosArray[RIGHT];
	m_getBasePointPosArray[3] = &m_baseMiddlePointPosArray[RIGHT];
	m_getBasePointPosArray[4] = &m_baseMiddlePointPosArray[RIGHT];
	m_getBasePointPosArray[5] = &m_baseFarPointPosArray[RIGHT];
	m_getBasePointPosArray[6] = &m_baseFarPointPosArray[RIGHT];
	m_getBasePointPosArray[7] = &m_baseFarPointPosArray[LEFT];
	m_getBasePointPosArray[8] = &m_baseFarPointPosArray[LEFT];
	m_getBasePointPosArray[9] = &m_baseMiddlePointPosArray[LEFT];
	m_getBasePointPosArray[10] = &m_baseMiddlePointPosArray[LEFT];
	m_getBasePointPosArray[11] = &m_baseNearPointPosArray[LEFT];
}

void ConeTypeViewingAngle::Parent(const KazMath::Vec2<float>& arg_enemyPos, const DirectX::XMVECTOR& arg_quaternion)
{
	for (int i = 0; i < m_getPointPosArray.size(); ++i)
	{
		KazMath::Transform3D mother({ arg_enemyPos.x,0.0f,arg_enemyPos.y }, { 1.0f,1.0f,1.0f });
		KazMath::Transform3D kid({ m_getBasePointPosArray[i]->x,0.0f,m_getBasePointPosArray[i]->y}, {1.0f,1.0f,1.0f});
		mother.quaternion = arg_quaternion;
		kid.SetParent(&mother);

		m_getPointPosArray[i]->x = kid.GetMat().r[3].m128_f32[0];
		m_getPointPosArray[i]->y = kid.GetMat().r[3].m128_f32[2];
	}
}

bool ConeTypeViewingAngle::IsHit(std::array<KazMath::Vec2<float>, 2> arg_pointArray, const KazMath::Vec2<float>& arg_playerPos)
{
	KazMath::Vec2<float>dir(arg_pointArray[1] - arg_pointArray[0]);
	dir.Normalize();
	float cross = dir.Cross(arg_playerPos);
	return 0.0f <= cross;
}

bool ConeTypeViewingAngle::Collision(
	const KazMath::Vec3<float>& arg_playerPos,
	const KazMath::Vec3<float>& arg_enemyPos,
	const DirectX::XMVECTOR& arg_quaternion)
{
	KazMath::Vec2<float>playerPosXZ(arg_playerPos.x, arg_playerPos.z);
	KazMath::Vec2<float>enemyPosXZ(arg_enemyPos.x, arg_enemyPos.z);

	Parent(enemyPosXZ, arg_quaternion);

	//当たり判定
	bool hitFlag = false;
	int countNum = 0;
	//Coneの内外判定
	for (int i = 0; i < m_getPointPosArray.size() / 2; ++i)
	{
		KazMath::Vec2<float>startPos(
			*m_getPointPosArray[i * 2]
		);
		KazMath::Vec2<float>endPos(
			*m_getPointPosArray[i * 2 + 1]
		);
		KazMath::Vec2<float>edgeVec(endPos - startPos);
		KazMath::Vec2<float>pointVec(playerPosXZ - startPos);
		float cross = edgeVec.Cross(pointVec);
		if (0.0f <= cross)
		{
			++countNum;
		}
	}
	//辺と点と外積が全て正なら視界内に入った
	if (m_getPointPosArray.size() / 2 - 1 <= countNum)
	{
		hitFlag = true;
	}
	//高さの範囲の中にいるのか
	if (-10.0f <= arg_playerPos.y && arg_playerPos.y <= 10.0f)
	{
		hitFlag = true;
	}

	return hitFlag;
}
