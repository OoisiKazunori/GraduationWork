#include "../../Raytracing/RaytracingNoiseHeader.hlsli"

//���͏��
RWTexture2D<float4> Albedo : register(u0);
RWTexture2D<float4> Emissive : register(u1);

//�o�͐�UAV  
RWTexture2D<float4> OutlineAlbedo : register(u2);
RWTexture2D<float4> OutlineEmissive : register(u3);

cbuffer NoiseData : register(b0)
{
    float4 m_noiseHorizontalLine;
    float m_timer;
}

//�߂��ɉ�������������m�C�Y�𑝂₷�B
void CheckHorizonalLine(float arg_horizonaLine, float arg_uv, float arg_length, float arg_power, inout float arg_noizeValue)
{
    
    //�����𑪂�B
    float distance = abs(arg_uv - arg_horizonaLine);
        
    //����Ă������΂��B
    if (arg_length < distance)
        return;
        
    //�����̃p�[�Z���e�[�W�����߂�B
    float distanceRate = 1.0f - distance / arg_length;
        
    //�C�[�W���O��������B
    distanceRate = distanceRate * distanceRate;
        
    //�m�C�Y���X�P�[�����O����B
    arg_noizeValue *= distanceRate * arg_power;
    
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
    
    //�����̋߂��ł͂���ɃX�P�[�����O����B
    const float HORIZONAL_SCALE_LENGTH = 70.0f;
    const float HORIZONAL_SCALE_POWER = 4.0f;
    float3 color = float3(0, 0, 0);
    CheckHorizonalLine(m_noiseHorizontalLine.x, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);
    CheckHorizonalLine(m_noiseHorizontalLine.y, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);
    CheckHorizonalLine(m_noiseHorizontalLine.z, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);
    CheckHorizonalLine(m_noiseHorizontalLine.w, uv.y, HORIZONAL_SCALE_LENGTH, HORIZONAL_SCALE_POWER, noiseValue);

    
    //�����UV�ɓK�p
    uv.x += noiseValue;
    
    Albedo[DTid.xy] += OutlineAlbedo[uv];
    Emissive[DTid.xy] += OutlineEmissive[uv];
    
    //Albedo[DTid.xy] += float4(noiseValue, noiseValue, noiseValue, 1.0f);
    //Emissive[DTid.xy] += float4(0, 0, 0, 1.0f);
    
}