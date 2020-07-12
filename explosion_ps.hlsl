#include "shader.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
    return txDiffuse.Sample( samLinear, input.Tex ) * m_fFade;	
	
}


