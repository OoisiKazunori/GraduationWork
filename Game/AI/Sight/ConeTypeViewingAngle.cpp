#include "ConeTypeViewingAngle.h"

ConeTypeViewingAngle::ConeTypeViewingAngle() :m_rangeScale(1.0f), m_lengthScale(1.0f)
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

	//視野角の形成
	m_sight.AddPoint(m_baseNearPointPosArray[LEFT]);
	m_sight.AddPoint(m_baseNearPointPosArray[RIGHT]);
	m_sight.AddPoint(m_baseNearPointPosArray[RIGHT]);
	m_sight.AddPoint(m_baseMiddlePointPosArray[RIGHT]);
	m_sight.AddPoint(m_baseMiddlePointPosArray[RIGHT]);
	m_sight.AddPoint(m_baseFarPointPosArray[RIGHT]);
	m_sight.AddPoint(m_baseFarPointPosArray[RIGHT]);
	m_sight.AddPoint(m_baseFarPointPosArray[LEFT]);
	m_sight.AddPoint(m_baseFarPointPosArray[LEFT]);
	m_sight.AddPoint(m_baseMiddlePointPosArray[LEFT]);
	m_sight.AddPoint(m_baseMiddlePointPosArray[LEFT]);
	m_sight.AddPoint(m_baseNearPointPosArray[LEFT]);

	//デバック用の描画座標渡し
	for (int i = 0; i < m_sight.GetSightVertexPointPosArray().size(); ++i)
	{
		m_getPointPosArray[i] = &m_sight.GetSightVertexPointPosArray()[i];
	}
}

bool ConeTypeViewingAngle::Collision(
	const KazMath::Vec3<float>& arg_playerPos,
	const KazMath::Vec3<float>& arg_enemyPos,
	const DirectX::XMVECTOR& arg_quaternion)
{
	KazMath::Vec2<float>enemyPosXZ(arg_enemyPos.x, arg_enemyPos.z);
	m_sight.Parent(enemyPosXZ, arg_quaternion);
	//当たり判定
	return m_sight.Collision(arg_playerPos);
}
