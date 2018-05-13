Texture2D ambiTex : register(t0);
Texture2D diffTex : register(t1);
Texture2D specTex : register(t2);
Texture2D normTex : register(t3);

SamplerState samplerState;

struct LightSource
{
  float4 pos;
  float4 color;
};

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

  LightSource lights[5];
};

struct VsOut
{
  float4 pos   : SV_POSITION;
  float2 tex   : TEXTURE;
};

float4 main(VsOut input) : SV_TARGET
{
  return float4(kA * ambiTex.Sample(samplerState, input.tex));
}
