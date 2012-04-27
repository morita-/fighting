
#include "..\LibGame\global.h"

#include "..\LibGame\CHWResource.h"
#include "CHWResource_win32.h"

INSTANCE_DEFINED(CHWResourceWin32)

CHWResourceWin32::CHWResourceWin32(){
	m_pData = NULL;
	m_nData = 0;
}

CHWResourceWin32::~CHWResourceWin32(){
	M_DELETE_ARRAY( m_pData );
	m_nData = 0;
}


HWResourceDataWin32*	CHWResourceWin32::AddResource(HWND hWnd){
	
	HWResourceDataWin32* pBuf = new HWResourceDataWin32[m_nData+1];

	memcpy(pBuf,m_pData,sizeof(HWResourceDataWin32)*m_nData);
	memset(&pBuf[m_nData],0,sizeof(HWResourceDataWin32));
	pBuf[m_nData].m_HWnd = hWnd;

	M_DELETE_ARRAY( m_pData );
	m_pData = pBuf;
	m_nData ++;

	return &m_pData[m_nData-1];
}

void					CHWResourceWin32::ReleaseResourceNo(mS32 no){
	if(no<0||no>=m_nData)return;

	if(m_nData<=1){
		M_DELETE_ARRAY( m_pData );
		return;
	}

	HWResourceDataWin32* pBuf = new HWResourceDataWin32[m_nData-1];
	if(no>0){
		memcpy(pBuf,m_pData,sizeof(HWResourceDataWin32)*(no-1 +1));
	}
	if((no+1)<m_nData){
		memcpy(&pBuf[no],&m_pData[no+1],sizeof(HWResourceDataWin32)*(m_nData-(no+1)));
	}
	M_DELETE_ARRAY( m_pData );
	m_pData = pBuf;
	--m_nData;

}

HWResourceDataWin32*	CHWResourceWin32::GetResource(HWND hWnd){
	mS32 i;
	for(i=0;i<m_nData;i++){
		if(m_pData[i].m_HWnd == hWnd){
			return &m_pData[i];
		}
	}
	return NULL;
}


