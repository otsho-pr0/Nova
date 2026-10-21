struct VSOutput
{
	float4 SV : SV_Position;
	float2 coord : TEXCOORD;
};

struct Vertex
{
	float2 position;
	float2 coord;
};

cbuffer uni
{
	float2 coord;
	float aspect;
};

VSOutput main(uint vertexID : SV_VertexID)
{

	Vertex vertices[] =
	{
		-0.005f + coord.x, +0.005f * aspect + coord.y, -1.0f,  1.0f,
		+0.005f + coord.x, +0.005f * aspect + coord.y,  1.0f,  1.0f,
		-0.005f + coord.x, -0.005f * aspect + coord.y, -1.0f, -1.0f,
		
		+0.005f + coord.x, +0.005f * aspect + coord.y,  1.0f,  1.0f,
		+0.005f + coord.x, -0.005f * aspect + coord.y,  1.0f, -1.0f,
		-0.005f + coord.x, -0.005f * aspect + coord.y, -1.0f, -1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 2, 1, 3, 2
	};

	VSOutput o;
	o.SV = float4(vertices[vertexID].position, float2(0.0f, 1.0f));
	o.coord = vertices[vertexID].coord;

	return o;
}