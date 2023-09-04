cbuffer solid_color_buffer : register(b0)
{
    float4 solid_color;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
};

struct PSOutput
{
    float4 Position : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Color : SV_TARGET2;
};

PSOutput ps_main(VSOutput ps_input)
{
    PSOutput output = (PSOutput) 0;
    
    output.Color = solid_color;
    output.Normal = ps_input.Normal;
    output.Position = ps_input.Position;
    
    return output;
}