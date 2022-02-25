Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

Texture2D NightTexture : register(t1);
sampler NightTextureSampler : register(s1);

struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 directionToCamera : TEXCOORD1;
};

cbuffer MVP : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    float4 lightDiretion;
    float4 cameraPosition;
    float lightIntensity;
    float3 lightColor;
};

float4 pixel(PS_IUTPUT input) : SV_TARGET
{
    float4 texColor = Texture.Sample(TextureSampler, 1.0 - input.texcoord);
    float4 nightColor = NightTexture.Sample(NightTextureSampler, 1.0 - input.texcoord);
    
    // 环境光
    float ka = lightIntensity;
    float3 ia = float3(.1f, .1f, .1f);
    ia *= texColor;
    ia *= lightColor.rgb;
    float3 ambientLight = ka * ia;   
    
    // 漫反射
    float kd = .9f;
    float3 idDay = float3(1.f, 1.f, 1.f);
    idDay *= texColor.rgb;
    idDay *= texColor.rgb;
    float3 idNight = float3(1.f, 1.f, 1.f);
    idNight *= nightColor.rgb;
    float amountDiffuseLight = dot(lightDiretion.xyz, input.normal);
    
    float3 id = lerp(idNight, idDay, (amountDiffuseLight + 1.f) / 2.f);
    
    float3 diffuseLight = kd *  id;
    
    // 镜面反射（高光）    
    float ks = 0.f;
    float3 is = float3(1.f, 1.f, 1.f);
    float3 reflectionLight = reflect(lightDiretion.xyz, input.normal);
    float shininess = 20.f;
    float amountSpecularLight = pow(max(0, dot(reflectionLight, input.directionToCamera)), shininess);
    
    float specularLight = ks * amountSpecularLight * is;
    
    float3 light = ambientLight + diffuseLight + specularLight;
    
    return float4(light, 1.f);
}