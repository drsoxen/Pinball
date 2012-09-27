#include "lighthelper.hlsl"

float4x4 Projection;
float4x4 World;
float4x4 View;

float4x4 WVP;

float DeltaTime;
float TotalTime;

Texture2D Texture;

#define LIGHT_COUNT 8

Light Lights[LIGHT_COUNT];
int LightType;
int BlendEffect;
float3 EyePos;

SamplerState texSampler
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState texSampler2
{
	Filter = ANISOTROPIC;
	AddressU = Mirror;
	AddressV = Mirror;
};

struct VS_IN
{
	float3 Pos  : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 Normal : NORMAL;
	float4 diffuse : DIFFUSE;
    float4 spec    : SPECULAR;
};

struct VS_OUT
{
    float4 PosH  : SV_POSITION;
	float3 PosW    : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 diffuse : DIFFUSE;
    float4 spec    : SPECULAR;
};

struct GS_OUT
{
    float4 PosH  : SV_POSITION;
	float3 PosW    : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
	float4 diffuse : DIFFUSE;
    float4 spec    : SPECULAR;
};

VS_OUT VS(VS_IN vIn)
{
  VS_OUT vOut;

  //vOut.diffuse = Texture.Sample( texSampler, vIn.TexCoord );
  // Transform vertex an normal into the to world space space.
  vOut.PosW    = mul(float4(vIn.Pos, 1.0f), World);
  vOut.Normal = mul(float4(vIn.Normal, 0.0f), World);
  vOut.PosH = mul(float4(vIn.Pos, 1.0f), WVP);

  // Output vertex attributes for interpolation across triangle.
  vOut.diffuse = vIn.diffuse;
  vOut.spec    = vIn.spec;

  vOut.TexCoord = float2(vIn.TexCoord.x, 1.0 - vIn.TexCoord.y);
  return vOut;
}

[maxvertexcount(8)]
void GS( triangle VS_OUT In[3], inout TriangleStream<GS_OUT> TriStream )
{
    VS_OUT Out;
    //First loop through the first, 'normal' triangle
    //transform its 3 vertices and add them to the output stream

	
    
    Out.diffuse = In[0].diffuse;
    Out.spec = In[0].spec;

    for( int i = 0; i < 3; i++ )
    {
        Out.PosH = float4(In[i].PosH.x,In[i].PosH.y,In[i].PosH.z,In[i].PosH.w);
        Out.PosW = float3(In[i].PosW.x,In[i].PosW.y,In[i].PosW.z);
		Out.TexCoord = In[i].TexCoord;
		Out.Normal = In[i].Normal;

        TriStream.Append( Out );
    }
  
  //  TriStream.RestartStrip();//to end the first triangle
    


}

float4 PS(GS_OUT pIn) : SV_Target
{

  pIn.Normal = normalize(pIn.Normal);
   
  SurfaceInfo v = { pIn.PosW, pIn.Normal, Texture.Sample( texSampler, pIn.TexCoord ), pIn.spec};
  
  float3 litColor = float3(0, 0, 0);


  for(int i = 0; i < LIGHT_COUNT; i++)
  {
	if( Lights[i].lightOn == 1 )
	{
		if( Lights[i].lightType == 0 ) // Parallel
		{
		  litColor += ParallelLight(v, Lights[i], EyePos);
		}
		else if( Lights[i].lightType == 1 ) // Point
		{
		  litColor += PointLight(v, Lights[i], EyePos);	
		}
		else if( Lights[i].lightType == 2 ) 
		{
		  litColor += Spotlight(v, Lights[i], EyePos);
		}
	}
  }
  //return Texture.Sample( texSampler, pIn.TexCoord );
  //return float4(pIn.PosH, 1.0);

  /*Starting Blend Effects.*/
  if(BlendEffect == 0)
  {
	//NULL	
  }
  else if(BlendEffect == 1)
  {
    //SweetRaveParty
	litColor.r *= cos((TotalTime * 151) / 3);
	litColor.g *= cos((TotalTime * 237) / 3);
	litColor.b *= cos((TotalTime * 495) / 3);
  }
  else if(BlendEffect == 2)
  {
	//Fuzzy
	litColor *= sin((TotalTime * v.diffuse) * 2);
  }

  else if(BlendEffect == 3)
  {
  //ApeShit
	litColor = cos(((litColor * -1) * v.pos) * TotalTime);
  }
  /*Ending Blend Effects*/
  

  return float4(litColor, pIn.diffuse.a);
}

technique10 tech0
{
  pass pass0
  {
    SetVertexShader  ( CompileShader( vs_4_0, VS() ) );
    SetGeometryShader(NULL);
    SetPixelShader	 ( CompileShader( ps_4_0, PS() ) );
  }
}
