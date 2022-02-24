Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

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
};

float4 pixel(PS_IUTPUT input) : SV_TARGET
{
    float4 texColor = Texture.Sample(TextureSampler, 1.0 - input.texcoord);    
    
	//AMBIENT LIGHT
    float ka = 1.5;
    float3 ia = float3(.1, .1, .1);
    ia *= texColor.rgb;

    float3 ambientLight = ka * ia;

	//DIFFUSE LIGHT
    float kd = .9;
    float3 id = float3(1.0, 1.0, 1.0);
    id *= texColor.rgb;
    float amountDiffuseLight = max(0.0, dot(lightDiretion.xyz, input.normal));

    float3 diffuseLight = kd * amountDiffuseLight * id;

	//SPECULAR LIGHT
    float ks = 0;
    float3 is = float3(1.0, 1.0, 1.0);
    float3 reflectedLight = reflect(lightDiretion.xyz, input.normal);
    float shininess = 20.0;
    float amountSpecularLight = pow(max(0.0, dot(reflectedLight, input.directionToCamera)), shininess);

    float3 specularLight = ks * amountSpecularLight * is;

    float3 finalLight = ambientLight + diffuseLight + specularLight;

    return float4(finalLight, 1.0);
}