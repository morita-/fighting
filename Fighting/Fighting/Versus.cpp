#include "Main.h"
#include "Versus.h"


// コマンド
static const int 
	CMD5PG[]={INPUT_NOT2|INPUT_NOT_P|INPUT_NOT_G, INPUT_P|INPUT_G, INPUT_END|10},
	CMD_STAND_K[]={INPUT_NOT_K, INPUT_K, INPUT_END|60},
	CMD_D8P[]={INPUT_NOT_P, INPUT8|INPUT_P, INPUT_END|10},
	CMD3236P[]={INPUT3, INPUT2, INPUT3, INPUT_NOT3|INPUT6, INPUT_P, INPUT_END|20},
	CMD5PP[]={INPUT_NOT_P, INPUT_NOT2|INPUT_P, INPUT_NOT_P, INPUT_NOT2|INPUT_P, INPUT_END|20},
	CMD121[]={INPUT_NOT1, INPUT1, INPUT_NOT1|INPUT2, INPUT1, INPUT_END|15},
	CMD323[]={INPUT_NOT3, INPUT3, INPUT_NOT3|INPUT2, INPUT3, INPUT_END|15},
	CMD44[]={INPUT_NOT4, INPUT4, INPUT_NOT4, INPUT4, INPUT_END|15},
	CMD66[]={INPUT_NOT6, INPUT6, INPUT_NOT6, INPUT6, INPUT_END|15},
	CMD3K[]={INPUT_NOT_K, INPUT3|INPUT_K, INPUT_END|10},
	CMD2K[]={INPUT_NOT_K, INPUT2|INPUT_K, INPUT_END|10},
	CMD5K[]={INPUT_NOT_K, INPUT_K, INPUT_END|10},
	CMD2P[]={INPUT_NOT_P, INPUT2|INPUT_P, INPUT_END|10},
	CMD5P[]={INPUT_NOT_P, INPUT_P, INPUT_END|10},
	CMD2G[]={INPUT2|INPUT_G, INPUT_END|1},
	CMD5G[]={INPUT_G, INPUT_END|1},
	CMD2[]={INPUT2, INPUT_END|1},
	CMD4[]={INPUT4, INPUT_END|1},
	CMD6[]={INPUT6, INPUT_END|1},
	CMD5[]={INPUT_END|1};


MOTION Motion[]={
	{ANIM_121, CMD121}, 
	{ANIM_323, CMD323}, 
	{ANIM_44, CMD44},
	{ANIM_66, CMD66}, 
	{ANIM_3K, CMD3K}, 
	{ANIM_2K, CMD2K}, 
	{ANIM_5K, CMD5K}, 
	{ANIM_2P, CMD2P}, 
	{ANIM_5P, CMD5P}, 
	{ANIM_2G, CMD2G}, 
	{ANIM_5G, CMD5G}, 
	{ANIM_2, CMD2}, 
	{ANIM_4, CMD4}, 
	{ANIM_6, CMD6}, 
	{ANIM_5, CMD5}, 
};

// 当たり判定
struct HIT {
	char* FrameName;
	float Radius;
	int FrameIndex;
};
static HIT HitDefense[]={
	{"F185_ROOT", 0.25f},
	{"F2_CTRL_C_Back", 0.25f},
	{"F49_DEST_C_Head", 0.2f},
	{"F26_CTRL_L_Upperarm", 0.15f},
	{"F47_CTRL_R_Upperarm", 0.15f},
	{"F16_CTRL_L_Lowerarm", 0.15f}, 
	{"F37_CTRL_R_Lowerarm", 0.15f},
	{"F27_CTRL_L_Wrist", 0.15f},
	{"F48_CTRL_R_Wrist", 0.15f},
	{"F22_CTRL_L_Thigh", 0.2f},
	{"F43_CTRL_R_Thigh", 0.2f},
	{"F17_CTRL_L_Shin", 0.2f},
	{"F38_CTRL_R_Shin", 0.2f},
	{"F7_CTRL_L_Ankle", 0.2f},
	{"F28_CTRL_R_Ankle", 0.2f},
	{"F25_CTRL_L_Toe", 0.1f},
	{"F46_CTRL_R_Toe", 0.1f},
};
static HIT HitAttack[]={
	{"F27_CTRL_L_Wrist", 0.1f},
	{"F48_CTRL_R_Wrist", 0.1f},
	{"F7_CTRL_L_Ankle", 0.1f},
	{"F28_CTRL_R_Ankle", 0.1f},
	{"F14_CTRL_L_Knee", 0.1f},
	{"F35_CTRL_R_Knee", 0.1f},
};
static HIT HitAttackR[]={
	{"F48_CTRL_R_Wrist", 0.1f},
	{"F27_CTRL_L_Wrist", 0.1f},
	{"F28_CTRL_R_Ankle", 0.1f},
	{"F7_CTRL_L_Ankle", 0.1f},
	{"F35_CTRL_R_Knee", 0.1f},
	{"F14_CTRL_L_Knee", 0.1f},
};
static ATTACK Attack[]={
	{ANIM_3236P, AP_MIDDLE, HIT_P_L, 11, 18, 20, 0.3f},
	{ANIM_5PP, AP_HIGH, HIT_P_L, 7, 13, 10, 0.1f},
	{ANIM_5PP, AP_HIGH, HIT_P_R, 15, 21, 10, 0.1f},
	{ANIM_3K, AP_MIDDLE, HIT_K_L, 13, 18, 10, 0.2f},
	{ANIM_2K, AP_LOW, HIT_K_L, 15, 18, 10, 0.2f},
	{ANIM_5K, AP_HIGH, HIT_K_R, 11, 16, 20, 0.2f},
	{ANIM_2P, AP_LOW, HIT_P_L, 11, 14, 10, 0.1f},
	{ANIM_5P, AP_HIGH, HIT_P_L, 7, 13, 10, 0.1f},
	{ANIM_D8P, AP_DOWN, HIT_D8P_L, 27, 31, 30, 0.1f},
	{ANIM_D8P, AP_DOWN, HIT_D8P_R, 27, 31, 30, 0.1f},
	{ANIM_STANDUP5K, AP_HIGH, HIT_K_R, 52, 58, 20, 0.2f},
	{ANIM_STANDUP2K, AP_LOW, HIT_K_R, 50, 56, 20, 0.2f},
};

