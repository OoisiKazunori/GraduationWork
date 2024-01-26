#pragma once

enum struct State
{
	Patrol,		//巡回
	Warning,	//警戒
	Combat,		//戦闘
	Holdup,		//ホールドアップ(消えそう)
	Death		//死亡
};

struct FindGaugeData
{
	float m_nearRate;//近ければ近いほど貯まる速度を上げる
	float m_offsetWaringRate;//警戒度が上がった際のゲージの増加をさらに増やす
	float m_gaugeIncreRate;//ゲージの増加している値
	float m_gaugeDecreRate;//ゲージの減少している値

	float m_gaugeNowRate;	//発見ゲージの現在の値
};
