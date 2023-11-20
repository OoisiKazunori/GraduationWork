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

	for (auto& index : m_echo) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}

	for (int index = 0; index < MAX_ELEMENT_COUNT; ++index) {

		m_echoArray[index] = m_echo[index].GetEchoData();

	}

	//GPU�Ƀf�[�^��]������B
	m_echoStructuredBuffer.bufferWrapper->TransData(m_echoArray.data(), sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT);

}

void EchoArray::Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, KazMath::Vec3<float> arg_color)
{

	for (auto& index : m_echo) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_color);

		break;

	}

}
