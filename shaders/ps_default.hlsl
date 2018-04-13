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
  float4 w_pos : POSITION;
  float4 norm  : NORMAL;
  float2 tex   : TEXTURE;
};

float4 main( VsOut input ) : SV_TARGET
{
  float4 L, V, R;
  float4 color = kA;
  float t;

  // Light 0
  L = normalize(lights[0].pos - input.w_pos);
  V = normalize(cameraPos - input.w_pos);
  R = 2 * dot(L, input.norm) * input.norm - L;

  t = max(0, 1 - pow(length(lights[0].pos - input.w_pos) / lights[0].color.w, 2));

  color += (kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * pow(max(dot(R, V), 0), kP.x)) * lights[0].color * t;

  // Light 1
  L = normalize(lights[1].pos - input.w_pos);
  V = normalize(cameraPos - input.w_pos);
  R = 2 * dot(L, input.norm) * input.norm - L;

  t = max(0, 1 - pow(length(lights[1].pos - input.w_pos) / lights[1].color.w, 2));

  color += (kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * pow(max(dot(R, V), 0), kP.x)) * lights[1].color * t;

  // Light 2
  L = normalize(lights[2].pos - input.w_pos);
  V = normalize(cameraPos - input.w_pos);
  R = 2 * dot(L, input.norm) * input.norm - L;

  t = max(0, 1 - pow(length(lights[2].pos - input.w_pos) / lights[2].color.w, 2));

  color += (kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * pow(max(dot(R, V), 0), kP.x)) * lights[2].color * t;

  // Light 3
  L = normalize(lights[3].pos - input.w_pos);
  V = normalize(cameraPos - input.w_pos);
  R = 2 * dot(L, input.norm) * input.norm - L;

  t = max(0, 1 - pow(length(lights[3].pos - input.w_pos) / lights[3].color.w, 2));

  color += (kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * pow(max(dot(R, V), 0), kP.x)) * lights[3].color * t;

  // Light 4
  L = normalize(lights[4].pos - input.w_pos);
  V = normalize(cameraPos - input.w_pos);
  R = 2 * dot(L, input.norm) * input.norm - L;

  t = max(0, 1 - pow(length(lights[4].pos - input.w_pos) / lights[4].color.w, 2));

  color += (kD * diffTex.Sample(samplerState, input.tex) * max(0, dot(input.norm, L)) +
    kS * pow(max(dot(R, V), 0), kP.x)) * lights[4].color * t;

  return float4(color.r, color.g, color.b, 1);
}
