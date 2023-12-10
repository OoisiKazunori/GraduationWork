#include "FindGauge.h"

FindGauge::FindGauge() :
	DISTANCE_MAX(200.0f),
	DISTANCE_GAUGE_INCRE_MAX_NUM(1.0f),
	GAUGE_DECRE_NUM(1.0f),
	WARING_INCRE_MAX_NUM(1.0f)
{
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
	const float findRate = std::clamp(disntace / DISTANCE_MAX, 0.0f, 1.0f) * DISTANCE_GAUGE_INCRE_MAX_NUM;
	//�x���x�ɂ��Q�[�W�㏸�ւ̉e��
	const float waringRate = FieldAI::Instance()->GetWaringRate();

	//����ɓ����Ă���ꍇ�͔����Q�[�W���グ��B�������߂��ꍇ�͏㏸���₷��
	//�������x���x�������ꍇ�͏㏸���������A���������Ⴂ�B�Ⴂ�ꍇ�͋t�ɂȂ�B
	if (arg_isFindFlag)
	{
		const float DEFAULT_VALUE = 1.0f;
		//�x���x��0.0�����͒l������̂ŃN�����v����
		const float CHANGING_VALUE = findRate + std::clamp(waringRate * WARING_INCRE_MAX_NUM, 0.0f, WARING_INCRE_MAX_NUM);
		m_gauge.Incre(DEFAULT_VALUE + CHANGING_VALUE);
	}
	else
	{
		const float DEFAULT_VALUE = GAUGE_DECRE_NUM;
		const float CHANGING_VALUE = -waringRate;
		m_gauge.Decre(std::clamp(DEFAULT_VALUE + CHANGING_VALUE, 0.1f, 1.0f));
	}
}

void FindGauge::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
}

bool FindGauge::IsFind()
{
	return m_gauge.IsMax();
}
