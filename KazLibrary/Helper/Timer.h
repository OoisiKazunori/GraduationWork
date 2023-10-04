#pragma once
#include<algorithm>

class Timer
{
public:
	Timer(float arg_intervalTime = 1.0f) :
		m_intervalTime(arg_intervalTime),
		m_elaspedTime(0.0f),
		m_onTimeUpTrigger(false)
	{}

	//設定時間が経過したか
	bool IsTimeUp()const;

	//タイムアップした瞬間
	const bool& IsTimeUpOnTrigger()const;

	//経過時間 / 設定時間
	float GetTimeRate(float arg_rate = 1.0f)const;


	//
	float GetTimeRate(float arg_startTimeRate, float arg_endTimeRate)const;

	//タイマースタートした瞬間
	bool IsTimeStartOnTrigger()const;

	//(1.0f - 経過時間 / 設定時間)
	float GetInverseTimeRate()const;

	//残り時間
	float GetLeftTime()const;


	//経過時間
	const float& GetElaspedTime()const { return m_elaspedTime; }

	//設定時間
	const float& GetIntervalTime()const { return m_intervalTime; }

	/// <summary>
	/// 時間経過
	/// </summary>
	/// <param name="arg_timeScale">タイムスケール</param>
	/// <returns>設定時間が経過したか</returns>
	bool UpdateTimer(float arg_timeScale = 1.0f);

	//強制的に時間経過完了
	void ForciblyTimeUp()
	{
		m_elaspedTime = m_intervalTime;
	}

	//リセット
	void Reset() { m_elaspedTime = 0.0f; }
	void Reset(float arg_interval)
	{
		m_intervalTime = arg_interval;
		Reset();
	}
	void Reset(int arg_interval)
	{
		Reset(static_cast<float>(arg_interval));
	}

	void operator=(float arg) = delete;
private:

	float m_intervalTime;		//設定時間
	float m_elaspedTime;			//経過時間
	bool m_onTimeUpTrigger;		//タイムアップトリガー記録

};
