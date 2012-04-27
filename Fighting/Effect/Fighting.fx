//==============================================================
// �萔

// ��{
float4x4 VProj;
float4 Diffuse;
texture MeshTex;
float4 Ambient;
float3 Light;

// �V���h�E
#define TEX_SIZE 2048
float4x4 World;
float4x4 WVProj;
float3 LightWPos;
float4x4 LightVProj;
float4 LightColor;
texture ShadowTex;
float Margin;

//==============================================================
// �e�N�X�`���T���v��
sampler MeshSmp=sampler_state {
	Texture=<MeshTex>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};
sampler ShadowSmp=sampler_state {
	Texture=<ShadowTex>;
	MipFilter=POINT;
	MinFilter=POINT;
	MagFilter=POINT;
	AddressU=CLAMP;
	AddressV=CLAMP;
};

//==============================================================
// �V�F�[�_�i��{�j

// ���_�V�F�[�_
void BasicVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float4 Col : COLOR0,
	out float2 MeshUV : TEXCOORD0
) {
	// ���_���W�A�e�N�X�`�����W
	Pos=mul(pos, VProj);
	MeshUV=mesh_uv;

	// ���_�F
	Col=min(1, max(0, dot(normal, -Light))+Ambient);
}

// �s�N�Z���V�F�[�_
float4 BasicPS(
	float4 Col : COLOR0,
	float2 MeshUV : TEXCOORD0
) : COLOR {
	return tex2D(MeshSmp, MeshUV)*Col*Diffuse;
}

//==============================================================
// �V�F�[�_�i�V���h�E�}�b�v�̍쐬�j

// ���_�V�F�[�_
void MakeShadowVS (
	float4 pos : POSITION,
	out float4 Pos : POSITION,
	out float2 Depth : TEXCOORD0
) {
	// ���_���W�����C�g�ɑ΂��Ďˉe�ϊ�
	Pos=mul(mul(pos, World), LightVProj);
	
	// �[�x
	Depth.xy=Pos.zw;
}

// �s�N�Z���V�F�[�_
float4 MakeShadowPS(
	float2 Depth : TEXCOORD0
) : COLOR {
	return Depth.x/Depth.y;
}

//==============================================================
// �V�F�[�_�i�V���h�E�̓K�p�A�X�e�[�W�j

// ���_�V�F�[�_
void ApplyShadowStageVS (
	float4 pos : POSITION,
	float2 mesh_uv : TEXCOORD0, 
	out float4 Pos : POSITION, 
	out float2 MeshUV : TEXCOORD0, 
	out float4 WPos : TEXCOORD1, 
	out float4 LVPPos : TEXCOORD3
) {
	// ���_���W�A�e�N�X�`�����W
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// ���_���W�Ɩ@�������[���h�ϊ�
	WPos=mul(pos, World);
	
	// ���_���W�����C�g�ɑ΂��Ďˉe�ϊ�
	LVPPos=mul(WPos, LightVProj);
}

// �s�N�Z���V�F�[�_
float4 ApplyShadowStagePS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float4 LVPPos : TEXCOORD3
) : COLOR {

	// �s�N�Z�������C�g�͈͓̔��ɂ���ꍇ
	float light_power=0;
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<=1 && abs(xy.y)<=1 && LVPPos.z>=0) {

		// ���C�g�ɑ΂��Ďˉe�ϊ��������_���W��
		// �V���h�E�}�b�v��̃e�N�X�`�����W�ɂ���
		float2 uv=xy*0.5f+0.5f;
		uv.y=1-uv.y;

		// �אڂ���4�e�N�Z���Ɋւ���
		// ���C�g�������邩�ǂ����𔻒肷��
		float z=LVPPos.z/LVPPos.w-Margin;
		float d=1.0f/TEX_SIZE;
		float t0, t1, t2, t3;
		t0=(tex2D(ShadowSmp, uv).r<z)?0:1;
		t1=(tex2D(ShadowSmp, uv+float2(d, 0)).r<z)?0:1;
		t2=(tex2D(ShadowSmp, uv+float2(0, d)).r<z)?0:1;
		t3=(tex2D(ShadowSmp, uv+float2(d, d)).r<z)?0:1;

		// ���茋�ʂ���`��Ԃ���
		float2 f=frac(uv*TEX_SIZE);
		light_power=lerp(
			lerp(t0, t1, f.x), 
			lerp(t2, t3, f.x), f.y);
	}

	// �s�N�Z���̐F
	float4 color=Ambient+(1-light_power)*LightColor;

	// ���b�V���e�N�X�`���Ƃ̍���
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

