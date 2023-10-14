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

cbuffer EchoData : register(b1)
{
    float3 m_color;
    float m_echoAlpha;
	float3 m_center;
    float m_echoRadius;
}


float4 SamplingPixel(Texture2D<float4> arg_texture, uint2 arg_uv)
{
    return arg_texture[uint2(clamp(arg_uv.x, 0, 1280), clamp(arg_uv.y, 0, 720))].xyzw;
}

bool CheckOutline(uint2 arg_uv, float4 arg_baseNormal, float4 arg_baseWorld, inout float3 arg_sampleWorldPos)
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
    //ワールド情報が書き込まれていなかったらアウト。
    if (length(sampleWorld.xyz) <= 0.1f)
    {
        return false;
    }
    
    //法線の違いをチェック
    bool isCheckNormal = dot(sampleNormal.xyz, arg_baseNormal.xyz) < outlineNormalDeadline;
    //ワールド座標による違いもチェック
    bool isCheckWorld = outlineDistanceDeadline < length(sampleWorld.xyz - arg_baseWorld.xyz);
    //床のアウトライン対策。
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
    
    //アウトラインの厚さ
    const int OUTLINE_THICKNESS = 1;
    
    //このピクセルにアウトラインを書くか？
    bool isOutline = false;
    
    //このピクセルの色情報
    float4 baseNormal = SamplingPixel(TargetNormal, DTid.xy);
    float4 baseWorld = SamplingPixel(TargetWorld, DTid.xy);
    
    //サンプリングしたワールド座標
    float3 sampleWorldPos = float3(0.0f, 0.0f, 0.0f);
    
    //右側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, 0), baseNormal, baseWorld, sampleWorldPos);
    
    //左側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, 0), baseNormal, baseWorld, sampleWorldPos);
    
    //上側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(0, -OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    //下側をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(0, OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    //右下をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    //左上をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    //左下をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(-OUTLINE_THICKNESS, OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    //右上をチェック
    isOutline |= CheckOutline(DTid.xy + uint2(OUTLINE_THICKNESS, -OUTLINE_THICKNESS), baseNormal, baseWorld, sampleWorldPos);
    
    if (isOutline)
    {
        
        //サンプリングした位置に応じて色を暗くする。
        float edgeDistance = 1.0f - clamp(length(sampleWorldPos - m_outlineCenterPos) / m_outlineLength, 0.0f, 0.8f);
        
        OutputAlbedo[DTid.xy] = m_outlineColor * edgeDistance;
        OutputEmissive[DTid.xy] = m_outlineColor * edgeDistance;
        
    }
    else
    {
        
        OutputAlbedo[DTid.xy] = float4(0, 0, 0, 0);
        OutputEmissive[DTid.xy] = float4(0, 0, 0, 0);
        
    }
    
    //中心地点から一定の距離だったら
    const float GRID_SCOPE = 100.0f;
    const int GRID_RANGE = 15.0f;
    float baseWorldDistance = length(baseWorld.xyz - m_outlineCenterPos);
    if (baseWorldDistance <= GRID_SCOPE && 0.0f < length(baseWorld.xyz))
    {
        
        //ワールド座標の値がGRID_RANGEで割り切れる値に近かったら
        bool isGrid = frac(baseWorld.x / GRID_RANGE) < 0.01f;
        isGrid |= baseNormal.y < 0.9f && (frac(baseWorld.y / GRID_RANGE) < 0.01f);
        isGrid |= frac(baseWorld.z / GRID_RANGE) < 0.01f;
        
        //割合を求める。
        float distanceRate = 1.0f - baseWorldDistance / GRID_SCOPE;
        distanceRate *= 0.3f;
        
        if (isGrid)
        {
            
            OutputAlbedo[DTid.xy] += m_outlineColor * distanceRate;
            OutputEmissive[DTid.xy] += m_outlineColor * distanceRate;

        }
        
    }
    
    //エコーを描画
    if (length(baseWorld.xyz - m_center) <= m_echoRadius && 0.0f < length(baseWorld.xyz))
    {
        
        //極細のグリッドを出す。
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