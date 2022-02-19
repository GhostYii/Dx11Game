struct VS_INPUT
{
    float4 position : POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

cbuffer MVP
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    uint time;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
        
    //output.position = lerp(input.position, input.position1, (sin(time / 1000.f) + 1.0f) / 2.0f);
    
    // world space
    output.position = mul(input.position, world);
    // view space
    output.position = mul(output.position, view);
    // projection space
    output.position = mul(output.position, projection);
    
    output.color = input.color;
    output.color1 = input.color1;
    
	return output;
}