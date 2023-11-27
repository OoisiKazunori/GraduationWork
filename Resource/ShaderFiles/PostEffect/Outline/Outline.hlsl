//���͏��
Texture2D<float4> TargetWorld : register(t0);
Texture2D<float4> TargetNormal : register(t1);
Texture2D<float4> SilhouetteWorld : register(t2);

//�o�͐�UAV  
RWTexture2D<float4> OutputAlbedo : register(u0);
RWTexture2D<float4> OutputEmissive : register(u1);
RWTexture2D<float4> SilhouetteTex : register(u2);

cbuffer OutlineData : register(b0)
{
    float4 m_outlineColor;
    float3 m_outlineCenterPos;
    float m_outlineLength;
}

cbuffer EchoData : register(b1)
{
    float3 m_color;
    float m_echoAlpha;
    float3 m_center;
    float m_echoRadius;
}

cbuffer EyePos : register(b2)
{
    matrix m_viewMat;
    matrix m_projMat;
    float3 m_eyePos;
    float m_noiseTimer;
};

float4 SamplingPixel(Texture2D<float4> arg_texture, uint2 arg_uv)
{
    return arg_texture[uint2(clamp(arg_uv.x, 0, 1280), clamp(arg_uv.y, 0, 720))].xyzw;
}

bool CheckOutline(uint2 arg_uv, float4 arg_baseNormal, float4 arg_baseWorld, inout float3 arg_sampleWorldPos)
{
    
    //�ǂꂭ�炢���[���h���W������Ă����炻���ɃA�E�g���C�����������ނ��B
    float outlineDistanceDeadline = 2.0f;
    //�@���̓��ς̍������ǂꂭ�炢��������A�E�g���C�����������ނ��B
    float outlineNormalDeadline = 0.8f;
    
    //�e�����T���v�����O����B
    float4 sampleNormal = SamplingPixel(TargetNormal, arg_uv);
    float4 sampleWorld = SamplingPixel(TargetWorld, arg_uv);
    
    //Base�̖@�������̃s�N�Z���̖@�����������܂�Ă��Ȃ��������΂��B
    if (length(arg_baseNormal.xyz) <= 0.1f && length(sampleNormal.xyz) < 0.1f)
    {
        return false;
    }
    //���[���h��񂪏������܂�Ă��Ȃ�������A�E�g�B
    if (length(sampleWorld.xyz) <= 0.1f)
    {
        return false;
    }
    
    //�@���̈Ⴂ���`�F�b�N
    bool isCheckNormal = dot(sampleNormal.xyz, arg_baseNormal.xyz) < outlineNormalDeadline;
    //���[���h���W�ɂ��Ⴂ���`�F�b�N
    bool isCheckWorld = outlineDistanceDeadline < length(sampleWorld.xyz - arg_baseWorld.xyz);
    //���̃A�E�g���C���΍�B
    if (0.9f < sampleNormal.y && 0.9f < arg_baseNormal.y)
    {
        
        if (1.0f < length(sampleWorld.y - arg_baseWorld.y))
        {
            
            isCheckWorld = true;
            
        }
        
    }
    
    if (isCheckNormal || isCheckWorld)
    {
        
        if (0.1f < length(sampleWorld.xyz))
        {
            arg_sampleWorldPos = sampleWorld.xyz;
        }
        
        return true;
        
    }
    
    return false;
    
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    //�A�E�g���C���̌���
    const int OUTLINE_THICKNESS = 1;
    
    //���̃s�N�Z���ɃA�E�g���C�����������H
    bool isOutline = false;
    
    //���̃s�N�Z���̐F���
    float4 baseNormal = SamplingPixel(TargetNormal, DTid.xy);
    float4 baseWorld = SamplingPixel(TargetWorld, DTid.xy);
    
    //�T���v�����O�������[���h���W
    float3 sampleWorldPos = float3(0.0f, 0.0f, 0.0f);
    
    ////�E�����`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, 0), baseNormal, baseWorld, sampleWorldPos);
    
    ////�������`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, 0), baseNormal, baseWorld, sampleWorldPos);
    
    ////�㑤���`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(0, -OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    ////�������`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(0, OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    ////�E�����`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    ////������`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    ////�������`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    ////�E����`�F�b�N
    //isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    
    //���͂̃s�N�Z���̖@���̒l�̕��ς��v�Z����B
    float3 normal = float3(0, 0, 0);
    normal = SamplingPixel(TargetNormal, DTid.xy).xyz * -8.0f;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(OUTLINE_THICKNESS, 0)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(-OUTLINE_THICKNESS, 0)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(0, -OUTLINE_THICKNESS)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(0, OUTLINE_THICKNESS)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(OUTLINE_THICKNESS, OUTLINE_THICKNESS)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(-OUTLINE_THICKNESS, OUTLINE_THICKNESS)).xyz;
    normal += SamplingPixel(TargetNormal, DTid.xy + uint2(OUTLINE_THICKNESS, -OUTLINE_THICKNESS)).xyz;

	//���͂̃s�N�Z���̐[�x�l�̕��ς��v�Z����B
    float depth = length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(OUTLINE_THICKNESS, 0)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(-OUTLINE_THICKNESS, 0)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(0, -OUTLINE_THICKNESS)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(0, OUTLINE_THICKNESS)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(OUTLINE_THICKNESS, OUTLINE_THICKNESS)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(-OUTLINE_THICKNESS, OUTLINE_THICKNESS)).xyz);
    depth += length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy + uint2(OUTLINE_THICKNESS, -OUTLINE_THICKNESS)).xyz);
    depth /= 8.0f;
    
    if (length(normal) >= 0.2f || abs(depth - length(m_outlineCenterPos - SamplingPixel(TargetWorld, DTid.xy).xyz)) > 0.4f)
    {
        isOutline = true;
    }
    else
    {
        isOutline = false;
    }
    
    

    //�V���G�b�g�p�̃}�X�N�Ȃ�A�E�g���C����`�悵�Ȃ�
    float4 silleout = SamplingPixel(SilhouetteWorld, DTid.xy);
    //������胏�[���h���W���傫����Δ���Ă���Ɣ��������ď�������
    bool silleoutFlag = false;
    float dis = abs(distance(m_eyePos, baseWorld.xyz));
    if (dis < silleout.r)
    {
        isOutline = false;
        silleoutFlag = true;
    }
    
    if (isOutline)
    {
        //�T���v�����O�����ʒu�ɉ����ĐF���Â�����B
        float edgeDistance = 1.0f - clamp(length(sampleWorldPos - m_outlineCenterPos) / m_outlineLength, 0.0f, 0.8f);
        
        OutputAlbedo[DTid.xy] += m_outlineColor * edgeDistance;
        OutputEmissive[DTid.xy] += m_outlineColor * edgeDistance;
    }
    else
    {
        if (silleoutFlag)
        {
            float4 silhouetteTexture = SilhouetteTex[DTid.xy];
            OutputAlbedo[DTid.xy] = silhouetteTexture;
            OutputEmissive[DTid.xy] = silhouetteTexture;
        }
        else
        {
            OutputAlbedo[DTid.xy] += float4(0, 0, 0, 0);
            OutputEmissive[DTid.xy] += float4(0, 0, 0, 0);
        }
    }
    
    ////���S�n�_������̋�����������
    //const float GRID_SCOPE = 100.0f;
    //const int GRID_RANGE = 15.0f;
    //float baseWorldDistance = length(baseWorld.xyz - m_outlineCenterPos);
    //if (baseWorldDistance <= GRID_SCOPE && 0.0f < length(baseWorld.xyz))
    //{
        
    //    //���[���h���W�̒l��GRID_RANGE�Ŋ���؂��l�ɋ߂�������
    //    bool isGrid = frac(baseWorld.x / GRID_RANGE) < 0.01f;
    //    isGrid |= baseNormal.y < 0.9f && (frac(baseWorld.y / GRID_RANGE) < 0.01f);
    //    isGrid |= frac(baseWorld.z / GRID_RANGE) < 0.01f;
        
    //    //���������߂�B
    //    float distanceRate = 1.0f - baseWorldDistance / GRID_SCOPE;
    //    distanceRate *= 0.3f;
        
    //    if (isGrid)
    //    {
            
    //        OutputAlbedo[DTid.xy] += m_outlineColor * distanceRate;
    //        OutputEmissive[DTid.xy] += m_outlineColor * distanceRate;

    //    }
        
    //}
    
    //�G�R�[��`��
    if (length(baseWorld.xyz - m_center) <= m_echoRadius && 0.0f < length(baseWorld.xyz))
    {
        //�ɍׂ̃O���b�h���o���B
        const float ECHO_GRID_RANGE = 2.0f;
        bool isGrid = frac(baseWorld.x / ECHO_GRID_RANGE) < 0.9f;
        isGrid |= baseNormal.y < 0.9f && (frac(baseWorld.y / ECHO_GRID_RANGE) < 0.9f);
        isGrid |= frac(baseWorld.z / ECHO_GRID_RANGE) < 0.9f;
        
        if (!isGrid)
        {
            
            OutputAlbedo[DTid.xy] += float4(m_color * m_echoAlpha, 1.0f);
            OutputEmissive[DTid.xy] += float4(m_color * m_echoAlpha, 1.0f);

        }
        
        OutputAlbedo[DTid.xy] += float4(m_color * m_echoAlpha, 1.0f);
    }

}