//==============================================================
// �V�F�[�_�i�V���h�E�̓K�p�A�v���C���[�j

// ���_�V�F�[�_
void ApplyShadowPlayerVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0, 
	out float4 Pos : POSITION, 
	out float2 MeshUV : TEXCOORD0, 
	out float4 WPos : TEXCOORD1, 
	out float3 WNormal : TEXCOORD2, 
	out float4 LVPPos : TEXCOORD3
) {
	// ���_���W�A�e�N�X�`�����W
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// ���_���W�Ɩ@�������[���h�ϊ�
	WPos=mul(pos, World);
	WNormal=normalize(mul(normal, (float3x3)World));
	
	// ���_���W�����C�g�ɑ΂��Ďˉe�ϊ�
	LVPPos=mul(WPos, LightVProj);
}

// �s�N�Z���V�F�[�_
float4 ApplyShadowPlayerPS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float3 WNormal : TEXCOORD2, 
	float4 LVPPos : TEXCOORD3
) : COLOR {

	// �s�N�Z�������C�g�͈͓̔��ɂ���ꍇ
	float light_power=1;
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<=1 && abs(xy.y)<=1 && LVPPos.z>=0) {

		// ���C�g�ɑ΂��Ďˉe�ϊ��������_���W��
		// �V���h�E�}�b�v��̃e�N�X�`�����W�ɂ���
		float2 uv=xy*0.5f+0.5f;
		uv.y=1-uv.y;

		// �אڂ���4�e�N�Z���Ɋւ���
		// ���C�g�������邩�ǂ����𔻒肷��
		float z=LVPPos.z/LVPPos.w-Margin;
		float d=1.0f/TEX_SIZE;
		float t0, t1, t2, t3;
		t0=(tex2D(ShadowSmp, uv).r<z)?0:1;
		t1=(tex2D(ShadowSmp, uv+float2(d, 0)).r<z)?0:1;
		t2=(tex2D(ShadowSmp, uv+float2(0, d)).r<z)?0:1;
		t3=(tex2D(ShadowSmp, uv+float2(d, d)).r<z)?0:1;

		// ���茋�ʂ���`��Ԃ���
		float2 f=frac(uv*TEX_SIZE);
		light_power=lerp(
			lerp(t0, t1, f.x), 
			lerp(t2, t3, f.x), f.y);
	}

	// �s�N�Z���̐F
	float4 color=Ambient+max(0, dot(WNormal, -Light))*light_power*LightColor;

	// ���b�V���e�N�X�`���Ƃ̍���
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

//==============================================================
// �e�N�j�b�N

technique Basic {
	pass P0 {
		VertexShader=compile vs_1_1 BasicVS();
		PixelShader=compile ps_2_0 BasicPS();
	}
}

technique MakeShadow {
	pass P0 {
		VertexShader=compile vs_1_1 MakeShadowVS();
		PixelShader=compile ps_2_0 MakeShadowPS();
	}
}

technique ApplyShadowStage {
	pass P0 {
		VertexShader=compile vs_1_1 ApplyShadowStageVS();
		PixelShader=compile ps_2_0 ApplyShadowStagePS();
	}
}

technique ApplyShadowPlayer {
	pass P0 {
		VertexShader=compile vs_1_1 ApplyShadowPlayerVS();
		PixelShader=compile ps_2_0 ApplyShadowPlayerPS();
	}
}
