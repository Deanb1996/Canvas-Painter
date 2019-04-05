//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4 LightColour;
	float4 LightPosition;
	float4 CameraPosition;
}

//--------------------------------------------------------------------------------------
// Shader Inputs
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float3 InstancePos : INSTANCEPOSITION;
	float3 InstanceColour : INSTANCECOLOUR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal: NORMAL;
	float4 PosWorld : TEXCOORD0;
	float2 TexCoord : TEXCOORD1;
	float3 Colour : TEXCOORD2;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = float4(input.Pos + input.InstancePos, 1);
	output.Pos = mul(output.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = mul(World, float4(input.Normal, 1.0f)).xyz;
	output.Normal = normalize(output.Normal);
	output.PosWorld = mul(float4(input.Pos, 1.0f), World);
	output.TexCoord = input.TexCoord;
	output.Colour = input.InstanceColour;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 matDiffuse = float4(0.8, 0.8, 0.8, 1.0);
	float4 matSpec = float4(1.0, 1.0, 1.0, 1.0);
	float4 ambient = float4(0.1, 0.1, 0.1, 1.0);
	float4 texColour = float4(input.Colour, 1);

	float3 viewDirection = normalize(CameraPosition - input.PosWorld);
	float4 light = ambient;

	float3 lightDir = normalize(LightPosition - input.PosWorld);
	float diffuse = max(0.0, dot(lightDir, input.Normal));
	float3 reflection = normalize(reflect(-lightDir, input.Normal));
	float spec = pow(max(0.0, dot(viewDirection, reflection)), 0.1 * 128);
	light += saturate(((matDiffuse*diffuse) + (matSpec*spec)) * LightColour);

	return light * texColour;
}
