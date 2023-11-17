#include "PatrolConfig.h"

PatrolConfig::PatrolConfig(
	size_t arg_x, size_t arg_y, float arg_chipSize)
{
	m_size_x = arg_x;
	m_size_y = arg_y;
	m_chipSize = arg_chipSize;

	m_astarDatas.resize(m_size_x);

	for (int i = 0; i < m_size_x; ++i)
	{
		m_astarDatas[i].
			resize(m_size_y);
	}
}

void PatrolConfig::Update()
{
	for (int i = 0; i < m_size_x; ++i) {
		for (int j = 0; j < m_size_y; ++j) {
			m_astarDatas[i][j].trans.scale = {
				m_chipSize,
				m_chipSize,
				m_chipSize
			};
		}
	}
}
