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

	//�ݒ莞�Ԃ��o�߂�����
	bool IsTimeUp()const;

	//�^�C���A�b�v�����u��
	const bool& IsTimeUpOnTrigger()const;

	//�o�ߎ��� / �ݒ莞��
	float GetTimeRate(float arg_rate = 1.0f)const;


	//
	float GetTimeRate(float arg_startTimeRate, float arg_endTimeRate)const;

	//�^�C�}�[�X�^�[�g�����u��
	bool IsTimeStartOnTrigger()const;

	//(1.0f - �o�ߎ��� / �ݒ莞��)
	float GetInverseTimeRate()const;

	//�c�莞��
	float GetLeftTime()const;


	//�o�ߎ���
	const float& GetElaspedTime()const { return m_elaspedTime; }

	//�ݒ莞��
	const float& GetIntervalTime()const { return m_intervalTime; }

	/// <summary>
	/// ���Ԍo��
	/// </summary>
	/// <param name="arg_timeScale">�^�C���X�P�[��</param>
	/// <returns>�ݒ莞�Ԃ��o�߂�����</returns>
	bool UpdateTimer(float arg_timeScale = 1.0f);

	//�����I�Ɏ��Ԍo�ߊ���
	void ForciblyTimeUp()
	{
		m_elaspedTime = m_intervalTime;
	}

	//���Z�b�g
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

	float m_intervalTime;		//�ݒ莞��
	float m_elaspedTime;			//�o�ߎ���
	bool m_onTimeUpTrigger;		//�^�C���A�b�v�g���K�[�L�^

};
