//���͏��
Texture2D<float4> TargetWorld : register(t0);
Texture2D<float4> TargetNormal : register(t1);

//�o�͐�UAV  
RWTexture2D<float4> OutputAlbedo : register(u0);
RWTexture2D<float4> OutputEmissive : register(u1);

cbuffer OutlineData : register(b0)
{
    float4 m_outlineColor;
    float3 m_outlineCenterPos;
    float m_outlineLength;
}

float4 SamplingPixel(Texture2D<float4> arg_texture, uint2 arg_uv)
{
    return arg_texture[uint2(clamp(arg_uv.x, 0, 1280), clamp(arg_uv.y, 0, 720))].xyzw;
}

bool CheckOutline(uint2 arg_uv, float4 arg_baseNormal, float4 arg_baseWorld)
{
    
    //�ǂꂭ�炢���[���h���W������Ă����炻���ɃA�E�g���C�����������ނ��B
    float outlineDistanceDeadline = 30.0f;
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
    
    //�A�E�g���C�����������ނ����v�Z�B
    bool isOutline = false;
    //�@���̈Ⴂ���`�F�b�N
    isOutline |= dot(sampleNormal.xyz, arg_baseNormal.xyz) < outlineNormalDeadline;
    //���[���h���W�ɂ��Ⴂ���`�F�b�N
    isOutline |= outlineDistanceDeadline < length(sampleWorld.xyz - arg_baseWorld.xyz);
    
    return isOutline;
    
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
    
    //�E�����`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, 0), baseNormal, baseWorld);
    
    //�������`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, 0), baseNormal, baseWorld);
    
    //�㑤���`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(0, -OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //�������`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(0, OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //�E�����`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //������`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //�������`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //�E����`�F�b�N
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld);
    

    if (isOutline)
    {
        
        OutputAlbedo[DTid.xy] = m_outlineColor;
        OutputEmissive[DTid.xy] = m_outlineColor;
        
    }
    else
    {
        
        OutputAlbedo[DTid.xy] = float4(0, 0, 0, 0);
        OutputEmissive[DTid.xy] = float4(0, 0, 0, 0);
        
    }

}