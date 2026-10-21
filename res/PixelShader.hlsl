cbuffer uni
{
	int mode;
	int normalMap;
};

Texture2D diffuse;
Texture2D normals;
SamplerState ss;

struct VSOutput
{
	float4 SV : SV_Position;
	float3 vPos : POSITION;
	float3 vTangent : TANGENT;
	float3 vBiTangent : BITANGENT;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;
	float3 vLightDir : LIGHTDIR;
	float3 vCameraPos : CAMERAPOS;
};

float4 main(VSOutput i) : SV_Target
{
	switch (mode)
	{
	default:
	case 0:
	{
		float4 diff = diffuse.Sample(ss, i.vTexCoord);
		clip(diff.a - 0.9f);
		float3 norm = normalMap == 1 ? normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f) : float3(0.0f, 0.0f, 1.0f);
		//if (normalMap == 1)
		//		norm.y = -norm.y;
		return diff * (min(max(dot(-i.vLightDir, norm), 0.0f), 0.5f) + pow(max(dot(reflect(i.vLightDir, norm), normalize(i.vCameraPos - i.vPos)), 0.0f), 16.0f) + 0.1f);
	} break;
	case 1:
	{
		return diffuse.Sample(ss, i.vTexCoord);
	} break;
	case 2:
	{
		float3 norm = normalMap == 1 ? mul(normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f), float3x3(i.vTangent, i.vBiTangent, i.vNormal)) : mul(float3(0.0f, 0.0f, 1.0f), float3x3(i.vTangent, i.vBiTangent, i.vNormal));
		//if (normalMap == 1)
		//		norm.y = -norm.y;
		return float4((norm + 1.0f) / 2.0f, 1.0f);
	} break;
	case 3:
	{
		float3 norm = mul(float3(0.0f, 1.0f, 0.0f), float3x3(i.vTangent, i.vBiTangent, i.vNormal));
		return float4((norm + 1.0f) / 2.0f, 1.0f);
	} break;
	}
}