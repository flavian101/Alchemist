struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};
// Pixel shader function
float4 main(VertexOut pIn) : SV_Target
{
    return pIn.color;
}
