cbuffer uni
{
	int mode;
	bool normalMapping;
	bool emission;
	float3 lightDir;
	float3 cameraPos;
};

Texture2D diffuse;
Texture2D normals;
Texture2D emissive;
SamplerState ss;

struct VSOutput
{
	float4 SV : SV_Position;
	float3 vPos : POSITION;
	float3 vTangent : TANGENT;
	float3 vBiTangent : BITANGENT;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;
};

struct PSOutput
{
	float4 albedo : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

PSOutput main(VSOutput i)
{
	PSOutput o;
	o.albedo = emission ? diffuse.Sample(ss, i.vTexCoord) + emissive.Sample(ss, i.vTexCoord) : diffuse.Sample(ss, i.vTexCoord);
	clip(o.albedo.a - 1.0f);
	float3 sample = normals.Sample(ss, i.vTexCoord).rgb * 2.0f - 1.0f;
	sample.g = -sample.g;
	o.normal = float4(mul(normalMapping ? sample : float3(0.0f, 0.0f, 1.0f), float3x3(i.vTangent, i.vBiTangent, i.vNormal)), 1.0f);
	o.position = float4(i.vPos, 1.0f);
	return o;
}