Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 directionToCamera : TEXCOORD1;
};

//cbuffer MVP : register(b0)
//{
//    row_major float4x4 world;
//    row_major float4x4 view;
//    row_major float4x4 projection;
//    float4 lightDiretion;
//};

float4 main(PS_IUTPUT input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, 1.f - input.texcoord);
} 