static DEFENSE Defense[ANIM_COUNT]={

	// 移動
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, -1}, 0},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, -1}, 0},
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, -1}, 0},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},

	// ガード、攻撃
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_GUARD2, -1, -1}, 0},
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, ANIM_BLOW20}, DA_COUNTER},
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, ANIM_BLOW20}, DA_COUNTER},
	{{ANIM_GUARD5, ANIM_GUARD5, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, ANIM_BLOW50}, DA_THROWABLE|DA_COUNTER},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, ANIM_BLOW50}, DA_THROWABLE|DA_COUNTER},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, ANIM_BLOW50}, DA_THROWABLE|DA_COUNTER},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, ANIM_BLOW50}, DA_THROWABLE|DA_COUNTER},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, ANIM_BLOW50}, DA_THROWABLE|DA_COUNTER},
	
	// ダメージ
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, -1}, DA_DAMAGE},
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, -1}, DA_DAMAGE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE|DA_DAMAGE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE|DA_DAMAGE},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE|DA_DAMAGE},

	// 吹き飛び、ダウン、ダウン攻撃
	{{ANIM_BLOW20, ANIM_BLOW20, ANIM_BLOW20, -1, -1}, DA_DAMAGE},
	{{ANIM_BLOW50, ANIM_BLOW50, ANIM_BLOW50, -1, -1}, DA_DAMAGE},
	{{-1, -1, -1, ANIM_DAMAGE_DOWN0, -1}, 0},
	{{-1, -1, -1, -1, -1}, 0},
	{{-1, -1, -1, -1, -1}, DA_DAMAGE},

	// ガード成功
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_GUARD2, -1, -1}, DA_GUARD},
	{{ANIM_GUARD5, ANIM_GUARD5, ANIM_DAMAGE5LOW0, -1, -1}, DA_THROWABLE|DA_GUARD},

	// 投げ
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, -1}, 0},
	{{-1, -1, -1, -1, -1}, 0},

	// 起きあがり、起きあがり攻撃
	{{-1, -1, -1, -1, -1}, 0},
	{{-1, ANIM_DAMAGE2MIDDLE0, ANIM_DAMAGE2LOW0, -1, ANIM_BLOW20}, DA_COUNTER},
	{{-1, -1, -1, -1, -1}, 0},
	{{ANIM_DAMAGE5HIGH0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5LOW0, -1, ANIM_BLOW50}, DA_COUNTER},
};

CPU_ACTION CPUActionNear[]={
	{1, CMD5PG, 0},
	{1, CMD3236P, 0},
	{1, CMD5PP, 0},
	{1, CMD3K, 0},
	{1, CMD2K, 0},
	{1, CMD5K, 0},
	{1, CMD2P, 0},
	{1, CMD5P, 0},
	{1, CMD121, 0},
	{1, CMD44, 0},
	{1, CMD4, 20},
	{2, CMD2, 20},
	{2, CMD5, 20},
	{3, CMD2G, 20},
	{3, CMD5G, 20},
};
CPU_ACTION CPUActionFar[]={
	{1, CMD5PP, 0},
	{1, CMD5K, 0},
	{1, CMD3K, 0},
	{1, CMD2, 20},
	{1, CMD5, 20},
	{2, CMD323, 0},
	{2, CMD66, 0},
	{2, CMD6, 20},	
	{3, CMD2G, 20},
	{3, CMD5G, 20},
};

// サウンド
CSfxPlayer*		gpSEPlayer = NULL;
CSfx			*gpSENormalHit=NULL, *gpSECounterHit=NULL, *gpSEDown=NULL;

// ゲージ
	
CGauge::CGauge(int index)
:	m_Index(index)
{
	m_WinCount=0;
	Reset();
}

void CGauge::Reset() {
	m_Value=m_PrevValue=0;
	m_VValue=1.0f/GAUGE_TIME;
	m_Wait=0;
}

void CGauge::Damage(float v) {
	if (m_Value>0 && m_VValue<=0) {
		m_Value-=v;
		if (m_Value<0) m_Value=0;
		m_VValue=(m_Value-m_PrevValue)/GAUGE_TIME;
		m_Wait=GAUGE_TIME;
	}
}

void CGauge::Move() {
	if (m_Wait>0) {
		m_Wait--;
	} else {
		if (m_VValue<0) {
			if (m_PrevValue>m_Value) {
				m_PrevValue+=m_VValue;
			} else {
				m_PrevValue=m_Value;
				m_VValue=0;
			}
		} else
		if (m_VValue>0) {
			if (m_Value<1) {
				m_Value+=m_VValue;
			} else {
				m_Value=m_PrevValue=1;
				m_VValue=0;
			}
		}
	}
}

