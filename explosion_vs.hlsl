#include "shader.hlsl"


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
EXPLOSIONPOS VS( EXPLOSIONPOS input )
{
	EXPLOSIONPOS output = (EXPLOSIONPOS)0;
 	float4 l_fScaledPos;
        l_fScaledPos.xyz = input.Pos.xyz * m_fScalePos;
        l_fScaledPos.w = 1.0f;

	// Transform to homogeneous clip space.
	output.Pos = mul( l_fScaledPos, g_matWorldViewProj );

	//output.NextPos = mul(input.NextPos, g_matWorldViewProj);
		
	return output;
}


