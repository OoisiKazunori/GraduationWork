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
//�?ィファーレンダリングの結果
RWTexture2D<float4> GBuffer : register(u0);
SamplerState smp : register(s0);

cbuffer Range : register(b1)
{
    float rate;
    float treshold;
    float minTreshold;
}

float GetBright(float4 color)
{
    float4 output = color * float4(0.299f,0.587f,0.114f,1.0f);
    return output.x + output.y + output.z;
}

float4 PSmain(ColorOutput input) : SV_TARGET
{
    float4 output = GBuffer[input.uv * uint2(1280,720)];

    if(input.uv.x - 0.001f <= rate && rate <= input.uv.x + 0.001f)
    {
        //中間�?��?
        return float4(1.0f,1.0f,1.0f,1.0f);
    }
    else if(input.uv.x < rate)
    {
        //�?ィファーレンダリングの描画
        return output;
    }
    else
    {
        float nw = GetBright(GBuffer[input.uv * float2(1280.0f - 1.0f,720.0f - 1.0f)]);
        float ne = GetBright(GBuffer[input.uv * float2(1280.0f + 1.0f,720.0f - 1.0f)]);
        float sw = GetBright(GBuffer[input.uv * float2(1280.0f - 1.0f,720.0f + 1.0f)]);
        float se = GetBright(GBuffer[input.uv * float2(1280.0f + 1.0f,720.0f + 1.0f)]);
        float m  = GetBright(GBuffer[input.uv * float2(1280.0f,720.0f)]);
        
        //輝度の最大値--------------------------------
        float luma1 = max(
            nw,
            ne
        );
        float luma2 = max(
            sw,
            se
        );
        float maxLuma = max(luma1,luma2);
        //輝度の最大値--------------------------------

        //コントラストの差分--------------------------------
        maxLuma = max(maxLuma,m);

        float lumaMin1 = min(
            nw,
            ne
        );
        float lumaMin2 = min(
            sw,
            se
        );
        float minLuma = min(lumaMin1,lumaMin2);
        minLuma = max(minLuma,m);

        float contrast = maxLuma - minLuma;
        //コントラストの差分--------------------------------

        //AA検知--------------------------------
        if(max(minTreshold,maxLuma * treshold) <= contrast)
        {
            output = float4(1,1,1,1);
        }
        else
        {
            output = float4(0,0,0,1);
        }        
        //AA検知--------------------------------
        
        return output;
    }
}