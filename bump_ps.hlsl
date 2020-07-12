#include "shader.hlsl"
//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------

void ComputeSpotLightObjRelative(Material d3dMaterial, SpotLight _SpotLight, float3 pos, float3 vNormal, float3 toEyeW, 
								 out float4 ambient, out float4 diffuse, out float4 spec)

//void ComputeSpotLightObjRelative(Material mat, SpotLight l_spotlight, float3 pos, float3 normal, float3 toEye,
				  
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = SpotLightPositionObjRelative.xyz -pos.xyz;
	//float3 lightVec = _SpotLight.Position.xyz - pos.xyz;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > _SpotLight.Range )
		return;

	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = d3dMaterial.Ambient * _SpotLight.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, vNormal);	
	
	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, vNormal);
		float specFactor = pow(max(dot(v, toEyeW), 0.0f), d3dMaterial.Specular.w);
					
		diffuse = diffuseFactor * d3dMaterial.Diffuse * _SpotLight.Diffuse;		
		spec    = specFactor * d3dMaterial.Specular * _SpotLight.Specular;
	       
    }
	
	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec.xyz, 
                            SpotLightDirectionObjRelative.xyz), 0.0f), 
                            _SpotLight.Spot);	
	//float3 l_SpotLightDirectionObjRelative = SpotLightDirectionObjRelative;
	//l_SpotLightDirectionObjRelative.x = 1.0f;
	//l_SpotLightDirectionObjRelative.y = 0.0f;
	//l_SpotLightDirectionObjRelative.z = 0.0f;
	//float spot = max(dot(-lightVec, SpotLightDirectionObjRelative), 0.0f);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(_SpotLight.Att, float3(1.0f, d, d*d));
			
	//ambient *= spot; 

	// >> temporär auskommentiert, weil att irgendwie gerade schrott ist:
	//diffuse = diffuse * att;//	spot;
	// << temporär auskommentiert, weil att irgendwie gerade schrott ist:
	spec    *= spot;//att;

}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
    //return input.Color;
	
	// Interpolating normal can unnormalize it, so normalize it.
	//input.Norm = normalize(input.Norm.xyz);

	//kamera muss eh nochmal differenziert werden
	float3 toEyeW = normalize(SpotLightPositionObjRelative.xyz - input.PosW.xyz);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;

	/*
	float4 finalColor = 0;
    //do NdotL lighting for 2 lights	
    for(int i=0; i<2; i++)
    {
        finalColor += saturate( dot( (float3)DirLight[i].Direction,input.Norm) * DirLight[i].Diffuse );
		
    }
	*/
	
	float3 vNormal = (txNormals.Sample( samLinear, input.Tex ).xyz - 0.5f ) * -2.0f;
	vNormal = normalize(vNormal);
	/*
    for(int i=0; i<2; i++)
    {
		
        //finalColor += saturate( dot( (float3)vLightDir[i],input.Norm) * vLightColor[i] );
	
		//ComputeDirectionalLight(d3dMaterial, DirLight[i], vNormal, toEyeW, A, D, S);
		ComputeDirectionalLight(d3dMaterial, DirLight[i], input.Norm.xyz, toEyeW, A, D, S);
		
		ambient += A;  
		diffuse += D;
		spec    += S;
			
		
	}
	*/
	

	/*
	ComputePointLight(d3dMaterial, _PointLight, input.PosW, input.Norm, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec    += S;
	*/
		
	//float diffuseFactor = dot(toEyeW, vNormal);
	//diffuse = diffuseFactor * _SpotLight.Diffuse;
	// soll in dieser Funktion eigentlich passieren:
/* kernaktion hieraus passiert unten direkt: diffusefactor = ...
	ComputeSpotLightObjRelative(d3dMaterial, 
                                    _SpotLight, 
                                    input.PosW.xyz,  
									normalize(input.Norm.xyz), //statt "vNormal," ...benutzt also nicht die Normalmap!
                                    toEyeW, 
                                    A, 
                                    D, 
                                    S);
	ambient += A;
	diffuse += D;
	spec    += S;
	*/
			
	// The vector from the surface to the light.
	//float3 lightVec = _SpotLight.Position - input.Pos;	
	float3 lightVec = SpotLightPositionObjRelative.xyz - input.PosW.xyz;

	// The distance from surface to light.
	float d = length(lightVec);

	// Normalize the light vector.
	lightVec /= d; 

	float diffuseFactor = dot(lightVec, vNormal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, vNormal);
		float specFactor = pow(max(dot(v, toEyeW), 0.0f), d3dMaterial.Specular.w);

		diffuse = diffuseFactor * d3dMaterial.Diffuse * _SpotLight.Diffuse;
		spec = specFactor * d3dMaterial.Specular * _SpotLight.Specular;

	}

		
	// >> temporär auskommentiert um nur diffuse zu sehen
	//float4 finalColor = ambient + diffuse + spec;
	// << temporär auskommentiert um nur diffuse zu sehen
	float4 finalColor = diffuse;

	// Common to take alpha from diffuse material.
    finalColor.a = 1;

    return finalColor * txDiffuse.Sample( samLinear, input.Tex ) + txEmissive.Sample(samLinear, input.Tex);	
	
}


