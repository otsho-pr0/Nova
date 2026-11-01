cbuffer uni
{
	int mode;
	bool normalMapping;
	bool emission;
	float3 lightDir;
	float3 cameraPos;
};

Texture2D albedo;
Texture2D normals;
Texture2D positions;

SamplerState ss;

float4 main(float4 vPos : SV_Position, float2 vUV : TEXCOORD) : SV_Target
{
	switch (mode)
	{
	case 1:
	{
		float3 N = normalize(normals.Sample(ss, vUV).xyz);
		float3 L = normalize(-lightDir);
		float3 V = normalize(cameraPos - positions.Sample(ss, vUV).xyz);
		float3 H = normalize(L + V);
		float specAngle = max(dot(N, H), 0.0);
		float specularFactor = pow(specAngle, 32.0f);
		return albedo.Sample(ss, vUV) * (max(dot(N, L), 0.0f) + specularFactor + 0.1f);
	} break;
	case 2:
	{
		float3 N = normalize(normals.Sample(ss, vUV).xyz);
		float3 L = normalize(-lightDir);
		float3 V = normalize(cameraPos - positions.Sample(ss, vUV).xyz);
		float3 H = normalize(L + V);
		float specAngle = max(dot(N, H), 0.0);
		float specularFactor = pow(specAngle, 32.0f);
		return float4((float3)specularFactor, 1.0f);
	} break;
	case 3:
	{
		float3 N = normalize(normals.Sample(ss, vUV).xyz) * 0.5f + 0.5f;
		return float4(N, 1.0f);
	} break;
	}
	
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}