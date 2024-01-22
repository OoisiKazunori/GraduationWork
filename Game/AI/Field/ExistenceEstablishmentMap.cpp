#include "ExistenceEstablishmentMap.h"

void ExistenceEstablishmentMap::Init(int arg_x, int arg_y)
{
	m_x = arg_x;
	m_z = arg_y;
	m_mapDataArray.resize(m_x * m_z);
}

void ExistenceEstablishmentMap::Update()
{
	for (int x = 0; x < m_x; ++x)
	{
		for (int z = 0; z < m_z; ++z)
		{
			//発見してから時間が経つとプレイヤーが居る可能性が高まる
			m_mapDataArray[x * m_x + z].m_rate += 0.01f;
			m_mapDataArray[x * m_x + z].m_rate = std::clamp(m_mapDataArray[x * m_x + z].m_rate, 0.0f, 1.0f);

			//デバック用
			int channel = static_cast<int>(255.0f * m_mapDataArray[x * m_x + z].m_rate);
			/*FieldAIDebugManager::Instance()->SetGridColorForSearch(
				x,
				z,
				KazMath::Color(channel, channel, channel, 255)
			);*/
		}
	}

}

void ExistenceEstablishmentMap::Find(SightCollision& arg_sightPoint)
{
	for (int x = 0; x < m_x; ++x)
	{
		for (int z = 0; z < m_z; ++z)
		{
			//視界に入ったら探索済みだと判断する
			if (arg_sightPoint.Collision({ (float)x,0.0f,(float)z }))
			{
				m_mapDataArray[x * m_x + z].m_rate = 0.0f;
			}
		}
	}
}
