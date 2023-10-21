#include "EchoArray.h"

EchoArray::EchoArray()
{

}

void EchoArray::Setting()
{

	//構造化バッファを生成。
	m_echoStructuredBuffer = KazBufferHelper::SetUploadBufferData(sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT, "EchoStructuredBuffer");

}

void EchoArray::Init()
{

	//配列をクリア。
	for (auto& index : m_echoArray) {

		index.m_radius = 0.0f;

	}

}

void EchoArray::Update()
{

	//GPUにデータを転送する。
	m_echoStructuredBuffer.bufferWrapper->TransData(m_echoArray.data(), sizeof(Echo::EchoData) * MAX_ELEMENT_COUNT);

}

void EchoArray::Add(std::weak_ptr<Echo> arg_refEcho)
{

	//フラグが有効化されているか？
	if (!arg_refEcho.lock()->GetIsActive()) return;

	//Radiusが0のところからどんどん積んでいく。
	for (auto& index : m_echoArray) {

		if (index.m_radius <= 0.0f) continue;

		index = arg_refEcho.lock()->GetEchoData();

		break;

	}

}