void CGauge::Draw() {
	CTextureRef*  t=gpGame->GetDummyTexture();
	CGraphics* g=gpGame->GetGraphics();
	LPDIRECT3DDEVICE9 device=g->GetDevice();
	float 
		sw=g->GetWidth(), sh=g->GetHeight(),
		x=sw*0.44f, y=sw*0.04f,
		w=-sw*0.4f, h=sw*0.02f;
	if (m_Index==1) {
		x=sw-x;
		w=-w;
	}
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	t->Draw(x, y-h*0.05f, w, h*0.05f, 0, 0, 1, 1, D3DCOLOR_ARGB(100, 255, 255, 255));
	t->Draw(x, y+h, w, h*0.05f, 0, 0, 1, 1, D3DCOLOR_ARGB(100, 255, 255, 255));
	t->Draw(x, y, w, h, 0, 0, 1, 1, D3DCOLOR_ARGB(150, 0, 0, 0));
	if (m_PrevValue>m_Value) t->Draw(x+m_Value*w, y, (m_PrevValue-m_Value)*w, h, 0, 0, 1, 1, D3DCOLOR_ARGB(150, 200, 100, 80));
	t->Draw(x, y, m_Value*w, h, 0, 0, 1, 1, D3DCOLOR_ARGB(150, 120, 220, 100));
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	char buf[100];
	sprintf_s(buf, sizeof(buf), "%d WINS", m_WinCount);
	CFontRef* f=gpGame->GetFont();
	f->DrawText(buf, m_Index==0?x+w:x+w-f->GetTextW(buf), y-f->GetTextH()*1.1f, COL_WHITE, COL_BLACK);
}


// キャラクタ
// コンストラクタ
CPlayer::CPlayer(int id, CModel* model)
:	m_ID(id), m_pModel(model)
{

	m_nAttackValid=sizeof(Attack)/sizeof(ATTACK);
	m_pAttackValid=new bool[m_nAttackValid];
	memset(m_pAttackValid,0,sizeof(bool)*m_nAttackValid);

	m_Pos=D3DXVECTOR3(m_ID-0.5f, 0, 0);
	m_Yaw=(m_ID+0.5f)*D3DX_PI;
	m_AnimIndex=ANIM_5;
	m_pCenterFrame=m_pModel->FindFrame(m_ID==0?"F1_2P_Layer1":"F1_1P_Layer1");
	for (int i=0; i<INPUT_COUNT; i++) m_Input[i]=INPUT_NULL;
	m_bReverse=(m_ID==1);
	for (int j=0; j<m_nAttackValid; j++) m_pAttackValid[j]=true;
	m_bThrow=m_bDownAttack=false;
	m_NextAnimIndex=-1;
	m_WaitTime=0;
	m_bHalfBody=false;

	CModelAnimation* anim=GetAnimation();
	m_pModel->ResetAnimationWeight();
	anim->m_fWeight=1;
	anim->m_fTime=0;

	m_bCPU=false;
	m_CPUCommand=NULL;
	m_CPUCommandTime=0;
	m_CPURepeat=0;

	for (int i=0; i<sizeof(HitDefense)/sizeof(HIT); i++) {
		HitDefense[i].FrameIndex=m_pModel->FindFrame(HitDefense[i].FrameName)->m_Index;
	}
	for (int i=0; i<sizeof(HitAttack)/sizeof(HIT); i++) {
		HitAttack[i].FrameIndex=m_pModel->FindFrame(HitAttack[i].FrameName)->m_Index;
	}
	for (int i=0; i<sizeof(HitAttackR)/sizeof(HIT); i++) {
		HitAttackR[i].FrameIndex=m_pModel->FindFrame(HitAttackR[i].FrameName)->m_Index;
	}
}

// デストラクタ
CPlayer::~CPlayer()
{
	delete [] m_pAttackValid;
}

	
// 入力の判定
bool CPlayer::CheckInput(const int *command) {
	int i=0, j=0;
	while (!(command[i]&INPUT_END)) i++;
	int time=command[i]&~INPUT_END;
	for (i--; i>=0; i--) {
		int input=command[i];
		while (j<INPUT_COUNT && (m_Input[j]&input)!=input) j++;
		if (j>=time || j==INPUT_COUNT) return false;
	}
	return true;
}

// フレームの更新
void CPlayer::AnimateFrame() {
	D3DXMATRIXA16 world, trans;
	D3DXMatrixRotationY(&world, m_Yaw);
	D3DXMatrixTranslation(&trans, m_Pos.x, m_Pos.y, m_Pos.z);
	world*=trans;
	m_pModel->AnimateFrame(&world);
}

// 軸足
void CPlayer::CheckHalfBody() {
	CModelAnimation* anim=GetAnimation();
	if (
		(m_AnimIndex==ANIM_5PP && anim->m_fTime>=1700) ||
		(m_AnimIndex==ANIM_RUN && anim->m_fTime>=1200 && anim->m_fTime<=2600)
	) {
		m_bHalfBody=!m_bHalfBody;
	}
}

