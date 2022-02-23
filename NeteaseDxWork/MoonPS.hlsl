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

    float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
    float kd = .9;
    float3 id = float3(1.0, 1.0, 1.0);
    id *= texColor.rgb;
    float amount_diffuse_light = max(0.0, dot(lightDiretion.xyz, input.normal));

    float3 diffuse_light = kd * amount_diffuse_light * id;

	//SPECULAR LIGHT
    float ks = 0;
    float3 is = float3(1.0, 1.0, 1.0);
    float3 reflected_light = reflect(lightDiretion.xyz, input.normal);
    float shininess = 20.0;
    float amount_specular_light = pow(max(0.0, dot(reflected_light, input.directionToCamera)), shininess);

    float3 specular_light = ks * amount_specular_light * is;

    float3 final_light = ambient_light + diffuse_light + specular_light;

    return float4(final_light, 1.0);
}