float4 main(float2 aPos : POSITION) : SV_Position
{
	return float4(aPos, 0.0f, 1.0f);
}