struct VSOutput
{
	float4 vPos : SV_Position;
	float2 vUV : TEXCOORD;
};

VSOutput main(float2 aPos : POSITION)
{
	VSOutput o;
	o.vPos = float4(aPos, 0.0f, 1.0f);
	o.vUV = float2(aPos.x * 0.5f + 0.5f, -aPos.y * 0.5f + 0.5f);
	return o;
}