#pragma once
#include"Gauge.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Evaluation/FieldAI.h"

class FindGauge
{
public:
	FindGauge();

	void Update(
		const KazMath::Vec3<float>& arg_playerPos,
		const KazMath::Vec3<float>& arg_enemyPos,
		float arg_warningLevel,
		bool arg_isFindFlag
	);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool IsFind();
private:
	Gauge m_gauge;

	//敵を視認した際にどれくらいの距離からゲージ増加量を増やすか
	const float DISTANCE_MAX;
	//上のゲージ増加に対し最大でどれくらい増やすか
	const float DISTANCE_GAUGE_INCRE_MAX_NUM;
	//発見されてないときにゲージをどれくらい減らすか
	const float GAUGE_DECRE_NUM;
	//警戒時に発見された場合にどれくらい値を増やすべきか
	const float WARING_INCRE_MAX_NUM;
};

