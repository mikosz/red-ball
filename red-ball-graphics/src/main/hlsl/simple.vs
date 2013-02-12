cbuffer WorldMatrixBuffer {
    matrix world;
}

cbuffer ViewMatrixBuffer {
    matrix view;
}

cbuffer ProjectionMatrixBuffer {
    matrix projection;
}

struct Vertex {
	float4 position: POSITION;
	float2 textureCoordinates: TEXCOORD0;
	float3 normal: NORMAL;
};

struct Pixel {
	float4 position: SV_POSITION;
	float2 textureCoordinates: TEXCOORD0;
	float3 normal: NORMAL;
};

Pixel main(Vertex vertex) {
	Pixel pixel;
	
	vertex.position.w = 1.0f;
	pixel.position = mul(mul(mul(vertex.position, world), view), projection);
	pixel.textureCoordinates = vertex.textureCoordinates;
	pixel.normal = normalize(mul(vertex.normal, (float3x3) world));
	
	return pixel;
}