// 移動
void CPlayer::Move() {
	
	// 入力の更新
	const CInputState* is=gpGame->GetInput()->GetState(m_ID);
	int input=0;
	if (!m_bCPU) {
		input|=!is->Down&&!is->Up&&!is->Left&&!is->Right?INPUT5:INPUT_NOT5;
		input|=is->Down?INPUT2:INPUT_NOT2;
		input|=is->Up?INPUT8:INPUT_NOT8;
		input|=is->Button[0]?INPUT_G:INPUT_NOT_G;
		input|=is->Button[1]?INPUT_P:INPUT_NOT_P;
		input|=is->Button[2]?INPUT_K:INPUT_NOT_K;
		if (m_bReverse) {
			input|=is->Left?INPUT6:INPUT_NOT6;
			input|=is->Right?INPUT4:INPUT_NOT4;
			input|=is->Left&&is->Down?INPUT3:INPUT_NOT3;
			input|=is->Right&&is->Down?INPUT1:INPUT_NOT1;
			input|=is->Left&&is->Up?INPUT9:INPUT_NOT9;
			input|=is->Right&&is->Up?INPUT7:INPUT_NOT7;
		} else {
			input|=is->Left?INPUT4:INPUT_NOT4;
			input|=is->Right?INPUT6:INPUT_NOT6;
			input|=is->Left&&is->Down?INPUT1:INPUT_NOT1;
			input|=is->Right&&is->Down?INPUT3:INPUT_NOT3;
			input|=is->Left&&is->Up?INPUT7:INPUT_NOT7;
			input|=is->Right&&is->Up?INPUT9:INPUT_NOT9;
		}
	} else {
		if (m_CPUCommand==NULL) {
			if (m_pEnemy->m_AnimIndex==ANIM_DOWN0) {
				m_CPUCommand=CMD_D8P;
				m_CPURepeat=0;					
			} else
			if (m_AnimIndex==ANIM_DOWN0) {
				if (gpGame->m_Rand.Real1()<0.1f) {
					m_CPUCommand=gpGame->m_Rand.Real1()<0.5f?CMD5K:CMD2K;
				} else {
					m_CPUCommand=gpGame->m_Rand.Real1()<0.5f?CMD5:CMD2;
				}
				m_CPURepeat=0;
			} else {
				D3DXVECTOR3 v=m_pEnemy->m_Pos-m_Pos;
				float dist=D3DXVec3Length(&v);
				CPU_ACTION* action;
				int i, n, sum;
				if (dist<=MIN_DIST*1.5f) {
					action=CPUActionNear;
					n=sizeof(CPUActionNear)/sizeof(CPU_ACTION);
				} else {
					action=CPUActionFar;
					n=sizeof(CPUActionFar)/sizeof(CPU_ACTION);
				}
				for (i=0, sum=0; i<n; i++) sum+=action[i].Rate;
				int rate=(int)(gpGame->m_Rand.Real1()*sum);
				for (i=0, sum=0; i<n-1; i++) {
					sum+=action[i].Rate;
					if (rate<=sum) break;
				}
				m_CPUCommand=action[i].Command;
				m_CPURepeat=(int)(gpGame->m_Rand.Real1()*action[i].Repeat);
			}
			m_CPUCommandTime=0;
		} else {				
			if (m_CPUCommand[m_CPUCommandTime]&INPUT_END) {
				m_CPUCommandTime=0;
				if (m_CPURepeat>0) {
					m_CPURepeat--;
				} else {
					m_CPUCommand=NULL;
				}
			}
			if (m_CPUCommand && !(m_CPUCommand[m_CPUCommandTime]&INPUT_END)) {
				input|=m_CPUCommand[m_CPUCommandTime];
				m_CPUCommandTime++;
			}
		}
	}
	for (int i=INPUT_COUNT-1; i>0; i--) m_Input[i]=m_Input[i-1];
	m_Input[0]=input;
	
	// 動作の切り替え
	int index=m_AnimIndex;
	CModelAnimation* anim=GetAnimation();
	m_bThrow=m_bDownAttack=false;

	// ダメージとガード
	if (m_NextAnimIndex!=-1) {
		CheckHalfBody();
		anim->m_fTime=0;
		m_AnimIndex=m_NextAnimIndex;
		m_NextAnimIndex=-1;
	} else

	// ダウン攻撃
	if (m_AnimIndex==ANIM_D8P && anim->m_fTime<D8P_TIME*100) {
		m_Pos+=m_VPos;
	} else

	// 吹き飛び
	if (m_AnimIndex==ANIM_BLOW20 || m_AnimIndex==ANIM_BLOW50) {
		m_VPos+=m_APos;
		m_Pos+=m_VPos;
		if (m_Pos.y<=-1) {
			m_AnimIndex=ANIM_DOWN0;
			gpSEPlayer->Play(gpSEDown);
		}
	} else
	
	// 投げられ
	if (m_AnimIndex==ANIM_THROWN5PG && anim->IsOver()) {
		m_AnimIndex=ANIM_DOWN0;
		gpSEPlayer->Play(gpSEDown);
	} else

	// ダウン攻撃くらい
	if (m_AnimIndex==ANIM_DAMAGE_DOWN0 && anim->IsOver()) {
		m_AnimIndex=ANIM_DOWN0;
	} else

	// 起きあがり
	if (m_AnimIndex==ANIM_DOWN0 && anim->IsOver()) {
		if (CheckInput(CMD_STAND_K)) {
			if (CheckInput(CMD2)) {
				m_AnimIndex=ANIM_STANDUP2K;
			} else {
				m_AnimIndex=ANIM_STANDUP5K;
			}
		} else
		if (CheckInput(CMD2)) {
			m_AnimIndex=ANIM_STANDUP2;
		} else {
			m_AnimIndex=ANIM_STANDUP5;
		}
	} else

	// 3236P
	if (m_AnimIndex==ANIM_323 && CheckInput(CMD3236P)) {
		m_AnimIndex=ANIM_3236P;
	} else

	// 5PP
	if (m_AnimIndex==ANIM_5P && CheckInput(CMD5PP)) {
		m_AnimIndex=ANIM_5PP;
	} else

	// 走り
	if (m_AnimIndex==ANIM_66 && CheckInput(CMD6) && anim->m_fTime==1000) {
		m_AnimIndex=ANIM_RUN;
	} else

	// コマンド
	if (
		m_AnimIndex==ANIM_5 || m_AnimIndex==ANIM_2 || 
		m_AnimIndex==ANIM_4 || m_AnimIndex==ANIM_6 ||
		m_AnimIndex==ANIM_5G || m_AnimIndex==ANIM_2G ||
		(m_AnimIndex==ANIM_RUN && !CheckInput(CMD6)) ||
		(m_AnimIndex!=ANIM_RUN && anim->IsOver()) ||
		(anim->m_fTime>=m_WaitTime && (Defense[m_AnimIndex].Attribute&(DA_DAMAGE|DA_GUARD)))
	) {
		CheckHalfBody();
		for (int i=0; i<sizeof(Motion)/sizeof(MOTION); i++) {
			if (CheckInput(Motion[i].Command)) {
				m_AnimIndex=Motion[i].AnimIndex;
				for (int j=0; j<m_nAttackValid; j++) m_pAttackValid[j]=true;
				break;
			}
		}
		m_bDownAttack=CheckInput(CMD_D8P);
		m_bThrow=CheckInput(CMD5PG);
	}
	
	// アニメーションの進行
	CModelAnimation* new_anim=GetAnimation();
	if (anim!=new_anim) {
		
		// 補間
		m_pModel->SaveSmoother();
		m_pModel->m_fSmootherWeight=1;

		// 位置更新
		D3DXMATRIXA16& m=m_pCenterFrame->m_mCombinedMatrix;
		m_Pos=D3DXVECTOR3(m._41, m._42, m._43);

		// アニメーション切り替え
		m_pModel->ResetAnimationWeight();
		new_anim->m_fWeight=1;
		if (m_AnimIndex==ANIM_5PP) {
			new_anim->m_fTime=anim->m_fTime;
		} else {
			new_anim->m_fTime=0;
		}
		
		// 吹き飛び
		if (m_AnimIndex==ANIM_BLOW20 || m_AnimIndex==ANIM_BLOW50) {
			m_VPos=D3DXVECTOR3(cos(m_Yaw)*0.02f, 0.2f, sin(m_Yaw)*0.02f);
			m_APos=D3DXVECTOR3(0, -0.01f, 0);
		} else {
			m_Pos.y=0;
		}
		
		// ダウン攻撃
		if (m_AnimIndex==ANIM_D8P) {
			m_VPos=(m_pEnemy->m_Pos-m_Pos)/D8P_TIME;
			float v=D3DXVec3Length(&m_VPos);
			if (v>D8P_SPEED) m_VPos*=D8P_SPEED/v;
		}
	} else
	if (anim->IsOver()) {
		D3DXMATRIXA16& m=m_pCenterFrame->m_mCombinedMatrix;
		m_Pos=D3DXVECTOR3(m._41, m._42, m._43);
		anim->m_fTime=0;
	} else {
		m_pModel->m_fSmootherWeight-=WEIGHT_ATTENUATION;
		if (m_pModel->m_fSmootherWeight<0) m_pModel->m_fSmootherWeight=0;
		anim->m_fTime+=ANIM_TIME_STEP;
	}
	AnimateFrame();

	// 表情
	if (GetAnimation()->m_fTime>=5 && (Defense[m_AnimIndex].Attribute&DA_DAMAGE)) {
		m_pModel->m_Material[0]->m_pTexture=gpGame->GetFaceTexPain();
	} else {
		if (m_pModel->m_Material[0]->m_pTexture==gpGame->GetFaceTexNormal()) {
			if (gpGame->m_Rand.Real1()<0.01f) m_pModel->m_Material[0]->m_pTexture=gpGame->GetFaceTexClose();
		} else {
			if (gpGame->m_Rand.Real1()<0.1f) m_pModel->m_Material[0]->m_pTexture=gpGame->GetFaceTexNormal();
		}
	}		

	// ステージ端
	D3DXMATRIXA16& m=m_pCenterFrame->m_mCombinedMatrix;
	D3DXVECTOR3 pos=D3DXVECTOR3(m._41, m._42, m._43);
	if (pos.x<-STAGE_SIZE)	m_Pos.x+=-STAGE_SIZE-pos.x;
	if (pos.x>STAGE_SIZE)	m_Pos.x+=STAGE_SIZE-pos.x;
	if (pos.z<-STAGE_SIZE)	m_Pos.z+=-STAGE_SIZE-pos.z;
	if (pos.z>STAGE_SIZE)	m_Pos.z+=STAGE_SIZE-pos.z;
	AnimateFrame();
}
	
