
#include "Shader_Engine_Defines.hlsli"
/* float2 float3 float4 == vector */
/* float1x3, float3x3, float1x3, float4x4 == matrix */

/* VertexShader */
/* �����ϳ��� � ��ȯ�� ���İ��� �Ǵ����� ���� ������ ���� ����. */
/* ���� ���̴��� ���� �ϳ��� �޾ƿ���. ������ ��ģ�� ������ ��ģ ������ �������ش�. */
/* �簢�����۸� �׸���. 6��ȣ��. */
// VS_MAIN(/*����*/float3 vPosition  : POSITION, float2 vTexcoord : TEXCOORD0)
//{
//
//}

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexcoord : TEXCOORD0;	
	
};

struct VS_OUT
{
	/* SV_ : ShaderValue */
	/* ���� �ؾ��� ������ ��� ���ĳ��� ���̹Ƿ� ���� dx�� �߰������� �ؾ��� �̸��� �ض�. */
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
	
};

/* 1. ������ ��ȯ������ �����Ѵ�. */
/* 2. ������ ���� ������ �����Ѵ�. */
VS_OUT VS_MAIN(/*����*/VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	/* ������ ��ġ�� ���� �� ������ȯ�Ѵ�.*/		
	/* Ŭ���̾�Ʈ���� ���������� �����ϴ� TrnasformCoord�Լ��ʹ� �ٸ��� */
	/* mul�Լ��� ��쿡�� �����ϰ� ����� ���ϱ⸸ ������ �ϰ� w�����⿬����ü�� �������� �ʴ´�. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

/* Triangle : ���� ������ �� vs_main�� ����Ҷ����� ��� */
/* ������ ��δ� ����Ǹ�. ���� ������ ����. */
/* ���ϵ� ������ w�� ������ xyzw�� ������. ���� */
/* ������ ��ġ�� ����Ʈ�� ��ȯ��. (��������ǥ�� ��ȯ�Ѵ�)*/
/* �����Ͷ����� : ���������� ��������Ͽ� �ȼ��� ���������. */

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;	
};

struct PS_OUT
{
	vector vColor : SV_TARGET0;
};

vector Sample(float2 vTexcoord)
{
    return g_Texture.Sample(LinearSampler, vTexcoord);
}

/* 1. �ȼ��� �������� ������ �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
    Out.vColor = Sample(In.vTexcoord); /*vector(1.f, In.vTexcoord.y, 0.f, 1.f);*/
	
	if (0.1 >= Out.vColor.a)
		discard;

	return Out;
}



technique11	DefaultTechnique
{
	/* ������ + ������Ʈ + ssao + ��ָ��� + pbr*/
    pass UI
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}