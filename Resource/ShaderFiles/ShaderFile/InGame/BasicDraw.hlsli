struct BasicDrawGBufferOutput
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 metalnessRoughness : SV_TARGET2;
    float4 world : SV_TARGET3;
    float4 emissive : SV_TARGET4;
    float4 outline : SV_TARGET5;
};