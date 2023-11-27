#include "GraphicsDepthTest.h"

#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"

GraphicsDepthTest::GraphicsDepthTest()
{
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; //デプスステンシルビュー

	DirectX12Device::Instance()->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
}

GraphicsDepthTest::~GraphicsDepthTest()
{
}

RESOURCE_HANDLE GraphicsDepthTest::CreateBuffer()
{
	RESOURCE_HANDLE handleNum = handle.GetHandle();
	dsvH.push_back({});

	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		WIN_X, WIN_Y,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CD3DX12_HEAP_PROPERTIES propertices(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.0f, 0);

	depthBuffer = KazBufferHelper::BufferResourceData(
		propertices,
		D3D12_HEAP_FLAG_NONE,
		depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		"GraphicsDepthTest"
	);

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectX12Device::Instance()->dev->CreateDepthStencilView(
		depthBuffer.bufferWrapper->GetBuffer().Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	dsvH[handleNum] = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dsvHeap->SetName(L"GraphicsDepthTest");

	//深度ステンシルバッファをテクスチャとして描画する用のバッファ
	//テクスチャとして描画する際はこちらのバッファを使用しています。
	{
		CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
			WIN_X,
			WIN_Y,
			1,
			0
		);
		depthBufferTex = KazBufferHelper::BufferResourceData(
			propertices,
			D3D12_HEAP_FLAG_NONE,
			texDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			"GraphicsDepthTestTex"
		);

		//深度ステンシルバッファをテクスチャにコピー
		WriteInTex();

		//SRVのビュー生成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		DescriptorHeapMgr::Instance()->CreateBufferView(
			DescriptorHeapMgr::Instance()->GetSize(BufferMemory::DESCRIPTORHEAP_MEMORY_SRV).endSize - 1,
			srvDesc,
			depthBufferTex.bufferWrapper->GetBuffer().Get()
		);
		depthBufferTex.rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		depthBufferTex.bufferWrapper->CreateViewHandle(DescriptorHeapMgr::Instance()->GetSize(BufferMemory::DESCRIPTORHEAP_MEMORY_SRV).endSize - 1);

	}

	return handleNum;
}

void GraphicsDepthTest::Clear(RESOURCE_HANDLE HANDLE)
{
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	DirectX12CmdList::Instance()->cmdList->ClearDepthStencilView(dsvH[HANDLE], D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0, 0, 0, nullptr);
}

void GraphicsDepthTest::WriteInTex()
{
	depthBuffer.bufferWrapper->ChangeBarrier(D3D12_RESOURCE_STATE_COPY_SOURCE);
	depthBufferTex.bufferWrapper->CopyBuffer(depthBuffer.bufferWrapper->GetBuffer());
	depthBuffer.bufferWrapper->ChangeBarrier(D3D12_RESOURCE_STATE_DEPTH_WRITE);
}
