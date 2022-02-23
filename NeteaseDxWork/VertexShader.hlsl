struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
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

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
        
    //output.position = lerp(input.position, input.position1, (sin(time / 1000.f) + 1.0f) / 2.0f);
    
    // world space
    output.position = mul(input.position, world);
    output.directionToCamera = normalize(output.position.xyz - cameraPosition.xyz);
    // view space
    output.position = mul(output.position, view);
    // projection space
    output.position = mul(output.position, projection);
    
    output.texcoord = input.texcoord;
    output.normal = input.normal;
    
	return output;
}