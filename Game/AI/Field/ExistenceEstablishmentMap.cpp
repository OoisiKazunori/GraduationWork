#include "ExistenceEstablishmentMap.h"

void ExistenceEstablishmentMap::Init(const KazMath::Vec2<int>& arg_mapIDMaxSize, const KazMath::Vec3<float>& arg_basePos)
{
	m_x = arg_mapIDMaxSize.x;
	m_z = arg_mapIDMaxSize.y;
	m_mapDataArray.resize(m_x * m_z);
	m_basePos = arg_basePos;
}

void ExistenceEstablishmentMap::Update()
{
	for (int x = 0; x < m_x; ++x)
	{
		for (int z = 0; z < m_z; ++z)
		{
			//�������Ă��玞�Ԃ��o�ƃv���C���[������\�������܂�
			m_mapDataArray[x * m_x + z].m_rate += 0.1f;
			m_mapDataArray[x * m_x + z].m_rate = std::clamp(m_mapDataArray[x * m_x + z].m_rate, 0.0f, 1.0f);

			//�f�o�b�N�p
			int channel = static_cast<int>(255.0f * m_mapDataArray[x * m_x + z].m_rate);
			FieldAIDebugManager::Instance()->SetGridColorForSearch(
				x,
				z,
				KazMath::Color(channel, 0, 0, 255)
			);
		}
	}

}

void ExistenceEstablishmentMap::Find(SightCollision& arg_sightPoint)
{
	for (int x = 0; x < m_x; ++x)
	{
		for (int z = 0; z < m_z; ++z)
		{
			//���E�ɓ�������T���ς݂��Ɣ��f����
			if (arg_sightPoint.Collision({ m_basePos.x + (float)x,-10.0f,m_basePos.z + (float)z }))
			{
				m_mapDataArray[x * m_x + z].m_rate = 0.0f;
			}
		}
	}
}
