#pragma once
#include <vector>
#include <memory>
#include "Echo.h"
#include "../KazLibrary/Helper/KazBufferHelper.h"
#include "../KazLibrary/Helper/ISinglton.h"

/// <summary>
/// �g�p����Echo�̃f�[�^�������Ă���Array�N���X�B���̃t���[���Ŏg�p����Echo��ǉ����Ă����B
/// </summary>
class EchoArray : public ISingleton<EchoArray>{

private:

	//GPU�ɓ]������\�����o�b�t�@�{�́B
	static const int MAX_ELEMENT_COUNT = 64;
	std::array<Echo::EchoData, MAX_ELEMENT_COUNT> m_echoArray;

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
	/// �z��ɒǉ����鏈���B�uEcho�̃t���O���L��������Ă��邩�v �� �u�z��̗v�f���̏���𒴂��Ȃ����v �����Ēǉ�����B
	/// </summary>
	/// <param name="arg_refEcho"></param>
	void Add(std::weak_ptr<Echo> arg_refEcho);

	KazBufferHelper::BufferData GetEchoStructuredBuffer() { return m_echoStructuredBuffer; }


};