#include "Lights.fx"

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 PosW : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};



VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;
	output.PosW = mul(inPos, World).xyz;
	output.Pos = mul(inPos, WVP);

	output.normal = mul(normal, World);

	output.TexCoord = inTexCoord;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET{
	input.normal = normalize(input.normal);
	float4 mapDiffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	float3 toEyeW = normalize(gEyePosW - input.PosW);
	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	// Sum the light contribution from each light source.
	float4 A, D, S;
	ComputeDirectionalLight(material, gDirLight, input.normal, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;
	ComputePointLight(material, gPointLight, input.PosW, input.normal, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;
	ComputeSpotLight(material, gSpotLight, input.PosW, input.normal, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;
	float4 litColor = mapDiffuse * (ambient + diffuse) + spec;
	// Common to take alpha from diffuse material.
	litColor.a = material.Diffuse.a * mapDiffuse.a;
	return litColor;
}


float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	return diffuse;
}
