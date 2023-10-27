#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"

//àÍå¬ÇµÇ©ê∂ê¨Ç≈Ç´Ç»Ç¢ÇÃÇ≈íçà”
class GraphicsDepthTest {
public:
	GraphicsDepthTest();
	~GraphicsDepthTest();

	RESOURCE_HANDLE CreateBuffer();
	void Clear(RESOURCE_HANDLE HANDLE);

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> dsvH;

	KazBufferHelper::BufferData depthBuffer;
	KazBufferHelper::BufferData depthBufferTex;
	void WriteInTex();
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	HandleMaker handle;
};

