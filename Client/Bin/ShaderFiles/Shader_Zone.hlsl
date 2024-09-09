#include "Shader_Engine_Defines.hlsli"

matrix g_WorldMatrix;
matrix g_ViewMatrix = matrix(1.f, 0.f, 0.f, 0.f,
								0.f, 1.f, 0.f, 0.f,
								0.f, 0.f, 1.f, 0.f,
								0.f, 0.f, 0.f, 1.f);
matrix g_ProjMatrix;
texture2D		g_Texture;
float			g_Percent; //원일경우 그냥 각도로 환산해서 보내셈
float2 Center = float2(0.5f, 0.5f);

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexcoord : TEXCOORD0;
	
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
	
};

VS_OUT VS_MAIN(/*정점*/VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;	
};

struct PS_OUT
{
	vector vColor : SV_TARGET0;
};

/* 1. 픽셀의 최종적인 색상을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
    int result; //색칠할거면1, 아니면 0.5 비중 넣어서 흐리게
	
	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (g_Percent < In.vTexcoord.x)
        Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
	
    if (0.1 >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_SQUARE(PS_IN In) // 비율이 50퍼가 안됨
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (In.vTexcoord.x <= 0.5f) //50퍼 이상은 색칠하고 감
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
        return Out;
    }
    
    if (g_Percent > radians(90.f) - atan((In.vTexcoord.y * (-2) + 1) / (In.vTexcoord.x * 2 - 1)))
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord) * 0.5f;
    }
	else
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    }
    if (0.1 >= Out.vColor.a)
        discard;

    //Out.vColor = g_Texture.Sample(LinearClampSampler, In.vTexcoord) * 0.5f;
	
    return Out;
}

PS_OUT PS_MAIN_NEXTSQUARE(PS_IN In) // 비율이 50퍼를 넘음
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (In.vTexcoord.x > 0.5f) //50퍼가 안되는 반은 일단 어둡게
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord) * 0.5f;
        return Out;
    }
    
    
    if (g_Percent - radians(180.f) > radians(90.f) + atan((In.vTexcoord.y * (-2) + 1) / ((1.f - In.vTexcoord.x) * 2 - 1)))
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord) * 0.5f;
    }
    else
    {
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    }
    if (0.1 >= Out.vColor.a)
        discard;

    //Out.vColor = g_Texture.Sample(LinearClampSampler, In.vTexcoord) * 0.5f;
	
    return Out;
}


technique11	DefaultTechnique
{
	pass UI
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    Pass Circle
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_SQUARE();
    }

    Pass NextCircle
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_NEXTSQUARE();
    }
}