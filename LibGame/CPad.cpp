
#include "global.h"
#include "CPad.h"

///////////////////
///パッドクラス
CPadData::CPadData(CPadControl* pCtrl,mS32 no,PadType type) : CPadIF(no,type){
	memset(&m_PadData,0,sizeof(PadData));
}

CPadData::~CPadData(){
}




///////////////////
///キークラス
CKeyData::CKeyData(CPadControl* pCtrl,mS32 no,PadType type) : CPadIF(no,type){
	memset(&m_KeyData,0,sizeof(KeyData));
}

CKeyData::~CKeyData(){
}




///////////////////
///マウスクラス
CMusData::CMusData(CPadControl* pCtrl,mS32 no,PadType type) : CPadIF(no,type){
	memset(&m_MusData,0,sizeof(MusData));
}

CMusData::~CMusData(){
}



///////////////////
///パッドコントロールクラス
CPadControl::CPadControl(mS32 max_port,PadType* type){
	m_nMaxPort = max_port;
	m_pPad = new CPadIF*[max_port];
	memset(m_pPad,0,sizeof(CPadIF*));

	mS32 n;
	for(n=0;n<max_port;n++){
		Create(n,type);
	}
}
CPadControl::~CPadControl(){
	mS32 n;
	for(n=0;n<m_nMaxPort;n++){
		Release(n);
	}
	delete [] m_pPad;
	m_pPad=NULL;
	m_nMaxPort=0;
}



void CPadControl::Create(mS32 n,PadType* type){
	switch(type[n]){
	case ePadType_Pad:		m_pPad[n] = new CPadData(this,n,type[n]);break;//パッド
	case ePadType_Key:		m_pPad[n] = new CKeyData(this,n,type[n]);break;//キーボード
	case ePadType_Mouse:	m_pPad[n] = new CMusData(this,n,type[n]);break;//マウス
	default:break;
	};
}

void CPadControl::Release(mS32 n)
{
	delete m_pPad[n];
};
