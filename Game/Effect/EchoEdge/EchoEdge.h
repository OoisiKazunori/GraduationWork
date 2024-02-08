#pragma once
#include <Helper/Compute.h>
#include "Helper/KazBufferHelper.h"
#include "Math/KazMath.h"
#include <array>
#include"../Game/Echo/EchoArray.h"

class EchoEdge
{
public:
	EchoEdge();
	void Generate(const KazBufferHelper::BufferData&arg_outlineBuffer);
	void Compute();

	const KazBufferHelper::BufferData& GetOutputBuffer()
	{
		return m_outputBuffer;
	}
	ComputeShader m_computeEdge;
private:
	std::vector<KazBufferHelper::BufferData>m_bufferArray;

	KazBufferHelper::BufferData m_outputBuffer;


};

