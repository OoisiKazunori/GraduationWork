static const int ECHO_ELEMENT_COUNT = 64;
struct EchoData
{
	float3 m_pos;
    float m_radius;
    float m_alpha;
	int m_colorID;
    int2 pad;
};
//エコー配列
cbuffer EchoBuffer : register(b0)
{
	EchoData m_echoData[ECHO_ELEMENT_COUNT]; 
}
//Outline処理済みの描画
RWTexture2D<float4> Input : register(u0);
//ワールド座標
Texture2D<float4> TargetWorld : register(t0);
//出力結果
RWTexture2D<float4> Output : register(u1);


float4 SamplingPixel(Texture2D<float4> arg_texture, uint2 arg_uv)
{
    return arg_texture[uint2(clamp(arg_uv.x, 0, 1280), clamp(arg_uv.y, 0, 720))].xyzw;
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float4 color = float4(1,1,1,1);
    float4 inputC = Input[DTid.xy];

    //エコー配列のエッジの検出------------------------
    //エコーの円の中に入っているかの判定を取る
    float4 worldPos = SamplingPixel(TargetWorld,uint2(DTid.xy));
    for(int i = 0;i < ECHO_ELEMENT_COUNT; ++i)
    {
        float distanceNum = distance(m_echoData[i].m_pos,worldPos.xyz);
        //円の中に入っているか
        bool inRadiusFlag = distanceNum <= m_echoData[i].m_radius;
        //エコーの縁部分の判定を取る
        float distanceOfEdge =  m_echoData[i].m_radius - distanceNum;
        const float NEAR_TO_EDGE_NUM = 5.0f;
        bool isNearToEdgeFlag = distanceOfEdge <= NEAR_TO_EDGE_NUM;
        //円内かつ縁部分なら演出を有効にする
        if(inRadiusFlag && isNearToEdgeFlag)
        {
           color = float4(1,1,1,1);
        }
    }

    //エッジ部分のUVずらし------------------------



    //デバック向けに色を変える
    Output[DTid.xy] = inputC * color;
}