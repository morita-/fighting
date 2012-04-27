#ifndef __CPAD__H__
#define __CPAD__H__

/** //////////////////////////////////////////////////////////////
 *  @brief	入力インターフェース用クラス
 *		 　	アプリケーション層
 *
 */ //////////////////////////////////////////////////////////////


#include "PadType.h"

class CPadControl;

//////////////////////////////////////////////////////////////
///////////////////
///パッドインターフェースクラス：個別インターフェース基底クラス
class CPadIF
{
	PadType		m_nPadType;
	mS32		m_nNo;

public:
	CPadIF(mS32 no,PadType type){
		m_nNo		= no;
		m_nPadType	= type;
	}
	virtual ~CPadIF(){}

	virtual void CreateDevice(){}
	virtual void ReleaseDevice(){}
	virtual void Update(){}

	virtual PadState GetPadState(){return PAD_DISCONNECT;}

	virtual PadData* GetPadData(){return NULL;}
	virtual KeyData* GetKeyData(){return NULL;}
	virtual MusData* GetMusData(){return NULL;}
};

//////////////////////////////////////////////////////////////
///////////////////
///パッド用基底クラス
class CPadData :public CPadIF
{
protected:
	PadData m_PadData;
public:
	CPadData(CPadControl* pCtrl,mS32 no,PadType type);
	~CPadData();

	virtual void Create(){}
	virtual void Release(){}
	virtual void Update(){}


	virtual PadData* GetPadData(){return &m_PadData;}
};

//////////////////////////////////////////////////////////////
///////////////////
///キーボード用基底クラス
class CKeyData :public CPadIF
{
protected:
	KeyData m_KeyData;
public:
	CKeyData(CPadControl* pCtrl,mS32 no,PadType type);
	~CKeyData();

	virtual void Create(){}
	virtual void Release(){}
	virtual void Update(){}


	virtual KeyData* GetKeyData(){return &m_KeyData;}
};

//////////////////////////////////////////////////////////////
///////////////////
///マウス用基底クラス
class CMusData :public CPadIF
{
protected:
	MusData m_MusData;
public:
	CMusData(CPadControl* pCtrl,mS32 no,PadType type);
	~CMusData();

	virtual void Create(){}
	virtual void Release(){}
	virtual void Update(){}


	virtual MusData* GetMusData(){return &m_MusData;}
};


//////////////////////////////////////////////////////////////
///////////////////
///インターフェース共通基底処理
class CPadCommon
{
public:
	CPadCommon(){}
	virtual ~CPadCommon(){}

	virtual void CreateDevice(){}
	virtual void ReleaseDevice(){}
	virtual void Update(){}
};


//////////////////////////////////////////////////////////////
#include "..\LibGameWin32\CPad_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	
	typedef CPadCommonWin32 CPadCommonRef;
	
	typedef CPadDataWin32 CPadDataRef;
	typedef CKeyDataWin32 CKeyDataRef;
	typedef CMusDataWin32 CMusDataRef;
	
#else
	
	typedef CPadCommon CPadCommonRef;
	
	typedef CPadData CPadDataRef;
	typedef CKeyData CKeyDataRef;
	typedef CMusData CMusDataRef;
	
#endif


//////////////////////////////////////////////////////////////
///////////////////
///パッドコントロールクラス
class CPadControl : public CPadCommonRef
{
	mS32		m_nMaxPort;
	CPadIF**	m_pPad;

	void Create(mS32 n,PadType* type);
	void Release(mS32 n);
public:
	CPadControl(mS32 max_port,PadType* type);
	~CPadControl();

	void Update();
};


#endif
