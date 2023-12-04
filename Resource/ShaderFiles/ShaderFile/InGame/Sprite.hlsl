#include"BasicDraw.hlsli"


struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};

cbuffer MatBuffer : register(b0)
{
    matrix mat; //3D�ϊ��s��
}

cbuffer ColorBuffer : register(b1)
{
    float4 color; //3D�ϊ��s��
}

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput op;
	op.svpos = mul(mat, pos);
	op.uv = uv;
	return op;
}

Texture2D<float4> tex : register(t0);
SamplerState smp1 :register(s0);

//マルチパスレンダリング
BasicDrawGBufferOutput PSmain(VSOutput input) : SV_TARGET
{
    float4 textureColor = float4(tex.Sample(smp1, input.uv));    
    if (textureColor.w * color.w < 0.01f)
    {
        discard;
    }

    BasicDrawGBufferOutput output;
    output.albedo = textureColor * color;
    output.normal = float4(0,0,0,1);
    output.metalnessRoughness = float4(0,0,0,1);
    output.emissive = float4(0,0,0,1);
    output.world = float4(0,0,0,1);
    output.outline = float4(0,0,0,1);
    output.outlineWorld = float4(-1,-1,-1,1);
    return output;
}