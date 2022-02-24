Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 directionToCamera : TEXCOORD1;
};

float4 pixel(PS_IUTPUT input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, 1.f - input.texcoord);
} 