#include "../../Raytracing/RaytracingNoiseHeader.hlsli"

//入力情報
RWTexture2D<float4> Albedo : register(u0);
RWTexture2D<float4> Emissive : register(u1);

//出力先UAV  
RWTexture2D<float4> OutlineAlbedo : register(u2);
RWTexture2D<float4> OutlineEmissive : register(u3);

cbuffer NoiseData : register(b0)
{
    float4 m_noiseHorizontalLine;
    float m_timer;
}

//近くに横線があったらノイズを増やす。
void CheckHorizonalLine(float arg_horizonaLine, float arg_uv, float arg_length, float arg_power, inout float arg_noizeValue)
{
    
    //距離を測る。
    float distance = abs(arg_uv - arg_horizonaLine);
        
    //離れていたら飛ばす。
    if (arg_length < distance)
        return;
        
    //距離のパーセンテージを求める。
    float distanceRate = 1.0f - distance / arg_length;
        
    //イージングをかける。
    distanceRate = distanceRate * distanceRate;
        
    //ノイズをスケーリングする。
    arg_noizeValue *= distanceRate * arg_power;
    
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    float2 uv = DTid.xy;
    
    //ノイズを計算。
    int octave = 4;
    float persistance = 1.0f;
    float lacunarity = 0.5f;
    float noiseValue = PerlinNoise2D((uv + float2(m_timer, 0.0f)) * float2(0.5f, 2.5f), octave, persistance, lacunarity, 0.0f);
    
    //ノイズの値を0~1から-1~1に調整。
    noiseValue *= 2.0f;
    noiseValue -= 1.0f;
    
    //スケーリング
    noiseValue *= 10.0f;
    
    //横線の近くではさらにスケーリングする。
    const float HORIZONAL_SCALE_LENGTH = 70.0f;
    const float HORIZONAL_SCALE_POWER = 4.0f;
    float3 color = float3(0, 0, 0);
    CheckHorizonalLine(m_noiseHorizontalLine.x, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);
    CheckHorizonalLine(m_noiseHorizontalLine.y, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);
    CheckHorizonalLine(m_noiseHorizontalLine.z, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);
    CheckHorizonalLine(m_noiseHorizontalLine.w, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);

    
    //それをUVに適用
    uv.x += noiseValue;
    
    Albedo[DTid.xy] += OutlineAlbedo[uv];
    Emissive[DTid.xy] += OutlineEmissive[uv];
    
    //Albedo[DTid.xy] += float4(noiseValue, noiseValue, noiseValue, 1.0f);
    //Emissive[DTid.xy] += float4(0, 0, 0, 1.0f);
    
}