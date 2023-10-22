#include "EchoArray.h"

EchoArray::EchoArray()
{

}

void EchoArray::Setting()
{

	//�\�����o�b�t�@�𐶐��B
	m_echoStructuredBuffer = KazBufferHelper::SetConstBufferData(sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT, "EchoData");

}

void EchoArray::Init()
{

	//�z����N���A�B
	for (auto& index : m_echoArray) {

		index.m_radius = 0.0f;

	}

}

void EchoArray::Update()
{

	//GPU�Ƀf�[�^��]������B
	m_echoStructuredBuffer.bufferWrapper->TransData(m_echoArray.data(), sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT);

}

void EchoArray::Add(std::weak_ptr<Echo> arg_refEcho)
{

	//�t���O���L��������Ă��邩�H
	if (!arg_refEcho.lock()->GetIsActive()) return;

	//Radius��0�̂Ƃ��납��ǂ�ǂ�ς�ł����B
	for (auto& index : m_echoArray) {

		if (0.0f < index.m_radius) continue;

		index = arg_refEcho.lock()->GetEchoData();

		break;

	}

}