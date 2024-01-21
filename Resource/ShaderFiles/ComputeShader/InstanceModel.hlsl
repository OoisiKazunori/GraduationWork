#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

cbuffer RootConstants : register(b0)
{    
    matrix viewProjection;
};
RWStructuredBuffer<matrix> worldMatData : register(u0);
RWStructuredBuffer<matrix> drawData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;
    drawData[index] = mul(viewProjection,worldMatData[index]);
}
