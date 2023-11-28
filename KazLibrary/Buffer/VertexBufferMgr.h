#pragma once
#include"Polygon.h"
#include"../KazLibrary/Helper/ISinglton.h"

struct IAPolygonData
{
	std::shared_ptr<KazBufferHelper::BufferData> m_buffer;
	IAPolygonData()
	{};
	IAPolygonData(const KazBufferHelper::BufferData& arg_vertexBufferGennerateData) :
		m_buffer(std::make_shared<KazBufferHelper::BufferData>(arg_vertexBufferGennerateData))
	{};

	void Generate(const KazBufferHelper::BufferData& arg_vertexBufferGennerateData)
	{
		m_buffer = std::make_shared<KazBufferHelper::BufferData>(arg_vertexBufferGennerateData);
	};
};

struct PolygonGenerateData
{
	void* m_ptr;
	int m_structureSize;
	size_t m_arraySize;
};

struct IAPolygonBufferData
{
	IAPolygonData m_cpuBuffer, m_gpuBuffer;

	IAPolygonBufferData(const PolygonGenerateData& arg_vertex) :
		m_cpuBuffer(
			KazBufferHelper::SetVertexBufferData(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(arg_vertex.m_arraySize, arg_vertex.m_structureSize))
		),
		m_gpuBuffer(
			KazBufferHelper::SetGPUBufferData(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(arg_vertex.m_arraySize, arg_vertex.m_structureSize))
		)
	{
		m_cpuBuffer.m_buffer->bufferWrapper->TransData(arg_vertex.m_ptr, KazBufferHelper::GetBufferSize<BUFFER_SIZE>(arg_vertex.m_arraySize, arg_vertex.m_structureSize));
		m_gpuBuffer.m_buffer->bufferWrapper->CopyBuffer(m_cpuBuffer.m_buffer->bufferWrapper->GetBuffer());
	};
	IAPolygonBufferData()
	{};

	void GenerateBuffer(const PolygonGenerateData& arg_vertex)
	{
		m_gpuBuffer.Generate(KazBufferHelper::SetGPUBufferData(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(arg_vertex.m_arraySize, arg_vertex.m_structureSize)));
		m_cpuBuffer.Generate(KazBufferHelper::SetVertexBufferData(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(arg_vertex.m_arraySize, arg_vertex.m_structureSize)));

		m_cpuBuffer.m_buffer->bufferWrapper->TransData(arg_vertex.m_ptr, KazBufferHelper::GetBufferSize<BUFFER_SIZE>(arg_vertex.m_arraySize, arg_vertex.m_structureSize));
		m_gpuBuffer.m_buffer->bufferWrapper->CopyBuffer(m_cpuBuffer.m_buffer->bufferWrapper->GetBuffer());
	};
};

/// <summary>
/// ���_���
/// </summary>
struct VertexAndIndexGenerateData
{
	void* verticesPos;
	int structureSize;
	size_t arraySize;
	int oneArraySize;
	std::vector<UINT>indices;

	VertexAndIndexGenerateData(void* VERT_PTR, int STRUCTURE_SIZE, size_t ARRAY_SIZE, int ONE_ARRAY_SIZE, std::vector<UINT>INDICES) :
		verticesPos(VERT_PTR), structureSize(STRUCTURE_SIZE), arraySize(ARRAY_SIZE), oneArraySize(ONE_ARRAY_SIZE), indices(INDICES)
	{};
};

struct VertexGenerateData
{
	void* verticesPos;
	int structureSize;
	size_t arraySize;
	int oneArraySize;

	VertexGenerateData(void* VERT_PTR, int STRUCTURE_SIZE, size_t ARRAY_SIZE, int ONE_ARRAY_SIZE) :
		verticesPos(VERT_PTR), structureSize(STRUCTURE_SIZE), arraySize(ARRAY_SIZE), oneArraySize(ONE_ARRAY_SIZE)
	{};
};

/// <summary>
/// ���X�^���C�U�`��p�̒��_�o�b�t�@�Ǘ�
/// ��{�g����
/// </summary>
class VertexBufferMgr :public ISingleton<VertexBufferMgr>
{
public:
	RESOURCE_HANDLE GenerateBuffer(const VertexGenerateData& arg_vertexData, bool arg_generateInVRAMFlag);


	RESOURCE_HANDLE GenerateBuffer(const std::vector<VertexAndIndexGenerateData>& vertexData, bool arg_generateInVRAMFlag = true);
	RESOURCE_HANDLE GenerateBuffer2(const VertexGenerateData& arg_vertexData, bool arg_generateInVRAMFlag);


	RESOURCE_HANDLE GeneratePlaneBuffer();
	RESOURCE_HANDLE GenerateBoxBuffer();
	RESOURCE_HANDLE StackVertexBuffer(const std::shared_ptr<KazBufferHelper::BufferData>& arg_vertexBuffer,
		const std::shared_ptr<KazBufferHelper::BufferData>& arg_indexBuffer);

	void ReleaseVeretexIndexBuffer(RESOURCE_HANDLE HANDLE);
	void ReleaseVeretexBuffer(RESOURCE_HANDLE HANDLE);
	PolygonMultiMeshedIndexData GetVertexIndexBuffer(RESOURCE_HANDLE HANDLE);
	PolygonInstanceData GetVertexBuffer(RESOURCE_HANDLE HANDLE);

	RESOURCE_HANDLE GetPlaneHandle()
	{
		return m_planeHandle;
	};
	void GenerateBasicVertex()
	{
		m_planeHandle = VertexBufferMgr::Instance()->GeneratePlaneBuffer();
	}
private:
	HandleMaker m_handle;
	HandleMaker m_vertexHandle;
	std::vector<std::vector<std::unique_ptr<PolygonBuffer>>>m_vertexBufferArray;
	//�C���f�b�N�X����------
	std::vector<PolygonMultiMeshedIndexData>m_drawIndexDataArray;
	std::vector<std::vector<std::vector<IAPolygonBufferData>>>m_polygonIndexBufferArray;
	//�C���f�b�N�X����------
	std::vector<IAPolygonBufferData>m_polygonBufferArray;
	std::vector<PolygonInstanceData>m_drawDataArray;
	int sDescHandle = 0;

	std::vector<DirectX::XMFLOAT3>GetPlaneVertices(const KazMath::Vec2<float>& anchorPoint, const KazMath::Vec2<float>& scale, const KazMath::Vec2<int>& texSize);


	RESOURCE_HANDLE m_planeHandle;

	void GenerateIABuffer(IAPolygonBufferData* arg_bufferData, const PolygonGenerateData& arg_generateBufferData);
	void GenerateBufferView(const D3D12_SHADER_RESOURCE_VIEW_DESC& arg_view);

	void SetInDescriptorHeap(const std::shared_ptr<KazBufferHelper::BufferData>&arg_bufferData,const D3D12_SHADER_RESOURCE_VIEW_DESC&arg_viewDesc);
	D3D12_SHADER_RESOURCE_VIEW_DESC SetView(UINT arg_structureByte, UINT arg_numElement);

	KazRenderHelper::DrawIndexedInstancedData SetIndexInstanceData(size_t arg_indexArrayNum);
};
