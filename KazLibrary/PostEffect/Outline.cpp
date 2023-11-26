#include "Outline.h"
#include "Buffer/DescriptorHeapMgr.h"
#include "Buffer/UavViewHandleMgr.h"
#include "../PostEffect/GaussianBlur.h"

PostEffect::Outline::Outline(KazBufferHelper::BufferData arg_outlineTargetWorld, KazBufferHelper::BufferData arg_outlineTargetNormal, KazBufferHelper::BufferData arg_silhouetteRenderTargetBuffer, KazBufferHelper::BufferData arg_eyeBuffer, KazBufferHelper::BufferData arg_silhouetteBuffer)
{

	//アウトラインをかける対象のテクスチャを保存しておく。
	m_outlineTargetWorld = arg_outlineTargetWorld;
	m_outlineTargetNormal = arg_outlineTargetNormal;

	//アウトラインの色
	m_outputAlbedoTexture = KazBufferHelper::SetUAVTexBuffer(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_outputAlbedoTexture.bufferWrapper->CreateViewHandle(UavViewHandleMgr::Instance()->GetHandle());
	DescriptorHeapMgr::Instance()->CreateBufferView(
		m_outputAlbedoTexture.bufferWrapper->GetViewHandle(),
		KazBufferHelper::SetUnorderedAccessTextureView(sizeof(DirectX::XMFLOAT4), 1280 * 720),
		m_outputAlbedoTexture.bufferWrapper->GetBuffer().Get()
	);

	//アウトラインのエミッシブ
	m_outputEmissiveTexture = KazBufferHelper::SetUAVTexBuffer(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_outputEmissiveTexture.bufferWrapper->CreateViewHandle(UavViewHandleMgr::Instance()->GetHandle());
	DescriptorHeapMgr::Instance()->CreateBufferView(
		m_outputEmissiveTexture.bufferWrapper->GetViewHandle(),
		KazBufferHelper::SetUnorderedAccessTextureView(sizeof(DirectX::XMFLOAT4), 1280 * 720),
		m_outputEmissiveTexture.bufferWrapper->GetBuffer().Get()
	);

	//アウトラインの色
	m_outlineData.m_color = KazMath::Vec4<float>(0.90f, 0.94f, 0.94f, 1);
	m_outlineData.m_outlineLength = 300.0f;
	m_outlineColorConstBuffer = KazBufferHelper::SetConstBufferData(sizeof(OutlineData));
	m_outlineColorConstBuffer.bufferWrapper->TransData(&m_outlineData, sizeof(OutlineData));

	//エコー関連
	m_echoData.m_color = KazMath::Vec3<float>(0.24f, 0.50f, 0.64f);
	m_echoData.m_center = KazMath::Vec3<float>(0.0f, 0.0f, 0.0f);
	m_echoData.m_echoAlpha = 0.0f;
	m_echoData.m_echoRadius = 0.0f;
	m_echoConstBuffer = KazBufferHelper::SetConstBufferData(sizeof(EchoData));
	m_echoConstBuffer.bufferWrapper->TransData(&m_echoData, sizeof(EchoData));

	m_inputOutlineWorldTexture = arg_silhouetteRenderTargetBuffer;

	//アウトライン計算用のシェーダー
	{
		std::vector<KazBufferHelper::BufferData>extraBuffer =
		{
			 m_outlineTargetWorld,
			 m_outlineTargetNormal,
			 m_inputOutlineWorldTexture,
			 m_outputAlbedoTexture,
			 m_outputEmissiveTexture,
			 arg_silhouetteBuffer,
			 m_outlineColorConstBuffer,
			 m_echoConstBuffer,
			 arg_eyeBuffer,
		};
		extraBuffer[0].rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		extraBuffer[0].rootParamType = GRAPHICS_PRAMTYPE_TEX;

		extraBuffer[1].rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		extraBuffer[1].rootParamType = GRAPHICS_PRAMTYPE_TEX2;

		extraBuffer[2].rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		extraBuffer[2].rootParamType = GRAPHICS_PRAMTYPE_TEX3;

		extraBuffer[3].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		extraBuffer[3].rootParamType = GRAPHICS_PRAMTYPE_TEX;

		extraBuffer[4].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		extraBuffer[4].rootParamType = GRAPHICS_PRAMTYPE_TEX2;

		extraBuffer[5].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		extraBuffer[5].rootParamType = GRAPHICS_PRAMTYPE_TEX3;

		extraBuffer[6].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
		extraBuffer[6].rootParamType = GRAPHICS_PRAMTYPE_DATA;

		extraBuffer[7].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
		extraBuffer[7].rootParamType = GRAPHICS_PRAMTYPE_DATA2;

		extraBuffer[8].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
		extraBuffer[8].rootParamType = GRAPHICS_PRAMTYPE_DATA3;
		m_outlineShader.Generate(ShaderOptionData(KazFilePathName::RelativeShaderPath + "PostEffect/Outline/" + "Outline.hlsl", "main", "cs_6_4", SHADER_TYPE_COMPUTE), extraBuffer);
	}

}

void PostEffect::Outline::Apply()
{
	std::vector<D3D12_RESOURCE_BARRIER> barrier;

	barrier.emplace_back(CD3DX12_RESOURCE_BARRIER::UAV(m_outputAlbedoTexture.bufferWrapper->GetBuffer().Get()));
	barrier.emplace_back(CD3DX12_RESOURCE_BARRIER::UAV(m_outputEmissiveTexture.bufferWrapper->GetBuffer().Get()));

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(static_cast<UINT>(barrier.size()), barrier.data());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_outlineTargetWorld.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_outlineTargetNormal.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_inputOutlineWorldTexture.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));



	m_outlineColorConstBuffer.bufferWrapper->TransData(&m_outlineData, sizeof(OutlineData));
	m_echoConstBuffer.bufferWrapper->TransData(&m_echoData, sizeof(EchoData));
	DispatchData dispatchData;
	dispatchData.x = static_cast<UINT>(1280 / 16) + 1;
	dispatchData.y = static_cast<UINT>(720 / 16) + 1;
	dispatchData.z = static_cast<UINT>(1);
	m_outlineShader.Compute(dispatchData);

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(static_cast<UINT>(barrier.size()), barrier.data());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_outlineTargetWorld.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_outlineTargetNormal.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_inputOutlineWorldTexture.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

}
