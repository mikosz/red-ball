cbuffer WorldMatrixBuffer {
    matrix world;
}

cbuffer ViewMatrixBuffer {
    matrix view;
}

cbuffer ProjectionMatrixBuffer {
    matrix projection;
}

cbuffer CameraBuffer {
    float3 cameraPosition;
    
    float padding__;
};

struct Vertex {
    float4 position: POSITION;
    float2 textureCoordinates: TEXCOORD0;
    float3 normal: NORMAL;
};

struct Pixel {
    float4 position: SV_POSITION;
    float2 textureCoordinates: TEXCOORD0;
    float3 normal: NORMAL;
    float3 viewDirection: TEXCOORD1;
};

Pixel main(Vertex vertex) {
    Pixel pixel;
    
    vertex.position.w = 1.0f;
    pixel.position = mul(mul(mul(vertex.position, world), view), projection);
    pixel.textureCoordinates = vertex.textureCoordinates;
    pixel.normal = normalize(mul(vertex.normal, (float3x3) world));

    float4 worldPosition = mul(vertex.position, world);
    pixel.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);
    
    return pixel;
}
