#include "shader.hlsl"

//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return float4(1.0,1.0f,1.0f,1.0);//vOutputColor;
}