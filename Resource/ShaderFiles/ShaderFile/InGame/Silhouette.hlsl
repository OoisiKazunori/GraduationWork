//入力情報
Texture2D<float4> BaseTex : register(t0);
SamplerState smp : register(s0);

//出力�??UAV  
RWTexture2D<float4> OutputTex : register(u0);

cbuffer OutlineData : register(b0)
{
    int uvTimer;
}

float4 SamplingPixel(Texture2D<float4> arg_texture, uint2 arg_uv)
{
    uint2 uv = arg_uv;
    uv.x -= uint(arg_uv.x / 32) * 32;
    uv.y -= uint(arg_uv.y / 32) * 32;
    return arg_texture[uv].xyzw;
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float2 uv = float2(DTid.xy + float2(0.0f,uvTimer));
    float4 baseTex = SamplingPixel(BaseTex,uv);
    OutputTex[DTid.xy] = baseTex;
}