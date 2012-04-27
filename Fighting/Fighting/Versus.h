#ifndef __VERSUS__H__
#define __VERSUS__H__

#define MIN_DIST 1
#define INPUT_COUNT 60
#define VIEW_FROM_Y 2
#define VIEW_LOOKAT_Y 1
#define VIEW_DIST 4
#define VIEW_DIST_RATE 0.3f
#define WEIGHT_ATTENUATION 0.2f
#define STAGE_SIZE 9.5f

#define DAMAGE_TIME 2000
#define GUARD_TIME 2000
#define D8P_SPEED 0.1f
#define D8P_TIME 30
#define THROW_DAMAGE 0.3f
#define GAUGE_TIME 30

// 入力
#define INPUT1 0x1
#define INPUT2 0x2
#define INPUT3 0x4
#define INPUT4 0x8
#define INPUT5 0x10
#define INPUT6 0x20
#define INPUT7 0x40
#define INPUT8 0x80
#define INPUT9 0x100
#define INPUT_G 0x200
#define INPUT_P 0x400
#define INPUT_K 0x800

#define INPUT_NOT1 0x1000
#define INPUT_NOT2 0x2000
#define INPUT_NOT3 0x4000
#define INPUT_NOT4 0x8000
#define INPUT_NOT5 0x10000
#define INPUT_NOT6 0x20000
#define INPUT_NOT7 0x40000
#define INPUT_NOT8 0x80000
#define INPUT_NOT9 0x100000
#define INPUT_NOT_G 0x200000
#define INPUT_NOT_P 0x400000
#define INPUT_NOT_K 0x800000

#define INPUT_NULL 0x40000000
#define INPUT_END 0x80000000

enum {
	HIT_P_L,
	HIT_P_R,
	HIT_K_L,
	HIT_K_R,
	HIT_D8P_L,
	HIT_D8P_R,
};

struct MOTION {
	int AnimIndex;
	const int* Command;
};

// 攻撃と防御
enum ATTACK_POSITION {
	AP_HIGH, AP_MIDDLE, AP_LOW, AP_DOWN, AP_BLOW, AP_COUNT
};
struct ATTACK {
	int AnimIndex;
	int Position;
	int HitIndex;
	int FromTime;
	int ToTime;
	int WaitTime;
	float Damage;
};


// 防御の結果
enum DEFENSE_ATTRIBUTE {
	DA_THROWABLE=1, 
	DA_COUNTER=2, 
	DA_DAMAGE=4,
	DA_GUARD=8,
};
struct DEFENSE {
	int NextAnimIndex[AP_COUNT];
	int Attribute;
	int CounterAnimIndex;
};

// CPU
struct CPU_ACTION {
	float Rate;
	const int* Command;
	int Repeat;
};

// ゲージ
class CGauge {
protected:
	float m_Value;
	float m_PrevValue;
	float m_VValue;
	int m_Index;
	int m_Wait;

public:
	int m_WinCount;
	
public:
	void Reset();
	
	CGauge(int index);
	
	void Damage(float v);
	
	void Move();
	
	void Draw();


	float GetValue(){return m_Value;}
	float GetPrevValue(){return m_PrevValue;}
};

// キャラクタ
class CPlayer {
protected:
	int				m_ID;
	D3DXVECTOR3		m_VPos, m_APos;
	float			m_Yaw;
	int				m_AnimIndex;
	CModel*			m_pModel;
	CModelFrame*	m_pCenterFrame;
	int				m_Input[INPUT_COUNT];
	bool			m_bReverse;
	bool			m_bThrow;
	bool			m_bDownAttack;
	int				m_NextAnimIndex;
	CPlayer*		m_pEnemy;
	int				m_WaitTime;

	bool*		m_pAttackValid;
	int			m_nAttackValid;

	bool		m_bCPU;
	const int*	m_CPUCommand;
	int			m_CPUCommandTime;
	int			m_CPURepeat;

	bool		m_bHalfBody;

public:
	D3DXVECTOR3		m_Pos;

public:
	// コンストラクタ
	CPlayer(int id, CModel* model);
	// デストラクタ
	~CPlayer();
	
	// 入力の判定
	bool CheckInput(const int *command);
	
	// フレームの更新
	void AnimateFrame();

	// 軸足
	void CheckHalfBody();

	// 移動
	void Move();
		
	// 描画
	void Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj);


	// アニメーションの取得
	CModelAnimation* GetAnimation() {
		return m_pModel->GetAnimation(m_AnimIndex+(m_bHalfBody?ANIM_COUNT:0));
	}

	//
	CModelFrame* GetCenterFrame(){return m_pCenterFrame;}

	//
	int			GetAnimIndex(){return m_AnimIndex;}
	int			GetNextAnimIndex(){return m_NextAnimIndex;}
	void		SetNextAnimIndex(int idx){m_NextAnimIndex=idx;}
	bool		IsAttackValid(int no){return  (no<m_nAttackValid)? m_pAttackValid[no]:false;}
	void		SetAttackValid(int no,bool sw){if(no<m_nAttackValid)m_pAttackValid[no]=sw;}

	int			GetWaitFrame(){return m_WaitTime;}
	void		SetWaitFrame(int cnt){m_WaitTime=cnt;}

	bool		IsThrow(){return m_bThrow;}
	bool		IsDownAttack(){return m_bDownAttack;}
	void		SetReverse(bool sw){m_bReverse=sw;}

	void		SetYaw(float yaw){m_Yaw=yaw;}

	//
	CPlayer*	GetEnemy(){return m_pEnemy;}
	void		SetEnemy(CPlayer* p){m_pEnemy=p;}

	bool		IsHalfBody(){return m_bHalfBody;}

	CModel*		GetModel(){return m_pModel;}

	void		SetCPU(bool cpu){ m_bCPU = cpu;}


};

// ゲーム本体（対戦）
class CFightingGameVersus : public CFightingGame {
protected:
	CPlayer*		m_pPlayer[2];
	D3DXMATRIXA16	m_View;
	D3DXVECTOR3		m_ViewFrom;
	CMesh* m_pBall;
	CMesh* m_pStage;
	int m_HitMode;
	int m_CPUMode;
	CGauge* m_pGauge[2];
	bool m_bShowStage;
	bool m_bTopView;
	bool m_bCPU;

public:
	// コンストラクタ
	CFightingGameVersus(bool cpu) {
		m_bCPU = cpu;
	}

	// ゲームのイニシャライズ
	void InitializeGame();

	// デバイスリセット
	void OnResetDevice();

	// 当たり判定
	bool CheckHit(int attack_player_id, int attack_hit_id);

	virtual void Move();
	
	void DrawMesh(CMesh* mesh, ID3DXEffect* effect);

	virtual void Draw();



};


#endif