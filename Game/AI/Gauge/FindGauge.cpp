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
	//距離が近いほど発見ゲージの増加量にプラスする
	m_debugData.m_nearRate = (1.0f - std::clamp(disntace / DISTANCE_MAX, 0.0f, 1.0f)) * DISTANCE_GAUGE_INCRE_MAX_NUM;
	//警戒度によるゲージ上昇への影響
	const float waringRate = FieldAI::Instance()->GetWaringRate();


	//上昇する場合
	{
		//視野に入っている場合は発見ゲージを上げる。距離が近い場合は上昇しやすい
		//ただし警戒度が高い場合は上昇率が高く、減少率が低い。低い場合は逆になる。
		const float DEFAULT_VALUE = 1.0f;
		m_debugData.m_offsetWaringRate = waringRate * WARING_INCRE_MAX_NUM;
		//警戒度が0.0未満は値が減るのでクランプする
		const float CHANGING_VALUE = m_debugData.m_nearRate + m_debugData.m_offsetWaringRate;
		m_debugData.m_gaugeIncreRate = DEFAULT_VALUE + CHANGING_VALUE;
	}
	//減少する場合
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