// 描画
void CPlayer::Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj) {
	m_pModel->AnimateVertex();
	if (gpGame->IsShaderEnabled()) {
		m_pModel->Draw(gpGame->GetEffect(), view, proj);
	} else {
		m_pModel->Draw(view, proj);
	}
}

// ゲーム本体（対戦）
// ゲームのイニシャライズ
void CFightingGameVersus::InitializeGame(){
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();
	m_pBall=new CMesh(device, m_ExePath+"..\\Model\\Ball.x", NULL);
	m_pStage=new CMesh(device, m_ExePath+"..\\Model\\Stage.x", NULL);
	m_HitMode=1;
	m_CPUMode=m_bCPU?1:0;
	m_bShowStage=true;
	m_bTopView=false;

	for (int i=0; i<2; i++) m_pPlayer[i]=new CPlayer(i, m_pModel[1-i]);
	for (int i=0; i<2; i++) m_pPlayer[i]->SetEnemy(m_pPlayer[1-i]);

	m_ViewFrom=D3DXVECTOR3(0, VIEW_FROM_Y, -VIEW_DIST);
	D3DXVECTOR3 lookat(0, VIEW_LOOKAT_Y, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&m_View, &m_ViewFrom, &lookat, &up);

	for (int i=0; i<2; i++) m_pGauge[i]=new CGauge(i);
	
	gpSEPlayer=new CSfxPlayer();
	gpSEPlayer->Initialize(m_pGraphics->GetHWnd(),DEF_SFX_DS_PRIO);
	gpSEPlayer->SetPrimaryBufferFormat(DEF_SFX_PRIM_CHANNELS,DEF_SFX_PRIM_FREQ,DEF_SFX_PRIM_BITRATE);
	gpSEPlayer->Set3DListener();

	gpSENormalHit=new CSfx(m_ExePath+"..\\Sound\\NormalHit.wav", NULL);
	gpSEPlayer->Download(gpSENormalHit);
	gpSECounterHit=new CSfx(m_ExePath+"..\\Sound\\CounterHit.wav", NULL);
	gpSEPlayer->Download(gpSECounterHit);
	gpSEDown=new CSfx(m_ExePath+"..\\Sound\\Down.wav", NULL);
	gpSEPlayer->Download(gpSEDown);
}

