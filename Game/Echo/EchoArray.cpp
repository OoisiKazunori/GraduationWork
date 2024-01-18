#include "EchoArray.h"

EchoArray::EchoArray()
{

}

void EchoArray::Setting()
{
	//�\�����o�b�t�@�𐶐��B
	m_echoStructuredBuffer = KazBufferHelper::SetConstBufferData(sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT, "EchoData");
	m_echoMemoryStructuredBuffer = KazBufferHelper::SetConstBufferData(sizeof(Echo::EchoMemoryData) * MAX_MEMORY_ELEMENT_COUNT, "EchoMemoryData");
}

void EchoArray::Init()
{

	//�z����N���A�B
	for (auto& index : m_echoArray) {

		index.m_radius = 0.0f;

	}
	for (auto& index : m_echoMemoryArray) {

		index.m_radius = 0.0f;
		index.m_isActive = false;

	}

}

void EchoArray::Update()
{

	//�G�R�[�̍X�V����
	for (auto& index : m_echo) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}
	for (auto& index : m_echoMemory) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}

	//�X�V���ꂽ�G�R�[�̃f�[�^��GPU�ɓ]������z��ɋl�ߍ��ށB
	for (int index = 0; index < MAX_ELEMENT_COUNT; ++index) {

		m_echoArray[index] = m_echo[index].GetEchoData();

	}
	for (int index = 0; index < MAX_MEMORY_ELEMENT_COUNT; ++index) {

		m_echoMemoryArray[index] = m_echoMemory[index].GetEchoMemoryData();
	}

	//GPU�Ƀf�[�^��]������B
	m_echoStructuredBuffer.bufferWrapper->TransData(m_echoArray.data(), sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT);
	m_echoMemoryStructuredBuffer.bufferWrapper->TransData(m_echoMemoryArray.data(), sizeof(Echo::EchoMemoryData) * MAX_MEMORY_ELEMENT_COUNT);

}

void EchoArray::Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, Echo::COLOR arg_colorID)
{

	//�G�R�[�𐶐��B
	for (auto& index : m_echo) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID);

		break;

	}
	//�L�^�p�̃G�R�[�������ɐ����B
	for (auto& index : m_echoMemory) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID, true);

		break;

	}

}
