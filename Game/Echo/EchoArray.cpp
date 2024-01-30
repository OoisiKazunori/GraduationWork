#include "EchoArray.h"

EchoArray::EchoArray()
{

}

void EchoArray::Setting()
{
	//構造化バッファを生成。
	m_echoStructuredBuffer = KazBufferHelper::SetConstBufferData(sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT, "EchoData");
	m_echoMemoryStructuredBuffer = KazBufferHelper::SetUploadBufferData(sizeof(Echo::EchoMemoryData) * MAX_MEMORY_ELEMENT_COUNT, "EchoMemoryData");
	m_echoMemoryStructuredVRAMBuffer = KazBufferHelper::SetGPUBufferData(sizeof(Echo::EchoMemoryData) * MAX_MEMORY_ELEMENT_COUNT, "EchoMemoryData");
	m_echoMemoryStructuredVRAMBuffer.bufferWrapper->ChangeBarrierUAV();

	m_echoMemoryStructuredVRAMBuffer.rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
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

	m_echoMemoryStructuredVRAMBuffer.bufferWrapper->CopyBuffer(m_echoMemoryStructuredBuffer.bufferWrapper->GetBuffer());

}

void EchoArray::Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, Echo::COLOR arg_colorID, float arg_memoryTimer)
{

	//エコーを生成。
	for (auto& index : m_echo) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID, arg_memoryTimer);

		break;

	}
	//記録用のエコーも同時に生成。
	for (auto& index : m_echoMemory) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID, arg_memoryTimer, true);

		break;

	}

}

void EchoArray::Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, Echo::COLOR arg_colorID)
{

	//エコーを生成。
	for (auto& index : m_echo) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID, 1800);

		break;

	}
	//記録用のエコーも同時に生成。
	for (auto& index : m_echoMemory) {

		if (index.GetIsActive()) continue;

		index.Generate(arg_pos, arg_maxRadius, arg_colorID, 1800, true);

		break;

	}

}
