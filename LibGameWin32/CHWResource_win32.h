#ifndef __CHWRESOURCE_WIN32__H__
#define __CHWRESOURCE_WIN32__H__

#include "..\LibGame\platform.h"
#if	PLATFORM_CHECK( PLATFORM_WIN32 )

class CHWResource;
class CHWResourceWin32 : public CHWResource
{
public:
	INSTANCE_METHOD( CHWResourceWin32 )


protected:
	CHWResourceWin32();
	~CHWResourceWin32();

	HWResourceDataWin32* m_pData;
	mS32				 m_nData;

	HWResourceDataWin32* AddResource(HWND hWnd);
	void ReleaseResourceNo(mS32 no);

	HWResourceDataWin32* GetResource(HWND hWnd);

public:
	void SetHWResourceInstance(HINSTANCE hInst){
		m_CommonData.m_hInstance = hInst;
	}
	HINSTANCE GetHWResourceInstance(){
		return m_CommonData.m_hInstance;
	}

	void SetHWResourceAccel(HACCEL hAccel){
		m_CommonData.m_HAccel = hAccel;
	}
	HACCEL GetHWResourceAccel(){
		return m_CommonData.m_HAccel;
	}

	void SetHWResourceD3D(LPDIRECT3D9 pD3D){
		m_CommonData.m_pD3D = pD3D;
	}
	LPDIRECT3D9 GetHWResourceD3D(){
		return m_CommonData.m_pD3D;
	}

	void SetHWResourceDevice(LPDIRECT3DDEVICE9 pDevice){
		m_CommonData.m_pDevice = pDevice;
	}
	LPDIRECT3DDEVICE9 GetHWResourceDevice(){
		return m_CommonData.m_pDevice;
	}

	void SetHWResourceInputDevice(LPDIRECTINPUT8 pDI){
		m_CommonData.m_pDI = pDI;
	}
	LPDIRECTINPUT8 GetHWResourceInputDevice(){
		return m_CommonData.m_pDI;
	}

	void AddHWResourceWnd(HWND hWnd,mS32 width,mS32 height){
		HWResourceDataWin32* p = AddResource(hWnd);
		p->m_nWindowWidth = width;
		p->m_nWindowHeight = height;
	}

	void ReleaseResource(HWResourceDataWin32* p){
		mS32 i;
		for(i=0;i<m_nData;i++){
			if(p == &m_pData[i]){
				ReleaseResourceNo( i );
				return;
			}
		}
	}
	
	void SetHWResourceWnd(HWND hWnd,mS32 width,mS32 height){
		HWResourceDataWin32* p = GetResource(hWnd);
		p->m_nWindowWidth = width;
		p->m_nWindowHeight = height;
	}

	void SetMainResource(HWND hWnd){
		m_pMainData = GetResource(hWnd);
	}
};

#endif
#endif