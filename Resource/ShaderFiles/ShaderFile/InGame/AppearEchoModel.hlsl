#include"BasicDraw.hlsli"
#include"../ModelBuffer.hlsli"
cbuffer MatBuffer : register(b0)
{
    matrix worldMat;
    matrix viewMat;
    matrix projectionMat;
    matrix rotaion;
}

struct EchoData
{
	float3 m_pos;
	float m_radius;
	float m_alpha;
	int m_isActive;
    uint2 pad;
};
RWStructuredBuffer<EchoData> lightBuffer:register(u0);

cbuffer LightBufferB1 : register(b1)
{
    uint lightNum;
}

cbuffer ColorBuffer : register(b2)
{
    float4 color;
}

struct VertexData
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    int4 boneNo : BONE_NO;
    float4 weight : WEIGHT;
};

struct PosUvNormalTangentBinormalOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 worldPos : POSITION;
    float3 tangent : TANGENT2;
    float3 binormal : BINORMAL;
};

//モ�?ルのアニメーション
PosUvNormalTangentBinormalOutput VSDefferdAnimationMain(VertexData input)
{
    float4 resultPos = input.pos;
    PosUvNormalTangentBinormalOutput op;
    op.svpos = mul(worldMat, resultPos);
    op.worldPos = op.svpos.xyz;
    op.svpos = mul(viewMat, op.svpos);
    op.svpos = mul(projectionMat, op.svpos);
    op.uv = input.uv;
    op.normal = input.normal;
    op.binormal = input.binormal;
    op.tangent = input.tangent;
    return op;
}

BasicDrawGBufferOutput PSDefferdAnimationMain(PosUvNormalTangentBinormalOutput input) : SV_TARGET
{
    float4 normalColor = NormalTex.Sample(smp, input.uv);
    //-1.0f ~ 1.0f
    float3 normalVec = 2 * normalColor - 1.0f;
    normalVec = normalize(normalVec);

    float3 normal = mul(rotaion, float4(input.normal, 1.0f));
    normal = normalize(normal);
    float3 tangent = mul(rotaion, float4(input.tangent, 1.0f));
    tangent = normalize(tangent);
    float3 binormal = cross(normal, tangent);

    float3 nWorld = CalucurateTangentToLocal(normalVec, normal, tangent, binormal);
    if (IsEnableToUseMaterialTex(normalColor))
    {
        nWorld = input.normal;
    }

    float4 texColor = AlbedoTex.Sample(smp, input.uv);
    float4 mrColor = MetalnessRoughnessTex.Sample(smp, input.uv);
    
    if (float4(texColor * color).w <= 0.0)
    {
        discard;
    }

    if (IsEnableToUseMaterialTex(mrColor))
    {
        mrColor.xyz = float3(0.0f, 0.0f, 0.0f);
    }

    
    BasicDrawGBufferOutput output;
    for(int i = 0;i < 256; ++i)
    {
        if(lightBuffer[i].m_isActive != 1)
        {
            continue;
        }
        float distanceNum = distance(lightBuffer[i].m_pos,input.worldPos);
        //エコー範囲内なら描画する
        if(distanceNum <= lightBuffer[i].m_radius)
        {
            output.albedo = float4(1,1,1,1) * color;
            output.normal = float4(normal, 1.0f);
            output.metalnessRoughness = float4(0, 0, 0, 1);
            output.world = float4(input.worldPos, 1.0f);
            output.emissive = EmissiveTex.Sample(smp, input.uv);
            output.outline = float4(0,0,0,1);
            output.outlineWorld = float4(0,0,0,1);
            return output;
        }
    }
    discard;
    return output;
}
