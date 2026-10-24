cbuffer uni
{
	int mode;
	bool normalMapping;
};

Texture2D diffuse;
Texture2D normals;
SamplerState ss;

struct VSOutput
{
	float4 SV : SV_Position;
	float3 vTangent : TANGENT;
	float3 vBiTangent : BITANGENT;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;
	float3 vLightDir : LIGHTDIR;
	float3 vCameraDir : CAMERADIR;
};

float4 main(VSOutput i) : SV_Target
{
	switch (mode)
	{
	case 0:
	{
		float4 diff = diffuse.Sample(ss, i.vTexCoord);
		clip(diff.a - 0.9f);
		float3 norm = normalMapping ? normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f) : float3(0.0f, 0.0f, 1.0f);
		return diff * (min(max(dot(-i.vLightDir, norm), 0.0f), 0.5f) + pow(max(dot(reflect(i.vLightDir, norm), i.vCameraDir), 0.0f), 32.0f) + 0.1f);
	} break;
	case 1:
	{
		return diffuse.Sample(ss, i.vTexCoord);
	} break;
	case 2:
	{
		float3 norm = normalMapping ? normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f) : float3(0.0f, 0.0f, 1.0f);
		return pow(max(dot(reflect(i.vLightDir, norm), i.vCameraDir), 0.0f), 32.0f);
	} break;
	case 3:
	{
		return float4(i.vLightDir * 0.5f + 0.5f, 1.0f);
	} break;
	case 4:
	{
		float3 norm = normalMapping ? mul(normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f), float3x3(i.vTangent, i.vBiTangent, i.vNormal)) : i.vNormal;
		return float4(norm * 0.5f + 0.5f, 1.0f);
	} break;
	}
	
	return 1.0f;
}