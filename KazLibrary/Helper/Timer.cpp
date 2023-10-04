#include "Timer.h"

bool Timer::IsTimeUp() const
{
	return m_intervalTime <= m_elaspedTime;
}

const bool& Timer::IsTimeUpOnTrigger() const
{
	return m_onTimeUpTrigger;
}

float Timer::GetTimeRate(float arg_rate) const
{
	return std::clamp(m_elaspedTime / (m_intervalTime * arg_rate), 0.0f, 1.0f);
}

float Timer::GetTimeRate(float arg_startTimeRate, float arg_endTimeRate) const
{
	auto timeRate = GetTimeRate();
	return std::clamp((timeRate - arg_startTimeRate) / (arg_endTimeRate - arg_startTimeRate), 0.0f, 1.0f);
}

bool Timer::IsTimeStartOnTrigger() const
{
	return GetTimeRate() == 0.0f;
}

float Timer::GetInverseTimeRate() const
{
	return 1.0f - GetTimeRate();
}

float Timer::GetLeftTime() const
{
	if (IsTimeUp())return 0.0f;
	return m_intervalTime - m_elaspedTime;
}

bool Timer::UpdateTimer(float arg_timeScale)
{
	//前フレームでタイムアップしたか
	bool oldTimeUp = IsTimeUp();
	m_elaspedTime += arg_timeScale;

	//タイムアップのトリガー記録
	m_onTimeUpTrigger = !oldTimeUp && IsTimeUp();

	return IsTimeUp();
}
