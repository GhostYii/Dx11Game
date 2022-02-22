Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 directionToCamera : TEXCOORD2;
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
    // 环境光
    float ka = .1f;
    float3 ia = float3(1.f, 1.f, 1.f);
    float3 ambientLight = ka * ia;
    
    // 漫反射
    float kd = .7f;
    float3 id = float3(1.f, 1.f, 1.f);
    float amountDiffuseLight = max(0, dot(lightDiretion.xyz, input.normal));
    
    float diffuseLight = kd * amountDiffuseLight * id;
    
    // 镜面反射（高光）
    float ks = 1.f;
    float3 is = float3(1.f, 1.f, 1.f);
    float3 reflectionLight = reflect(lightDiretion.xyz, input.normal);
    float shininess = 20.f;
    float amountSpecularLight = pow(max(0, dot(reflectionLight, input.directionToCamera)), shininess);
    
    float specularLight = ks * amountSpecularLight * is;
    
    float3 light = ambientLight + diffuseLight + specularLight;
    
    return float4(light, 1.f);
    
    //return Texture.Sample(TextureSampler, input.texcoord);
}