struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};

cbuffer MatBuffer : register(b0)
{
    matrix mat; //3D�ϊ��s��
}

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput op;
	op.svpos = mul(mat, pos);
	op.uv = uv;
	return op;
}

Texture2D<uint> tex : register(t0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    //ステンシル情報を抽出する
    uint stencil = (tex[input.uv * float2(1280,720)] & 0x000F) > 24;
    float4 output = float4(stencil,0,0,1);
    return output;
}