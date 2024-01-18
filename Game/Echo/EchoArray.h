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

	//���X�^���C�Y�ɑ���Echo�̋L�^���
	static const int MAX_MEMORY_ELEMENT_COUNT = 256;
	std::array<Echo::EchoMemoryData, MAX_MEMORY_ELEMENT_COUNT> m_echoMemoryArray;

	//Echo�N���X
	std::array<Echo, MAX_ELEMENT_COUNT> m_echo;
	std::array<Echo, MAX_MEMORY_ELEMENT_COUNT> m_echoMemory;

	//�\�����o�b�t�@
	KazBufferHelper::BufferData m_echoStructuredBuffer;
	KazBufferHelper::BufferData m_echoMemoryStructuredBuffer;


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
	void Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, Echo::COLOR arg_colorID);

	KazBufferHelper::BufferData* GetEchoStructuredBuffer() { return &m_echoStructuredBuffer; }
	const KazBufferHelper::BufferData &GetEchoMemoryStructuredBuffer() { return m_echoMemoryStructuredBuffer; }
	std::array<Echo, MAX_ELEMENT_COUNT>& GetEcho() { return m_echo; }


};