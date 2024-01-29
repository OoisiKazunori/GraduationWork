#include "PatrolConfig.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"

PatrolConfig::PatrolConfig(
	size_t arg_x, size_t arg_y, float arg_chipSize)
{
	m_size_x = arg_x;
	m_size_y = arg_y;
	m_chipSize = arg_chipSize;
	m_offset_x = -63.6f;
	m_offset_y = -16.82f;

	m_astarDatas.resize(m_size_x);

	for (int i = 0; i < m_size_x; ++i)
	{
		m_astarDatas[i].
			resize(m_size_y);
	}

	for (int i = 0; i < m_size_x; ++i)
	{
		for (int j = 0; j < m_size_y; ++j)
		{
			m_astarDatas[i][j].type = AstarType::Move;
		}
	}
}

void PatrolConfig::Init()
{
	//PatrolConfig();
}

void PatrolConfig::Update()
{
	if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_LEFT))
	{
		m_offset_x -= m_chipSize;
	}
	else if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_RIGHT))
	{
		m_offset_x += m_chipSize;
	}
	else if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_UP))
	{
		m_offset_y -= m_chipSize;
	}
	else if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_DOWN))
	{
		m_offset_y += m_chipSize;
	}
	else if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_1))
	{
		m_chipSize += 0.01f;
	}
	else if (KeyBoradInputManager::
		Instance()->InputTrigger(DIK_2))
	{
		m_chipSize -= 0.01f;
	}

	float l_chipSize = m_chipSize;

	float l_offset_x = m_offset_x;
	float l_offset_y = m_offset_y;

	for (int i = 0; i < m_size_x; ++i)
	{
		for (int j = 0; j < m_size_y; ++j)
		{
			//箱トランスフォーム
			KazMath::Transform3D l_trans;

			//仮
			l_trans.pos = {
				i * m_chipSize + l_offset_x,
				-20.0f,
				j * m_chipSize + l_offset_y
			};

			l_trans.scale = {
				l_chipSize,
				l_chipSize,
				l_chipSize
			};

			m_astarDatas[i][j].trans = l_trans;
		}
	}
}
