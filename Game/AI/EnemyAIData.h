#pragma once

enum struct State
{
	Patrol,		//����
	Warning,	//�x��
	Combat,		//�퓬
	Holdup,		//�z�[���h�A�b�v(��������)
	Death		//���S
};

struct FindGaugeData
{
	float m_nearRate;//�߂���΋߂��قǒ��܂鑬�x���グ��
	float m_offsetWaringRate;//�x���x���オ�����ۂ̃Q�[�W�̑���������ɑ��₷
	float m_gaugeIncreRate;//�Q�[�W�̑������Ă���l
	float m_gaugeDecreRate;//�Q�[�W�̌������Ă���l

	float m_gaugeNowRate;	//�����Q�[�W�̌��݂̒l
};