// デバイスリセット
void CFightingGameVersus::OnResetDevice() {
	CFightingGame::OnResetDevice();
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();
	
	// ライト
	D3DXVECTOR4 light(0, -1, 1, 0);
	D3DXVec4Normalize(&light, &light);
	m_pEffect->SetVector("Light", &light);
	m_pGraphics->SetLight(0, D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 1), D3DXCOLOR(1, 1, 1, 1), 1000);
}

// 当たり判定
bool CFightingGameVersus::CheckHit(int attack_player_id, int attack_hit_id) {
	HIT* hit_attack=(m_pPlayer[attack_player_id]->IsHalfBody()?HitAttackR:HitAttack)+attack_hit_id;
	D3DXMATRIXA16& am=m_pPlayer[attack_player_id]->GetModel()->GetFrame(hit_attack->FrameIndex)->m_mCombinedMatrix;
	D3DXVECTOR3 apos(am._41, am._42, am._43);
	for (int i=0, n=sizeof(HitDefense)/sizeof(HIT); i<n; i++) {
		D3DXMATRIXA16& dm=m_pPlayer[1-attack_player_id]->GetModel()->GetFrame(HitDefense[i].FrameIndex)->m_mCombinedMatrix;
		D3DXVECTOR3 dpos(dm._41, dm._42, dm._43);
		D3DXVECTOR3 dist=apos-dpos;
		if (D3DXVec3Length(&dist)<=hit_attack->Radius+HitDefense[i].Radius) return true;
	}
	return false;
}

