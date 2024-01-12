#pragma once

/// <summary>
/// ヒットストップ設定用構造体
/// </summary>
struct HitStopParam
{
	//適用する時間
	int m_time;
	//適用する速度
	float m_speed;

	//コンストラクタ
	HitStopParam(int time = 0, float speed = 1.0f)
	{
		m_time = time;
		m_speed = speed;
	};
};