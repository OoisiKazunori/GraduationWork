struct ColorOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;    
};

cbuffer MatBuffer : register(b0)
{
    matrix mat;
}

ColorOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    ColorOutput op;
    op.svpos = mul(mat,pos);
    op.uv = uv;
    return op;
}
//ディファーレンダリングの結果
RWTexture2D<float4> GBuffer : register(u0);
SamplerState smp : register(s0);

cbuffer Range : register(b1)
{
    float rate;
}

float4 PSmain(ColorOutput input) : SV_TARGET
{
    float4 output = GBuffer[input.uv * uint2(1280,720)];

    if(input.uv.x - 0.001f <= rate && rate <= input.uv.x + 0.001f)
    {
        //中間の線
        return float4(1.0f,1.0f,1.0f,1.0f);
    }
    else if(input.uv.x < rate)
    {
        //ディファーレンダリングの描画
        return output;
    }
    else
    {
        //アンチエイリアスの描画
        output = float4(0,0,0,1);
        return output;
    }
}