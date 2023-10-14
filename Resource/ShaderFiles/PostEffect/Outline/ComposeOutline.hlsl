#include "../../Raytracing/RaytracingNoiseHeader.hlsli"

//���͏��
RWTexture2D<float4> Albedo : register(u0);
RWTexture2D<float4> Emissive : register(u1);

//�o�͐�UAV  
RWTexture2D<float4> OutlineAlbedo : register(u2);
RWTexture2D<float4> OutlineEmissive : register(u3);

cbuffer NoiseData : register(b0)
{
    float m_timer;
    float3 m_pad;
}


[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    float2 uv = DTid.xy;
    
    //�m�C�Y���v�Z�B
    int octave = 4;
    float persistance = 1.0f;
    float lacunarity = 0.5f;
    float noiseValue = PerlinNoise2D((uv + float2(m_timer, 0.0f)) * float2(0.5f, 2.5f), octave, persistance, lacunarity, 0.0f);
    
    //�m�C�Y�̒l��0~1����-1~1�ɒ����B
    noiseValue *= 2.0f;
    noiseValue -= 1.0f;
    
    //�X�P�[�����O
    noiseValue *= 10.0f;
    
    //�����UV�ɓK�p
    uv.x += noiseValue;
    
    Albedo[DTid.xy] += OutlineAlbedo[uv];
    Emissive[DTid.xy] += OutlineEmissive[uv];
    
    
    //Albedo[DTid.xy] += float4(noiseValue, noiseValue, noiseValue, 1.0f);
    //Emissive[DTid.xy] += float4(0, 0, 0, 1.0f);
    
}