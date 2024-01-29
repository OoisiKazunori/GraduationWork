#include "FindGauge.h"

FindGauge::FindGauge() :
	DISTANCE_MAX(200.0f),
	DISTANCE_GAUGE_INCRE_MAX_NUM(1.0f),
	GAUGE_DECRE_NUM(1.0f),
	WARING_INCRE_MAX_NUM(1.0f)
{
	m_gauge.Init(100.0f);
}

void FindGauge::Update(
	const KazMath::Vec3<float>& arg_playerPos,
	const KazMath::Vec3<float>& arg_enemyPos,
	float arg_warningLevel,
	bool arg_isFindFlag
)
{
	const float disntace = arg_playerPos.Distance(arg_enemyPos);
	//�������߂��قǔ����Q�[�W�̑����ʂɃv���X����
	m_debugData.m_nearRate = (1.0f - std::clamp(disntace / DISTANCE_MAX, 0.0f, 1.0f)) * DISTANCE_GAUGE_INCRE_MAX_NUM;
	//�x���x�ɂ��Q�[�W�㏸�ւ̉e��
	const float waringRate = FieldAI::Instance()->GetWaringRate();


	//�㏸����ꍇ
	{
		//����ɓ����Ă���ꍇ�͔����Q�[�W���グ��B�������߂��ꍇ�͏㏸���₷��
		//�������x���x�������ꍇ�͏㏸���������A���������Ⴂ�B�Ⴂ�ꍇ�͋t�ɂȂ�B
		const float DEFAULT_VALUE = 1.0f;
		m_debugData.m_offsetWaringRate = waringRate * WARING_INCRE_MAX_NUM;
		//�x���x��0.0�����͒l������̂ŃN�����v����
		const float CHANGING_VALUE = m_debugData.m_nearRate + m_debugData.m_offsetWaringRate;
		m_debugData.m_gaugeIncreRate = DEFAULT_VALUE + CHANGING_VALUE;
	}
	//��������ꍇ
	{
		const float DEFAULT_VALUE = GAUGE_DECRE_NUM;
		const float CHANGING_VALUE = -waringRate;
		m_debugData.m_gaugeDecreRate = DEFAULT_VALUE + CHANGING_VALUE;
	}
	if (arg_isFindFlag)
	{
		m_gauge.Incre(m_debugData.m_gaugeIncreRate);
	}
	else
	{
		m_gauge.Decre(m_debugData.m_gaugeDecreRate);
	}

	m_debugData.m_gaugeNowRate = m_gauge.GetRate();
}

void FindGauge::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
}

bool FindGauge::IsFind()
{
	return m_gauge.IsMax();
}
