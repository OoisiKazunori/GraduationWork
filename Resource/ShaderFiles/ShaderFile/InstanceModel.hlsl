#include"ModelBuffer.hlsli"
#include"InGame/BasicDraw.hlsli"

struct OutputData
{
    matrix mat;
    float4 color;
};
RWStructuredBuffer<OutputData>matrixBuffer:register(u0);

struct PosUvNormalTangentBinormalOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 worldPos : POSITION;
    float3 tangent : TANGENT2;
    float3 binormal : BINORMAL;
    uint id :SV_InstanceID;
};

//ディファードレンダリング対応
PosUvNormalTangentBinormalOutput VSDefferdMain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float3 tangent : TANGENT, float3 binormal : BINORMAL,uint id : SV_InstanceID)
{
    PosUvNormalTangentBinormalOutput op;
    op.svpos = mul(matrixBuffer[id].mat, pos);
    op.worldPos = float3(0,0,0);
    op.uv = uv;
    op.normal = normal;
    op.binormal = binormal;
    op.tangent = tangent;
    op.id = id;
    return op;
}

//ディファードレンダリング対応
BasicDrawGBufferOutput PSDefferdMain(PosUvNormalTangentBinormalOutput input) : SV_TARGET
{
    float4 normalColor = NormalTex.Sample(smp, input.uv);
    //-1.0f ~ 1.0f
    float3 normalVec = 2 * normalColor - 1.0f;
    normalVec = normalize(normalVec);

    float4 texColor = AlbedoTex.Sample(smp, input.uv);
    float4 mrColor = MetalnessRoughnessTex.Sample(smp, input.uv);

    if (IsEnableToUseMaterialTex(mrColor))
    {
        mrColor.xyz = float3(0.0f, 0.0f, 0.0f);
    }

    BasicDrawGBufferOutput output;
    output.albedo = texColor * matrixBuffer[input.id].color;
    output.normal = float4(0,0,0,1);
    output.metalnessRoughness = float4(mrColor.xyz, raytracingId);
    output.world = float4(input.worldPos, 1.0f);
    output.emissive = EmissiveTex.Sample(smp, input.uv);
    output.outline = float4(0,0,0,0);
    output.outlineWorld = float4(0,0,0,0);
    return output;
}
