#pragma once
#include "../KazLibrary/Math/KazMath.h"

/// <summary>
/// �G�R�[�N���X�B�G�R�[���o���e�N���X������������AEchoArray�ɓo�^���邱�Ƃ�GPU�ɒl��������B
/// </summary>
class Echo {

public:

	/// <summary>
	/// GPU�ɑ���G�R�[�̏��BEchoArray��Echo�N���X��n�����Ƃł��̍\���̂̒l���ǉ������B
	/// </summary>
	struct EchoData {
		KazMath::Vec3<float> m_pos;
		float m_radius;
		KazMath::Vec3<float> m_color;
		float m_alpha;
	};

private:

	EchoData m_echoData;	//�G�R�[�Ɋւ���f�[�^
	float m_maxEchoRadius;	//�G�R�[�̔��a�̍ő�l
	bool m_isActive;		//���̃G�R�[���L��������Ă��邩�̃t���O

	enum class STATUS {
		APPEAR,
		EXIT,
	}m_status;
	float m_easingTimer;
	const float APPEAR_EASING_TIMER = 12.0f;
	const float EXIT_EASING_TIMER = 16.0f;
	const float ALPHA = 0.12f;


public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Echo();

	/// <summary>
	/// ���������� �����I�ɃG�R�[��؂�B
	/// </summary>
	void Init();

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="arg_maxEchoRadius"> �G�R�[�̓��B���a </param>
	/// <param name="arg_echoColor"> �G�R�[�̐F </param>
	void Generate(KazMath::Vec3<float> arg_pos, float arg_maxEchoRadius, KazMath::Vec3<float> arg_echoColor);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �G�R�[�̐F��ύX�������Ƃ��B
	/// </summary>
	/// <param name="arg_echoColor"> �K�p�������G�R�[�̐F </param>
	void ChangeColor(KazMath::Vec3<float> arg_echoColor) { m_echoData.m_color = arg_echoColor; }

	//�e��Q�b�^
	EchoData GetEchoData() { return m_echoData; }
	KazMath::Vec3<float> GetPos() { return m_echoData.m_pos; }
	float GetNowRadius() { return m_echoData.m_radius; }
	bool GetIsActive() { return m_isActive; }

};