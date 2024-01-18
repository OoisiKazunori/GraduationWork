#include "ISight.h"

void SightCollision::AddPoint(const KazMath::Vec2<float>& arg_pointPos)
{
	m_sightPointArray.emplace_back();
	m_basePointPosArray.emplace_back(arg_pointPos);
}

void SightCollision::Parent(const KazMath::Vec2<float>& arg_enemyPos, const DirectX::XMVECTOR& arg_quaternion)
{
	for (int i = 0; i < m_sightPointArray.size(); ++i)
	{
		KazMath::Transform3D mother({ arg_enemyPos.x,0.0f,arg_enemyPos.y }, { 1.0f,1.0f,1.0f });
		KazMath::Transform3D kid({ m_basePointPosArray[i].x,0.0f,m_basePointPosArray[i].y }, { 1.0f,1.0f,1.0f });
		mother.quaternion = arg_quaternion;
		kid.SetParent(&mother);

		m_sightPointArray[i].x = kid.GetMat().r[3].m128_f32[0];
		m_sightPointArray[i].y = kid.GetMat().r[3].m128_f32[2];
	}
}

bool SightCollision::Collision(const KazMath::Vec3<float>& arg_playerPos)
{
	KazMath::Vec2<float>playerPosXZ(arg_playerPos.x, arg_playerPos.z);

	//当たり判定
	bool hitFlag = false;
	int countNum = 0;
	//Coneの内外判定
	for (int i = 0; i < m_sightPointArray.size() / 2; ++i)
	{
		KazMath::Vec2<float>startPos(
			m_sightPointArray[i * 2]
		);
		KazMath::Vec2<float>endPos(
			m_sightPointArray[i * 2 + 1]
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
	if (m_sightPointArray.size() / 2 - 1 <= countNum)
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
