struct VSOutput
{
	float4 SV : SV_Position;
	float2 coord : TEXCOORD;
};

float4 main(VSOutput i) : SV_Target
{
	clip((dot(i.coord, i.coord) < 1.0f) - 1.0f);
	return 1.0f;
}