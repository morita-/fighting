//==============================================================
// 定数

// 基本
float4x4 VProj;
float4 Diffuse;
texture MeshTex;
float4 Ambient;
float3 Light;

// シャドウ
#define TEX_SIZE 2048
float4x4 World;
float4x4 WVProj;
float3 LightWPos;
float4x4 LightVProj;
float4 LightColor;
texture ShadowTex;
float Margin;

//==============================================================
// テクスチャサンプラ
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
// シェーダ（基本）

// 頂点シェーダ
void BasicVS (
	float4 pos : POSITION,
	float3 normal : NORMAL,
	float2 mesh_uv : TEXCOORD0,
	out float4 Pos : POSITION,
	out float4 Col : COLOR0,
	out float2 MeshUV : TEXCOORD0
) {
	// 頂点座標、テクスチャ座標
	Pos=mul(pos, VProj);
	MeshUV=mesh_uv;

	// 頂点色
	Col=min(1, max(0, dot(normal, -Light))+Ambient);
}

// ピクセルシェーダ
float4 BasicPS(
	float4 Col : COLOR0,
	float2 MeshUV : TEXCOORD0
) : COLOR {
	return tex2D(MeshSmp, MeshUV)*Col*Diffuse;
}

//==============================================================
// シェーダ（シャドウマップの作成）

// 頂点シェーダ
void MakeShadowVS (
	float4 pos : POSITION,
	out float4 Pos : POSITION,
	out float2 Depth : TEXCOORD0
) {
	// 頂点座標をライトに対して射影変換
	Pos=mul(mul(pos, World), LightVProj);
	
	// 深度
	Depth.xy=Pos.zw;
}

// ピクセルシェーダ
float4 MakeShadowPS(
	float2 Depth : TEXCOORD0
) : COLOR {
	return Depth.x/Depth.y;
}

//==============================================================
// シェーダ（シャドウの適用、ステージ）

// 頂点シェーダ
void ApplyShadowStageVS (
	float4 pos : POSITION,
	float2 mesh_uv : TEXCOORD0, 
	out float4 Pos : POSITION, 
	out float2 MeshUV : TEXCOORD0, 
	out float4 WPos : TEXCOORD1, 
	out float4 LVPPos : TEXCOORD3
) {
	// 頂点座標、テクスチャ座標
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// 頂点座標と法線をワールド変換
	WPos=mul(pos, World);
	
	// 頂点座標をライトに対して射影変換
	LVPPos=mul(WPos, LightVProj);
}

// ピクセルシェーダ
float4 ApplyShadowStagePS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float4 LVPPos : TEXCOORD3
) : COLOR {

	// ピクセルがライトの範囲内にある場合
	float light_power=0;
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<=1 && abs(xy.y)<=1 && LVPPos.z>=0) {

		// ライトに対して射影変換した頂点座標を
		// シャドウマップ上のテクスチャ座標にする
		float2 uv=xy*0.5f+0.5f;
		uv.y=1-uv.y;

		// 隣接する4テクセルに関して
		// ライトが当たるかどうかを判定する
		float z=LVPPos.z/LVPPos.w-Margin;
		float d=1.0f/TEX_SIZE;
		float t0, t1, t2, t3;
		t0=(tex2D(ShadowSmp, uv).r<z)?0:1;
		t1=(tex2D(ShadowSmp, uv+float2(d, 0)).r<z)?0:1;
		t2=(tex2D(ShadowSmp, uv+float2(0, d)).r<z)?0:1;
		t3=(tex2D(ShadowSmp, uv+float2(d, d)).r<z)?0:1;

		// 判定結果を線形補間する
		float2 f=frac(uv*TEX_SIZE);
		light_power=lerp(
			lerp(t0, t1, f.x), 
			lerp(t2, t3, f.x), f.y);
	}

	// ピクセルの色
	float4 color=Ambient+(1-light_power)*LightColor;

	// メッシュテクスチャとの合成
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

//==============================================================
// シェーダ（シャドウの適用、プレイヤー）

// 頂点シェーダ
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
	// 頂点座標、テクスチャ座標
	Pos=mul(pos, WVProj);
	MeshUV=mesh_uv;

	// 頂点座標と法線をワールド変換
	WPos=mul(pos, World);
	WNormal=normalize(mul(normal, (float3x3)World));
	
	// 頂点座標をライトに対して射影変換
	LVPPos=mul(WPos, LightVProj);
}

// ピクセルシェーダ
float4 ApplyShadowPlayerPS(
	float2 MeshUV : TEXCOORD0,
	float4 WPos : TEXCOORD1, 
	float3 WNormal : TEXCOORD2, 
	float4 LVPPos : TEXCOORD3
) : COLOR {

	// ピクセルがライトの範囲内にある場合
	float light_power=1;
	float2 xy=LVPPos.xy/LVPPos.w;
	if (abs(xy.x)<=1 && abs(xy.y)<=1 && LVPPos.z>=0) {

		// ライトに対して射影変換した頂点座標を
		// シャドウマップ上のテクスチャ座標にする
		float2 uv=xy*0.5f+0.5f;
		uv.y=1-uv.y;

		// 隣接する4テクセルに関して
		// ライトが当たるかどうかを判定する
		float z=LVPPos.z/LVPPos.w-Margin;
		float d=1.0f/TEX_SIZE;
		float t0, t1, t2, t3;
		t0=(tex2D(ShadowSmp, uv).r<z)?0:1;
		t1=(tex2D(ShadowSmp, uv+float2(d, 0)).r<z)?0:1;
		t2=(tex2D(ShadowSmp, uv+float2(0, d)).r<z)?0:1;
		t3=(tex2D(ShadowSmp, uv+float2(d, d)).r<z)?0:1;

		// 判定結果を線形補間する
		float2 f=frac(uv*TEX_SIZE);
		light_power=lerp(
			lerp(t0, t1, f.x), 
			lerp(t2, t3, f.x), f.y);
	}

	// ピクセルの色
	float4 color=Ambient+max(0, dot(WNormal, -Light))*light_power*LightColor;

	// メッシュテクスチャとの合成
	return float4((tex2D(MeshSmp, MeshUV)*color*Diffuse).rgb, Diffuse.a);
}

//==============================================================
// テクニック

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
