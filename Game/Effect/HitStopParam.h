#pragma once

/// <summary>
/// �q�b�g�X�g�b�v�ݒ�p�\����
/// </summary>
struct HitStopParam
{
	//�K�p���鎞��
	int m_time;
	//�K�p���鑬�x
	float m_speed;

	//�R���X�g���N�^
	HitStopParam(int time = 0, float speed = 1.0f)
	{
		m_time = time;
		m_speed = speed;
	};
};