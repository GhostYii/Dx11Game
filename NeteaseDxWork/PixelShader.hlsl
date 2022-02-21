Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

cbuffer MVP : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    float4 lightDiretion;
};

float4 main(PS_IUTPUT input) : SV_TARGET
{
    float ka = .1f;
    float3 ia = float3(1.f, 1.f, 1.f);
    float3 ambientLight = ka * ia;
    
    float kd = 1.f;
    float3 id = float3(1.f, 1.f, 1.f);
    float amountDiffuseLight = max(0, dot(lightDiretion.xyz, input.normal));
    
    float diffuseLight = kd * amountDiffuseLight * id;
    float3 light = ambientLight + diffuseLight;
    
    return float4(light, 1.f);
    
    //return Texture.Sample(TextureSampler, input.texcoord);
}