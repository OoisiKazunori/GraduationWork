#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

cbuffer RootConstants : register(b0)
{    
    matrix view;
    matrix proj;
};
struct OutputData
{
    matrix mat;
    float4 color;
};
RWStructuredBuffer<matrix> worldMatData : register(u0);
RWStructuredBuffer<float4> colorData : register(u1);
RWStructuredBuffer<OutputData> drawData : register(u2);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;
    OutputData output;
    output.mat = mul(view,worldMatData[index]);
    output.mat = mul(proj,output.mat);
    output.color = colorData[index];
    drawData[index] = output;
}
