#include "EchoArray.h"

EchoArray::EchoArray()
{

}

void EchoArray::Setting()
{
	//構造化バッファを生成。
	m_echoStructuredBuffer = KazBufferHelper::SetConstBufferData(sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT, "EchoData");
	m_echoMemoryStructuredBuffer = KazBufferHelper::SetConstBufferData(sizeof(Echo::EchoMemoryData) * MAX_MEMORY_ELEMENT_COUNT, "EchoMemoryData");
}

void EchoArray::Init()
{

	//配列をクリア。
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

	//エコーの更新処理
	for (auto& index : m_echo) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}
	for (auto& index : m_echoMemory) {

		if (!index.GetIsActive()) continue;

		index.Update();

	}

	//更新されたエコーのデータをGPUに転送する配列に詰め込む。
	for (int index = 0; index < MAX_ELEMENT_COUNT; ++index) {

		m_echoArray[index] = m_echo[index].GetEchoData();

	}
	for (int index = 0; index < MAX_MEMORY_ELEMENT_COUNT; ++index) {

		m_echoMemoryArray[index] = m_echoMemory[index].GetEchoMemoryData();
	}

	//GPUにデータを転送する。
	m_echoStructuredBuffer.bufferWrapper->TransData(m_echoArray.data(), sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT);
	m_echoMemoryStructuredBuffer.bufferWrapper->TransData(m_echoMemoryArray.data(), sizeof(Echo::EchoMemoryData) * MAX_MEMORY_ELEMENT_COUNT);

}

void EchoArray::Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, Echo::COLOR arg_colorID)
{

	//エコーを生成。
	for (auto& index : m_echo) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID);

		break;

	}
	//記録用のエコーも同時に生成。
	for (auto& index : m_echoMemory) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID, true);

		break;

	}

}
