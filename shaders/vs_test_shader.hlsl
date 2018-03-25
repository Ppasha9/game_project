cbuffer MatrixBuffer
{
  matrix world;
  matrix view;
  matrix proj;
};

float4 main( float4 pos : POSITION, float3 norm : NORMAL, float2 tex : TEXTURE ) : SV_POSITION
{
  pos = mul(pos, world);
  pos = mul(pos, view);
  pos = mul(pos, proj);

  return pos;
}