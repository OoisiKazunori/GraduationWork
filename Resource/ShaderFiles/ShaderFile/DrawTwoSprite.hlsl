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

float GetBright(float4 color)
{
    return dot(color,float4(0.299f,0.587f,0.114f,1.0f));
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
        float bright = GetBright(output);

        float3x3 xEdge;
        xEdge[0] = float3(-1,0,1);
        xEdge[1] = float3(-2,0,2);
        xEdge[2] = float3(-1,0,1);
        float3x3 yEdge;
        yEdge[0] = float3(-1,-2,-1);
        yEdge[1] = float3(0,0,0);
        yEdge[2] = float3(1,2,1);
        float3x3 target;
        target[0] = float3(
            GetBright(GBuffer[input.uv * float2(1280 - 1,720 - 1)]),
            GetBright(GBuffer[input.uv * float2(1280,    720 - 1)]),
            GetBright(GBuffer[input.uv * float2(1280 + 1,720 - 1)])
            );
        target[1] = float3(
            GetBright(GBuffer[input.uv * float2(1280 - 1,720)]),
            GetBright(GBuffer[input.uv * float2(1280,    720)]),
            GetBright(GBuffer[input.uv * float2(1280 + 1,720)])
        );
        target[2] = float3(
            GetBright(GBuffer[input.uv * float2(1280 - 1,720 + 1)]),
            GetBright(GBuffer[input.uv * float2(1280,    720 + 1)]),
            GetBright(GBuffer[input.uv * float2(1280 + 1,720 + 1)])
        );

        float3x3 result = mul(xEdge,target) + mul(yEdge,target);
        
        //アンチエイリアスの描画
        output = float4(result[1][1],result[1][1],result[1][1],1);
        return output;
    }
}