// 移動
void CFightingGameVersus::Move() {
	m_pInput->UpdateState();

	// 一時停止・コマ送りなど
	const CInputState* is=m_pInput->GetState(0);
	if (!m_bPrevInput && is->Button[3]) m_bPaused=!m_bPaused;
	bool move=!m_bPaused||(!m_bPrevInput && is->Button[4]);
	if (!m_bPrevInput && is->Button[5]) m_HitMode=(m_HitMode+1)%3;
	if (!m_bPrevInput && is->Button[6]) m_CPUMode=(m_CPUMode+1)%4;
	if (!m_bPrevInput && is->Button[7]) m_bShaderEnabled=!m_bShaderEnabled;
	if (!m_bPrevInput && is->Button[8]) m_bShowStage=!m_bShowStage;
	if (!m_bPrevInput && is->Button[9]) m_bTopView=!m_bTopView;
	m_pPlayer[0]->SetCPU((m_CPUMode==2 || m_CPUMode==3));
	m_pPlayer[1]->SetCPU((m_CPUMode==1 || m_CPUMode==3));
	m_bPrevInput=false;
	for (int i=3; i<=9; i++) m_bPrevInput|=is->Button[i];
	if (!move) return;

	// プレイヤーの移動
	for (int i=0; i<2; i++) m_pPlayer[i]->Move();

	// 調整の準備
	D3DXMATRIXA16& m0=m_pPlayer[0]->GetCenterFrame()->m_mCombinedMatrix;
	D3DXMATRIXA16& m1=m_pPlayer[1]->GetCenterFrame()->m_mCombinedMatrix;
	D3DXVECTOR3 p0=D3DXVECTOR3(m0._41, 0, m0._43);
	D3DXVECTOR3 p1=D3DXVECTOR3(m1._41, 0, m1._43);
	D3DXVECTOR3 center=(p0+p1)*0.5f;
	D3DXVECTOR3 unit=p0-p1;
	float dist=D3DXVec3Length(&unit);
	D3DXVec3Normalize(&unit, &unit);
	
	// 攻撃と防御
	for (int i=0; i<2; i++) {
		CPlayer* player=m_pPlayer[i];
		CPlayer* enemy =m_pPlayer[1-i];
		DEFENSE* defense=&Defense[enemy->GetAnimIndex()];
		float time=player->GetAnimation()->m_fTime*0.01f;;
		for (int j=0, jn=sizeof(Attack)/sizeof(ATTACK); j<jn; j++) {
			ATTACK* attack=&Attack[j];
			if (
				player->IsAttackValid(j) &&
				attack->AnimIndex==player->GetAnimIndex() &&
				attack->FromTime<=time && time<=attack->ToTime &&
				CheckHit(i, attack->HitIndex)
			) {
				player->SetAttackValid(j,false);
				enemy->SetNextAnimIndex(defense->NextAnimIndex[attack->Position]);
				enemy->SetWaitFrame(attack->WaitTime*100);
				if (Defense[enemy->GetNextAnimIndex()].Attribute&DA_DAMAGE) m_pGauge[1-i]->Damage(attack->Damage);
				if (defense->Attribute&DA_COUNTER) {
					enemy->SetWaitFrame(enemy->GetWaitFrame()*1.5f);
					if (Defense[enemy->GetNextAnimIndex()].Attribute&DA_DAMAGE) m_pGauge[1-i]->Damage(attack->Damage*0.5f);
					if (player->GetAnimIndex()==ANIM_3236P || player->GetAnimIndex()==ANIM_5K) {
						enemy->SetNextAnimIndex(defense->NextAnimIndex[AP_BLOW]);
					}
					gpSEPlayer->Play(gpSECounterHit);
				} else {
					gpSEPlayer->Play(gpSENormalHit);						
				}
				break;
			}
		}
		if (
			player->IsThrow() && 
			dist<=MIN_DIST*1.1f && 
			(defense->Attribute&DA_THROWABLE) &&
			(
				!enemy->IsThrow() ||
				(defense->Attribute&DA_THROWABLE)==0
			)
		) {
			player->SetNextAnimIndex(ANIM_5PG);
			enemy->SetNextAnimIndex(ANIM_THROWN5PG);
			m_pGauge[1-i]->Damage(THROW_DAMAGE);
		}
		if (player->IsDownAttack() && enemy->GetAnimIndex()==ANIM_DOWN0) {
			player->SetNextAnimIndex(ANIM_D8P);
		}
	}

	// 間合いと角度の調整
	if (
		m_pPlayer[0]->GetAnimIndex()!=ANIM_D8P && 
		m_pPlayer[1]->GetAnimIndex()!=ANIM_D8P
	) {
		if (dist<MIN_DIST) {
			D3DXVECTOR3 vpos=unit*(MIN_DIST-dist)*0.5f;
			m_pPlayer[0]->m_Pos+=vpos;
			m_pPlayer[1]->m_Pos-=vpos;
		}
		for (int i=0; i<2; i++) {
			D3DXVECTOR3& a=m_pPlayer[i]->m_Pos;
			D3DXVECTOR3& b=m_pPlayer[1-i]->m_Pos;
			m_pPlayer[i]->SetYaw(atan2f(b.x-a.x, b.z-a.z));
			m_pPlayer[i]->AnimateFrame();
		}
	}

	// カメラ調整
	if (dist>=MIN_DIST*0.5f) {
		float view_dist=dist*VIEW_DIST*VIEW_DIST_RATE;
		if (view_dist<VIEW_DIST) view_dist=VIEW_DIST;
		D3DXVECTOR3 view_from[]={
			D3DXVECTOR3(center.x-unit.z*view_dist, VIEW_FROM_Y, center.z+unit.x*view_dist),
			D3DXVECTOR3(center.x+unit.z*view_dist, VIEW_FROM_Y, center.z-unit.x*view_dist)
		};
		float d[2];
		for (int i=0; i<2; i++) {
			D3DXVECTOR3 v=m_ViewFrom-view_from[i];
			d[i]=D3DXVec3Length(&v);
		}
		m_ViewFrom=view_from[d[0]<d[1]?0:1]; 
		D3DXVECTOR3 lookat(center.x, VIEW_LOOKAT_Y, center.z);
		D3DXVECTOR3 up(0, 1, 0);
		if (m_bTopView) {
			m_ViewFrom=D3DXVECTOR3(center.x, view_dist*1.2f, center.z);
			lookat=D3DXVECTOR3(center.x, 0, center.z);
			up=D3DXVECTOR3(unit.z, 0, -unit.x);
		}
		D3DXMatrixLookAtLH(&m_View, &m_ViewFrom, &lookat, &up);
	}
		
	// 入力方向の調整
	D3DXVECTOR4 v[2];
	for (int i=0; i<2; i++) {
		D3DXVec3Transform(&v[i], &m_pPlayer[i]->m_Pos, &m_View);
	}
	for (int i=0; i<2; i++) {
		m_pPlayer[i]->SetReverse((v[i].x>v[1-i].x));
	}
	
	// ゲージ
	for (int i=0; i<2; i++) {
		m_pGauge[i]->Move();
		if (m_pGauge[i]->GetValue()<=0 && m_pGauge[i]->GetPrevValue()<=0) {
			m_pGauge[1-i]->m_WinCount++;
			m_pGauge[i]->Reset();
		}
	}
}

// メッシュの描画
void CFightingGameVersus::DrawMesh(CMesh* mesh, ID3DXEffect* effect) {
	for (DWORD i=0, n=mesh->GetNumMaterials(); i<n; i++) {

		// 色、テクスチャ
		effect->SetVector("Diffuse", mesh->GetDiffuse(i));
		effect->SetTexture("MeshTex", mesh->GetTexture(i));
		
		// サブセットの描画
		UINT p, np;
		effect->Begin(&np, 0);
		for (p=0; p<np; p++) {
			effect->BeginPass(p);
			mesh->DrawSubset(i);
			effect->EndPass();
		}
		effect->End();
	}
}

