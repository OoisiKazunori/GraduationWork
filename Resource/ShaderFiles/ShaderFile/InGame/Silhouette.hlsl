//入力情報
Texture2D<float4> BaseTex : register(t0);
SamplerState smp : register(s0);

//出力先UAV  
RWTexture2D<float4> OutputTex : register(u0);

cbuffer OutlineData : register(b0)
{
    int uvTimer;
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //float4 baseTex = BaseTex.Sample(smp,DTid.xy + float2(0.0f,uvTimer));
    //OutputTex[DTid.xy] = baseTex;
}