#pragma once
namespace EnemyConfig
{
	enum Direction
	{
		RIGHT,
		UP,
		LEFT,
		DOWN
	};

	//�`�F�b�N�|�C���g�؍ݎ���
	static const int checkPointDelay = 120;
	//��������,����Ɉڍs����܂ł̎���
	static const int changePatrolDelay = 300;
	//�ő�HP
	static const int maxHP = 2;
	//��������퓬�Ɉڍs����܂ł̎���
	static const int changeCombatDelay = 120;
	//���˕p�x(��)
	static const int shotDelay = 15;
	//���������u�Ԃ̃X���[����
	static const int slowDelay = 120;
	//�X���[�̒x���Ȃ�{��(1.0�����{)
	//static const float slowMag = 0.5f;
	//�ړ����x
	static const float speed = 0.5f;
	//���E����
	static const float eyeCheckDist = 5.0f;
	//�����E������
	static const float soundCheckDist = 50.0f;
	//�d��
	static const float gravity = 0.05f;
}