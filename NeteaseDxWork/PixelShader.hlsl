struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

cbuffer CONSTANT
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    uint time;
};

float4 main(PS_IUTPUT input) : SV_TARGET
{
    return float4(lerp(input.color, input.color1, (sin(time / 1000.f) + 1.0f) / 2.0f), 1.0f);
}