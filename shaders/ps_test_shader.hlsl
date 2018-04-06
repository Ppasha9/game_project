Texture2D ambiTex : register(t0);
Texture2D diffTex : register(t1);
Texture2D specTex : register(t2);
Texture2D normTex : register(t3);

SamplerState samplerState;

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

float4 main( VsOut input ) : SV_TARGET
{
  float4 L = normalize(lightPos - input.w_pos);
  float4 V = normalize(cameraPos - input.w_pos);
  float4 R = 2 * dot(L, input.norm) * input.norm - L;
  
  float4 color = kA * ambiTex.Sample(samplerState, input.tex) +
    kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * specTex.Sample(samplerState, input.tex) * pow(max(dot(R, V), 0), kP.x);
  
  return float4(color.r, color.g, color.b, 1);
}
