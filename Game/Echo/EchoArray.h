#pragma once
#include <vector>
#include <memory>
#include "Echo.h"
#include "../KazLibrary/Helper/KazBufferHelper.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include "../Game/Echo/Echo.h"

/// <summary>
/// �g�p����Echo�̃f�[�^�������Ă���Array�N���X�B���̃t���[���Ŏg�p����Echo��ǉ����Ă����B
/// </summary>
class EchoArray : public ISingleton<EchoArray>{

private:

	//GPU�ɓ]������\�����o�b�t�@�{�́B
	static const int MAX_ELEMENT_COUNT = 64;
	std::array<Echo::EchoData, MAX_ELEMENT_COUNT> m_echoArray;

	//Echo�N���X
	std::array<Echo, MAX_ELEMENT_COUNT> m_echo;

	//�\�����o�b�t�@
	KazBufferHelper::BufferData m_echoStructuredBuffer;


public:

	EchoArray();

	/// <summary>
	/// �\�����o�b�t�@�Ȃǂ𐶐�����B
	/// </summary>
	void Setting();

	/// <summary>
	/// ���������� ��ɔz����N���A���鏈���������\��B�V�[���̍X�V�����̈�Ԑ擪�Ɏ����Ă������B
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V���� ���GPU�Ƀf�[�^��]�����鏈���������\��B�V�[���̍X�V�����̉��̕��Ɏ����Ă������B
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="arg_pos"></param>
	/// <param name="arg_maxRadius"></param>
	/// <param name="arg_color"></param>
	void Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, KazMath::Vec3<float> arg_color);

	KazBufferHelper::BufferData* GetEchoStructuredBuffer() { return &m_echoStructuredBuffer; }
	std::array<Echo, MAX_ELEMENT_COUNT>& GetEcho() { return m_echo; }


};