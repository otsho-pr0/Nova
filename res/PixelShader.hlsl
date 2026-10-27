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
		float3 sample = normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f);
		sample.y = -sample.y;
		float3 N = normalMapping ? sample : float3(0.0f, 0.0f, 1.0f);
		float3 L = normalize(-i.vLightDir);
		float3 V = normalize(i.vCameraDir);
		float3 H = normalize(L + V);
		return diff * (min(max(dot(L, N), 0.0f), 0.5f) + pow(max(dot(N, H), 0.0f), 32.0f) + 0.1f);
	} break;
	case 1:
	{
		return diffuse.Sample(ss, i.vTexCoord);
	} break;
	case 2:
	{
		float3 sample = normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f);
		sample.y = -sample.y;
		float3 N = normalMapping ? sample : float3(0.0f, 0.0f, 1.0f);
		float3 L = normalize(-i.vLightDir);
		float3 V = normalize(i.vCameraDir);
		float3 H = normalize(L + V);
		return pow(max(dot(N, H), 0.0f), 32.0f);
	} break;
	case 3:
	{
		return float4(i.vLightDir * 0.5f + 0.5f, 1.0f);
	} break;
	case 4:
	{
		float3 sample = normalize(normals.Sample(ss, i.vTexCoord).xyz * 2.0f - 1.0f);
		sample.y = -sample.y;
		float3 N = mul(normalMapping ? sample : float3(0.0f, 0.0f, 1.0f), float3x3(i.vTangent, i.vBiTangent, i.vNormal));
		return float4(N * 0.5f + 0.5f, 1.0f);
	} break;
	}
	
	return pow(i.SV.z, 100.0f);
}