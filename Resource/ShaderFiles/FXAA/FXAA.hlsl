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
    float treshold;
    float minTreshold;
    float blur;
    int isEdge;
}

static const float4 LUMA = float4(0.299f,0.587f,0.114f,1.0f);

float GetBright(float4 color)
{
    return dot(color.xyz,LUMA.xyz);
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
        //FXAAの描画処理

        const float2 inputUV = input.uv * float2(1280.0f,720.0f);
        float nw = GetBright(GBuffer[inputUV + float2(-1.0f,-1.0f)]);
        float ne = GetBright(GBuffer[inputUV + float2( 1.0f,-1.0f)]);
        float sw = GetBright(GBuffer[inputUV + float2(-1.0f, 1.0f)]);
        float se = GetBright(GBuffer[inputUV + float2( 1.0f, 1.0f)]);
        float m  = GetBright(GBuffer[inputUV]);
        
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
        if(max(minTreshold,maxLuma * treshold) < contrast)
        {
            output = float4(1,1,1,1);
        }
        else
        {
            //AA必要なし
            if(isEdge)
            {
                output = float4(0,0,0,1);
            }
            return output;
        }        
        //AA検知--------------------------------

        //AA実行--------------------------------

        //ベクトル算出
        float2 dir;
        dir.x = -((nw + ne)-(sw + se));
        dir.y = -((nw + sw)-(ne + se));
        dir.xy = normalize(dir.xy) * clamp(blur,0.0f,1.0f);

        //最終ピクセル算出
        const float2 rgbAOffset = float2(1.0f / 3.0f - 0.5f,2.0f / 3.0f - 0.5f);
        float2 getUV = input.uv + dir * float2(rgbAOffset.x,rgbAOffset.x);
        float2 getUV2 = input.uv + dir * float2(rgbAOffset.y,rgbAOffset.y);
        getUV = clamp(getUV,0.0f,1.0f);
        getUV2 = clamp(getUV2,0.0f,1.0f);
        float3 rgbA = (1.0f/2.0f) * (
            GBuffer[float2(1280.0f,720.0f) * getUV].xyz +
            GBuffer[float2(1280.0f,720.0f) * getUV2].xyz);

        const float2 rgbBOffset = float2(0.0f / 3.0f - 0.5f,3.0f / 3.0f - 0.5f);
        getUV = input.uv + dir * float2(rgbBOffset.x,rgbBOffset.x);
        getUV2 = input.uv + dir * float2(rgbBOffset.y,rgbBOffset.y);
        getUV = clamp(getUV,0.0f,1.0f);
        getUV2 = clamp(getUV2,0.0f,1.0f);
        float3 rgbB = rgbA * (1.0f/2.0f) + (1.0f/4.0f) * (
            GBuffer[float2(1280.0f,720.0f) * getUV].xyz +
            GBuffer[float2(1280.0f,720.0f) * getUV2].xyz);


        float lumaB = GetBright(float4(rgbB,1.0));
        if((lumaB < minLuma) || (maxLuma < lumaB))
        {
            return float4(rgbA.xyz,1.0f);
        }
        else
        {
            return float4(rgbB.xyz,1.0f);
        }
    }
}