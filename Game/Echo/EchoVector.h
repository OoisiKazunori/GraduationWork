#pragma once
#include <vector>
#include <memory>
#include "Echo.h"

/// <summary>
/// �g�p����Echo�̃f�[�^�������Ă���vector�N���X�B���̃t���[���Ŏg�p����Echo��ǉ����Ă����B
/// </summary>
class EchoVector {

private:

	std::vector<Echo::EchoData> m_echoVector;
	const int MAX_ELEMENT_COUNT = 64;


public:

	EchoVector();

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


};