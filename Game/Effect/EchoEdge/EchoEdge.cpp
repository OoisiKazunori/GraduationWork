#include "EchoEdge.h"
#include"../KazLibrary/Buffer/GBufferMgr.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

EchoEdge::EchoEdge()
{
	//出力用
	m_outputBuffer = KazBufferHelper::SetUAVTexBuffer(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM, "EdgeDistortionBuffer");
	m_outputBuffer.bufferWrapper->CreateViewHandle(UavViewHandleMgr::Instance()->GetHandle());
	DescriptorHeapMgr::Instance()->CreateBufferView(
		m_outputBuffer.bufferWrapper->GetViewHandle(),
		KazBufferHelper::SetUnorderedAccessTextureView(sizeof(DirectX::XMFLOAT4), 1280 * 720),
		m_outputBuffer.bufferWrapper->GetBuffer().Get()
	);
}

void EchoEdge::Generate(const KazBufferHelper::BufferData& arg_outlineBuffer)
{
	//ワールド座標
	m_bufferArray.emplace_back(RenderTargetStatus::Instance()->GetBuffer(GBufferMgr::Instance()->GetRenderTarget()[GBufferMgr::WORLD]));
	m_bufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
	m_bufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;
	//エコー配列
	m_bufferArray.emplace_back();
	m_bufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_bufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
	//Outline処理済みの描画
	m_bufferArray.emplace_back(arg_outlineBuffer);
	m_bufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_bufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;
	//出力
	m_bufferArray.emplace_back(m_outputBuffer);
	m_bufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_bufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX2;

	m_computeEdge.Generate(
		ShaderOptionData(KazFilePathName::RelativeShaderPath + "PostEffect/EchoEdge/" + "Edge.hlsl", "main", "cs_6_4", SHADER_TYPE_COMPUTE),
		m_bufferArray
	);
}

void EchoEdge::Compute()
{
	m_computeEdge.m_extraBufferArray[1] = *EchoArray::Instance()->GetEchoStructuredBuffer();
	m_computeEdge.m_extraBufferArray[1].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_computeEdge.m_extraBufferArray[1].rootParamType = GRAPHICS_PRAMTYPE_DATA;


	m_bufferArray[0].bufferWrapper->ChangeBarrier(D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	DispatchData dispatchData;
	dispatchData.x = static_cast<UINT>(1280 / 16) + 1;
	dispatchData.y = static_cast<UINT>(720 / 16) + 1;
	dispatchData.z = static_cast<UINT>(1);
	m_computeEdge.Compute(dispatchData);

	m_bufferArray[0].bufferWrapper->ChangeBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}
