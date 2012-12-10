cbuffer MatrixBuffer {
    matrix world;
    matrix view;
    matrix projection;
}

struct Vertex {
	float4 position: POSITION;
	float2 textureCoordinates: TEXCOORD0;
};

struct Pixel {
	float4 position: SV_POSITION;
	float2 textureCoordinates: TEXCOORD0;
};

Pixel main(Vertex vertex) {
	Pixel pixel;
	
	vertex.position.w = 1.0f;
	pixel.position = mul(mul(mul(vertex.position, world), view), projection);
	pixel.textureCoordinates = vertex.textureCoordinates;
	
	return pixel;
}
