cbuffer ConstBuffer
{
  matrix world;
  matrix view;
  matrix proj;

  float4 cameraPos;
  float4 cameraDir;

  float4 kA;
  float4 kD;
  float4 kS;
  float4 kP;

  float4 lightPos;
  float4 lightColor;
};

struct VsOut
{
  float4 pos   : SV_POSITION;
  float4 w_pos : POSITION;
  float4 norm  : NORMAL;
  float2 tex   : TEXTURE;
};

VsOut main( float4 pos : POSITION, float3 norm : NORMAL, float2 tex : TEXTURE )
{
  VsOut output;

  pos = mul(pos, world);
  output.w_pos = pos;
  pos = mul(pos, view);
  pos = mul(pos, proj);

  output.norm = float4(norm.x, norm.y, norm.z, 0);
  output.norm = mul(output.norm, world);
  output.norm = normalize(output.norm);

  output.pos = pos;
  output.tex = tex;

  return output;
}