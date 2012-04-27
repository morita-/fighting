#ifndef __CHWRESOURCE__H__
#define __CHWRESOURCE__H__

/**
 *  @brief	�e�v���b�g�t�H�[���ŗ��p����郊�\�[�X�Ǘ��ׂ̈̃N���X
 *		 �@	�v���b�g�t�H�[���w�ŗ��p
 */

//////////////////////////////////////////////////////////////
#include "CHWResourceData.h"

class CHWResource
{
protected:
	HWCommonResourceDataRef m_CommonData;
	HWResourceDataRef*		m_pMainData;

protected:
	CHWResource()
	{
		memset(&m_CommonData,0,sizeof(HWCommonResourceDataRef));
		m_pMainData = NULL;
	}
	virtual ~CHWResource(){}

public:
	const HWCommonResourceDataRef*	GetHWCommonResourceData(){
		return &m_CommonData;	
	}
	const HWResourceDataRef*		GetHWResourceData(){
		return m_pMainData;	
	}
};




//////////////////////////////////////////////////////////////
#include "..\LibGameWin32\CHWResource_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	typedef CHWResourceWin32	CHWResourceRef;
#else
	typedef CHWResource			CHWResourceRef;
#endif



#endif