// 描画
void CFightingGameVersus::Draw() {
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();
	m_pGraphics->Clear(COL_WHITE);

	// 射影行列
	D3DXMATRIXA16 proj;
	float w=m_pGraphics->GetWidth(), h=m_pGraphics->GetHeight();
	D3DXMatrixPerspectiveLH(&proj, 1, h/w, 1, 1000);

	// 行列の設定
	device->SetTransform(D3DTS_VIEW, &m_View);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	// シャドウ
	if (
		m_bShaderEnabled && m_pShadowTexture && m_pShadowDepthStencil
	) {

		// ワールド行列
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		m_pEffect->SetMatrix("World", &world);

		// ワールドビュー射影行列
		D3DXMATRIX wvproj=world*m_View*proj;
		m_pEffect->SetMatrix("WVProj", &wvproj);

		// ライトの位置
		D3DXMATRIXA16& m0=m_pPlayer[0]->GetCenterFrame()->m_mCombinedMatrix;
		D3DXMATRIXA16& m1=m_pPlayer[1]->GetCenterFrame()->m_mCombinedMatrix;
		D3DXVECTOR3 p0=D3DXVECTOR3(m0._41, 0, m0._43);
		D3DXVECTOR3 p1=D3DXVECTOR3(m1._41, 0, m1._43);
		D3DXVECTOR3 light_lookat=(p0+p1)*0.5f;
		D3DXVECTOR3 light_up(0, 1, 0);

		D3DXVECTOR3 unit=p0-p1;
		float dist=D3DXVec3Length(&unit)+4;

		D3DXVECTOR4 light_color(0.5f, 0.5f, 0.5f, 1);
		m_pEffect->SetVector("LightColor", &light_color);
		m_pEffect->SetTexture("ShadowTex", m_pShadowTexture);

		LPDIRECT3DSURFACE9 ort, ods;
		device->GetRenderTarget(0, &ort);
		device->GetDepthStencilSurface(&ods);

		LPDIRECT3DSURFACE9 rt;
		m_pShadowTexture->GetSurfaceLevel(0, &rt);

		// ステージの描画
		if (m_bShowStage) {
			device->SetRenderTarget(0, rt);
			device->SetDepthStencilSurface(m_pShadowDepthStencil);
			m_pGraphics->Clear();
			m_pEffect->SetTechnique("MakeShadow");
			D3DXVECTOR3 light_pos=light_lookat+D3DXVECTOR3(-5000, 5000, -5000);
			m_pEffect->SetValue("LightWPos", &light_pos, sizeof(D3DXVECTOR3));
			D3DXMATRIX light_view;
			D3DXMatrixLookAtLH(&light_view, &light_pos, &light_lookat, &light_up);
			D3DXMATRIX light_vproj;
			D3DXMatrixOrthoLH(&light_vproj, dist, dist, 1, 10000);
			light_vproj=light_view*light_vproj;
			m_pEffect->SetMatrix("LightVProj", &light_vproj);
			m_pEffect->SetFloat("Margin", m_bUseIntTexture?0.002f:0.001f);
			for (int i=0; i<2; i++) m_pPlayer[i]->Draw(&m_View, &proj);

			device->SetRenderTarget(0, ort);
			device->SetDepthStencilSurface(ods);
			m_pEffect->SetTechnique("ApplyShadowStage");
			DrawMesh(m_pStage, m_pEffect);
		}

		// キャラクタの描画
		{
			device->SetRenderTarget(0, rt);
			device->SetDepthStencilSurface(m_pShadowDepthStencil);
			m_pGraphics->Clear();
			m_pEffect->SetTechnique("MakeShadow");
			D3DXVECTOR3 light_pos=light_lookat+D3DXVECTOR3(-10, 10, -10);
			m_pEffect->SetValue("LightWPos", &light_pos, sizeof(D3DXVECTOR3));
			D3DXMATRIX light_view;
			D3DXMatrixLookAtLH(&light_view, &light_pos, &light_lookat, &light_up);
			D3DXMATRIX light_vproj;
			D3DXMatrixOrthoLH(&light_vproj, dist, dist, 1, 30);
			light_vproj=light_view*light_vproj;
			m_pEffect->SetMatrix("LightVProj", &light_vproj);
			m_pEffect->SetFloat("Margin", m_bUseIntTexture?0.002f:0.001f);
			for (int i=0; i<2; i++) m_pPlayer[i]->Draw(&m_View, &proj);

			device->SetRenderTarget(0, ort);
			device->SetDepthStencilSurface(ods);
			m_pEffect->SetTechnique("ApplyShadowPlayer");
			for (int i=0; i<2; i++) m_pPlayer[i]->Draw(&m_View, &proj);
		}

		rt->Release();
		ort->Release();
		ods->Release();
	} else {

		// ステージの描画
		if (m_bShowStage) {
			D3DXMATRIXA16 world;
			D3DXMatrixIdentity(&world);
			device->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_pStage->Draw(world);
			device->SetRenderState(D3DRS_LIGHTING, TRUE);
		}

		// キャラクタの描画
		m_pEffect->SetTechnique("Basic");
		for (int i=0; i<2; i++) m_pPlayer[i]->Draw(&m_View, &proj);

		// 当たり判定
		if (m_HitMode>0) {
			for (int i=0; i<2; i++) {
				HIT* hit;
				int n;
				if ((m_HitMode+i)%2==0) {
					hit=HitDefense;
					n=sizeof(HitDefense)/sizeof(HIT);
				} else {
					hit=HitAttack;
					n=sizeof(HitAttack)/sizeof(HIT);
				}
				for (int j=0; j<n; j++) {
					float r=hit[j].Radius;
					D3DXMATRIXA16 scale;
					D3DXMatrixScaling(&scale, r, r, r);
					D3DXMATRIXA16 world=scale*m_pPlayer[i]->GetModel()->GetFrame(hit[j].FrameIndex)->m_mCombinedMatrix;
					if (hit==HitAttack) {
						if (CheckHit(i, j)) {
							m_pBall->SetColor(D3DXCOLOR(0.2f, 0, 0, 0.8f), D3DXCOLOR(0, 0, 0, 0));
						} else {
							m_pBall->SetColor(D3DXCOLOR(0, 0.5f, 0, 0.6f), D3DXCOLOR(0, 0, 0, 0));
						}
					} else {
						m_pBall->SetColor(D3DXCOLOR(0, 0, 1, 0.4f), D3DXCOLOR(0, 0, 0, 0));
					}
					m_pBall->Draw(world);
				}
			}
		}
	}
	
	// ゲージ
	for (int i=0; i<2; i++) m_pGauge[i]->Draw();
}

// ファクトリ
CFightingGame* NewFightingGameVersus(bool cpu) {
	return new CFightingGameVersus(cpu);
}

