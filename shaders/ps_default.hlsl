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
  //input.norm += float4(0, -1, 0, 0) * (1 - normTex.Sample(samplerState, input.tex));
  input.norm = normalize(input.norm);

  return input.norm;

  float4 L = normalize(lightPos - input.w_pos);
  float4 V = normalize(cameraPos - input.w_pos);
  float4 R = 2 * dot(L, input.norm) * input.norm - L;
  
  float4 color = kA +
    kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * pow(max(dot(R, V), 0), kP.x);
  
  //return float4(color.r, color.g, color.b, 1);
}
