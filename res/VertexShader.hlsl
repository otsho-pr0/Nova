cbuffer uni
{
	matrix projection;
	matrix view;
	matrix model;
};

struct VSOutput
{
	float4 SV : SV_Position;
	float3 vPos : POSITION;
	float3 vTangent : TANGENT;
	float3 vBiTangent : BITANGENT;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;
};

VSOutput main(float3 aPos : POSITION, float3 aTangent : TANGENT, float3 aBiTangent : BITANGENT, float3 aNormal : NORMAL, float2 aTexCoord : TEXCOORD)
{
	VSOutput o;
	o.SV = mul(mul(mul(float4(aPos, 1.0f), model), view), projection);
	o.vPos = mul(float4(aPos, 1.0f), model).xyz;
	o.vTangent = normalize(mul(aTangent, (float3x3)model));
	o.vBiTangent = normalize(mul(aBiTangent, (float3x3)model));
	o.vNormal = normalize(mul(aNormal, (float3x3)model));
	o.vTexCoord = aTexCoord;
	return o;
}