#pragma once
#include "../KazLibrary/Math/KazMath.h"

/// <summary>
/// �G�R�[�N���X�B�G�R�[���o���e�N���X������������AEchoArray�ɓo�^���邱�Ƃ�GPU�ɒl��������B
/// </summary>
class Echo {

public:

	enum class COLOR {
		WHITE = 0,
		BLUE = 1,
	};

	/// <summary>
	/// GPU�ɑ���G�R�[�̏��BEchoArray��Echo�N���X��n�����Ƃł��̍\���̂̒l���ǉ������B
	/// </summary>
	struct EchoData {
		KazMath::Vec3<float> m_pos;
		float m_radius;
		float m_alpha;
		int m_colorID;
		KazMath::Vec2<int> pad;
	};

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�ɑ���G�R�[�̏��
	/// </summary>
	struct EchoMemoryData {
		KazMath::Vec3<float> m_pos;
		float m_radius;
		float m_alpha;
		int m_isActive;
		KazMath::Vec2<int> pad;
	};

private:

	EchoData m_echoData;				//�G�R�[�Ɋւ���f�[�^
	EchoMemoryData m_echoMemoryData;	//�G�R�[�Ɋւ���f�[�^
	float m_maxEchoRadius;	//�G�R�[�̔��a�̍ő�l
	bool m_isActive;		//���̃G�R�[���L��������Ă��邩�̃t���O

	enum class STATUS {
		APPEAR,
		EXIT,
	}m_status;
	float m_easingTimer;
	const float APPEAR_EASING_TIMER = 12.0f;
	const float EXIT_EASING_TIMER = 16.0f;
	const float EXIT_EASING_TIMER_MEMORY = 1800;
	const float ALPHA = 0.12f;

	bool m_isMemory;	//���̃G�R�[���L�^�p�̂���ǂ����B


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
	void Generate(KazMath::Vec3<float> arg_pos, float arg_maxEchoRadius, COLOR arg_echoColorID, bool arg_isMemory = false);


	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �G�R�[�̐F��ύX�������Ƃ��B
	/// </summary>
	/// <param name="arg_echoColor"> �K�p�������G�R�[�̐F </param>
	void ChangeColorID(COLOR arg_echoColorID) { m_echoData.m_colorID = static_cast<int>(arg_echoColorID); }

	//�e��Q�b�^
	EchoData GetEchoData() { return m_echoData; }
	EchoMemoryData GetEchoMemoryData() { return m_echoMemoryData; }
	KazMath::Vec3<float> GetPos() { return m_echoData.m_pos; }
	float GetNowRadius() { return m_echoData.m_radius; }
	bool GetIsActive() { return m_isActive; }

};