#include "Outline.h"
#include "Buffer/DescriptorHeapMgr.h"
#include "Buffer/UavViewHandleMgr.h"
#include "../PostEffect/GaussianBlur.h"

PostEffect::Outline::Outline(KazBufferHelper::BufferData arg_outlineTargetWorld, KazBufferHelper::BufferData arg_outlineTargetNormal)
{

	//�A�E�g���C����������Ώۂ̃e�N�X�`����ۑ����Ă����B
	m_outlineTargetWorld = arg_outlineTargetWorld;
	m_outlineTargetNormal = arg_outlineTargetNormal;

	//�A�E�g���C���̐F
	m_outputAlbedoTexture = KazBufferHelper::SetUAVTexBuffer(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_outputAlbedoTexture.bufferWrapper->CreateViewHandle(UavViewHandleMgr::Instance()->GetHandle());
	DescriptorHeapMgr::Instance()->CreateBufferView(
		m_outputAlbedoTexture.bufferWrapper->GetViewHandle(),
		KazBufferHelper::SetUnorderedAccessTextureView(sizeof(DirectX::XMFLOAT4), 1280 * 720),
		m_outputAlbedoTexture.bufferWrapper->GetBuffer().Get()
	);

	//�A�E�g���C���̃G�~�b�V�u
	m_outputEmissiveTexture = KazBufferHelper::SetUAVTexBuffer(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_outputEmissiveTexture.bufferWrapper->CreateViewHandle(UavViewHandleMgr::Instance()->GetHandle());
	DescriptorHeapMgr::Instance()->CreateBufferView(
		m_outputEmissiveTexture.bufferWrapper->GetViewHandle(),
		KazBufferHelper::SetUnorderedAccessTextureView(sizeof(DirectX::XMFLOAT4), 1280 * 720),
		m_outputEmissiveTexture.bufferWrapper->GetBuffer().Get()
	);

	//�A�E�g���C���̐F
	m_outlineData.m_color = KazMath::Vec4<float>(0.84f, 0.93f, 0.95f, 1);
	m_outlineData.m_outlineLength = 300.0f;
	m_outlineColorConstBuffer = KazBufferHelper::SetConstBufferData(sizeof(OutlineData));
	m_outlineColorConstBuffer.bufferWrapper->TransData(&m_outlineData, sizeof(OutlineData));

	//�A�E�g���C���v�Z�p�̃V�F�[�_�[
	{

		std::vector<KazBufferHelper::BufferData>extraBuffer =
		{
			 m_outlineTargetWorld,
			 m_outlineTargetNormal,
			 m_outputAlbedoTexture,
			 m_outputEmissiveTexture,
			 m_outlineColorConstBuffer,
		};
		extraBuffer[0].rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		extraBuffer[0].rootParamType = GRAPHICS_PRAMTYPE_TEX;
		extraBuffer[1].rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		extraBuffer[1].rootParamType = GRAPHICS_PRAMTYPE_TEX2;
		extraBuffer[2].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		extraBuffer[2].rootParamType = GRAPHICS_PRAMTYPE_TEX3;
		extraBuffer[3].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		extraBuffer[3].rootParamType = GRAPHICS_PRAMTYPE_TEX4;
		extraBuffer[4].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
		extraBuffer[4].rootParamType = GRAPHICS_PRAMTYPE_DATA;
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


	m_outlineColorConstBuffer.bufferWrapper->TransData(&m_outlineData, sizeof(OutlineData));
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

}
