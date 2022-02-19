struct PS_IUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

cbuffer Constant
{
    float m_angle;
}

float4 main(PS_IUTPUT input) : SV_TARGET
{
    return float4(lerp(input.color, input.color1, (sin(m_angle) + 1.0f) / 2.0f), 1.0f);
}