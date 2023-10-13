//入力情報
Texture2D<float4> TargetWorld : register(t0);
Texture2D<float4> TargetNormal : register(t1);

//出力先UAV  
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
    
    //どれくらいワールド座標が離れていたらそこにアウトラインを書き込むか。
    float outlineDistanceDeadline = 30.0f;
    //法線の内積の差分がどれくらいだったらアウトラインを書き込むか。
    float outlineNormalDeadline = 0.8f;
    
    //各情報をサンプリングする。
    float4 sampleNormal = SamplingPixel(TargetNormal, arg_uv);
    float4 sampleWorld = SamplingPixel(TargetWorld, arg_uv);
    
    //Baseの法線もこのピクセルの法線も書き込まれていなかったら飛ばす。
    if (length(arg_baseNormal.xyz) <= 0.1f && length(sampleNormal.xyz) < 0.1f)
    {
        return false;
    }
    
    //アウトラインを書き込むかを計算。
    bool isOutline = false;
    //法線の違いをチェック
    isOutline |= dot(sampleNormal.xyz, arg_baseNormal.xyz) < outlineNormalDeadline;
    //ワールド座標による違いもチェック
    isOutline |= outlineDistanceDeadline < length(sampleWorld.xyz - arg_baseWorld.xyz);
    
    return isOutline;
    
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    //アウトラインの厚さ
    const int OUTLINE_THICKNESS = 1;
    
    //このピクセルにアウトラインを書くか？
    bool isOutline = false;
    
    //このピクセルの色情報
    float4 baseNormal = SamplingPixel(TargetNormal, DTid.xy);
    float4 baseWorld = SamplingPixel(TargetWorld, DTid.xy);
    
    //右側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, 0), baseNormal, baseWorld);
    
    //左側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, 0), baseNormal, baseWorld);
    
    //上側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(0, -OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //下側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(0, OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //右下をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //左上をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //左下をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld);
    
    //右上をチェック
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