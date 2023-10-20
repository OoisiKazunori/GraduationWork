#include "EchoVector.h"

EchoVector::EchoVector()
{
}

void EchoVector::Init()
{

	//�z����N���A�B
	m_echoVector.clear();

}

void EchoVector::Update()
{

	//GPU�Ƀf�[�^��]������B

}

void EchoVector::Add(std::weak_ptr<Echo> arg_refEcho)
{

	//�t���O���L��������Ă��邩�H
	if (!arg_refEcho.lock()->GetIsActive()) return;

	//�v�f�����I�[�o�[���Ȃ����H
	const int NOW_ELEMENT_COUNT = static_cast<int>(m_echoVector.size());
	if (MAX_ELEMENT_COUNT <= NOW_ELEMENT_COUNT - 1) return;

	//�ǉ��B
	m_echoVector.emplace_back(arg_refEcho.lock()->GetEchoData());

}