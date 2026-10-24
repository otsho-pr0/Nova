cbuffer uni
{
	matrix projection;
	matrix view;
	matrix model;
};

cbuffer n
{
	float3 lightDir;
	float3 cameraPos;
};

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

VSOutput main(float3 aPos : POSITION, float3 aTangent : TANGENT, float3 aBiTangent : BITANGENT, float3 aNormal : NORMAL, float2 aTexCoord : TEXCOORD)
{
	VSOutput o;
	o.SV = mul(mul(mul(float4(aPos, 1.0f), model), view), projection);
	o.vTangent = normalize(mul(aTangent, (float3x3)model));
	o.vBiTangent = normalize(mul(aBiTangent, (float3x3)model));
	o.vNormal = normalize(mul(aNormal, (float3x3)model));
	float3x3 TBN = float3x3(o.vTangent, o.vBiTangent, o.vNormal);
	o.vTexCoord = aTexCoord;
	o.vLightDir = mul(TBN, lightDir);
	o.vCameraDir = normalize(mul(TBN, cameraPos - mul(float4(aPos, 1.0f), model).xyz));
	return o;
}