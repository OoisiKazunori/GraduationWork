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
	//距離が近いほど発見ゲージの増加量にプラスする
	const float findRate = std::clamp(disntace / DISTANCE_MAX, 0.0f, 1.0f) * DISTANCE_GAUGE_INCRE_MAX_NUM;
	//警戒度によるゲージ上昇への影響
	const float waringRate = FieldAI::Instance()->GetWaringRate();

	//視野に入っている場合は発見ゲージを上げる。距離が近い場合は上昇しやすい
	//ただし警戒度が高い場合は上昇率が高く、減少率が低い。低い場合は逆になる。
	if (arg_isFindFlag)
	{
		const float DEFAULT_VALUE = 1.0f;
		//警戒度が0.0未満は値が減るのでクランプする
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
