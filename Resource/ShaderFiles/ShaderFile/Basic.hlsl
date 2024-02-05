struct OutputData
{
    matrix mat;
    float4 color;
};

cbuffer MatrixBuffer : register(b0)
{
    matrix mat;
}
cbuffer colorBuffer : register(b1)
{
    float4 color;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
};

VSOutput VSmain(float4 pos : POSITION)
{
	VSOutput op;
	op.svpos = mul(mat, pos);
	return op;
}

float4 PSmain(VSOutput input) : SV_TARGET
{
    return color;
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

struct VSOutputWorld
{
    float4 svpos : SV_POSITION;
    float3 worldPos : POSITION;
};

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMat;
    matrix viewProjMat;
}

VSOutputWorld GBufferVSmain(float4 pos : POSITION)
{
	VSOutputWorld op;
	op.svpos = mul(worldMat, pos);
    op.worldPos = op.svpos.xyz;
    op.svpos = mul(viewProjMat, pos);
	return op;
}

struct BasicDrawGBufferOutput
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 metalnessRoughness : SV_TARGET2;
    float4 world : SV_TARGET3;
    float4 emissive : SV_TARGET4;
    float4 outline : SV_TARGET5;
    float4 outlineWorld : SV_TARGET6;
};

BasicDrawGBufferOutput GBufferPSmain(VSOutputWorld input) : SV_TARGET
{
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
            output.albedo = color;
            output.normal = float4(0,0,0,1);
            output.metalnessRoughness = float4(0,0,0,1);
            output.world = float4(0,0,0,1);
            output.emissive = float4(0,0,0,1);
            output.outline = color;
            output.outlineWorld = float4(0,0,0,1);
            return output;
        }
    }
    discard;
    return output;
}