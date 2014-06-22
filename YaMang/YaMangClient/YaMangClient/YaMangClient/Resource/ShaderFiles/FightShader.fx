// ��ȯ���
float4x4    matWVP;
float		time;
bool		isEnemy;
bool		isAttack;
bool		isSelect;

// �ؽ�ó
texture	tex0;

// �Է����� ����
struct VS_INPUT
{
	float3 pos  : POSITION;
	float2 tex  : TEXCOORD0;
};

// ������� ����
struct VS_OUTPUT
{
	float4 pos  : POSITION;
	float2 tex  : TEXCOORD0;
};

// VS��� �������̴� �Լ� ����
VS_OUTPUT VS( VS_INPUT In )
{
	// ��� ���� �ʱ�ȭ
	VS_OUTPUT Out = (VS_OUTPUT)0;

	if ( isAttack == true )
	{
		In.pos = In.pos + In.pos * ( sin( time ) + 1 ) / 40;
	}
	Out.pos = mul( float4( In.pos, 1 ), matWVP );
	Out.tex = In.tex;

	return Out;
}

// �ؽ�ó ���÷� ����, 
sampler Sampler = sampler_state
{
	Texture = ( tex0 ); // g_pd3dDevice->SetTexture( 0, g_pTexture );
};

float4 PS(
	float2 Tex : TEXCOORD0 ): COLOR
{
	float4 color = 0.0f;
	color.a = 1.0f;
	
	if ( isAttack == true )
	{
		color.r = 0.9f;
		color.gb = 0.3f;
		color = color * ( ( sin( time ) + 1 ) / 2 );
	}
	if ( isEnemy == true )
	{
		color.b = 0.5f;
		color.rg = 0.0f;
	}
	if ( isSelect == true )
	{
		color.b = 0.7f;
		color.rg = 0.3f;
	}
	else
	{
		color.r = 0.7f;
		color.gb = 0.3f;
	}

	return tex2D( Sampler, Tex ) / 3 + color;
}

// MyShader ��ũ�м���
technique MyShader
{
	pass P0	// ������ 0��° �н�
	{
		Lighting = FALSE;

		// samplers
		Sampler[0] = ( Sampler );

		// shaders
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}