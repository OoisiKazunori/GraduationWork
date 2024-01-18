#pragma once
#include"../KazLibrary/Math/KazMath.h"

/// <summary>
/// �Q�[�W�̐��l�Ǘ�
/// </summary>
class Gauge
{
public:
	Gauge();
	/// <param name="arg_maxRate">�Q�[�W�̍ő�l</param>
	/// <param name="arg_rangeFlag">true...-1.0f ~ 1.0f,false...0.0f ~ 1.0f</param>
	/// <param name="arg_nowRate">�Q�[�W�̏����l</param>
	void Init(float arg_maxRate, bool arg_rangeFlag = false, float arg_nowRate = 0.0f);

	float GetRate();
	bool IsMax();
	bool IsEmpty();
	void Reset();

	void Incre(float arg_increNum = 1.0f);
	void Decre(float arg_decreNum = 1.0f);

private:
	float m_nowRate, m_minRate, m_maxRate;

	void Clamp();

};

