#pragma once
#include"../KazLibrary/Math/KazMath.h"

/// <summary>
/// ƒQ[ƒW‚Ì”’lŠÇ—
/// </summary>
class Gauge
{
public:
	Gauge();
	void Init(float arg_maxRate, float arg_nowRate = 0.0f);

	float GetRate();
	bool IsMax();
	void Reset();

	void Incre(float arg_increNum = 1.0f);
	void Decre(float arg_decreNum = 1.0f);

private:
	float m_nowRate, m_maxRate;

};

