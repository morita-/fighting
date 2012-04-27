#include "LibUtil.h"


//==============================================================
// アイコン
CIcon::CIcon()
:	HIcon(NULL)
{}

CIcon::~CIcon() {
	if (HIcon) DeleteObject(HIcon);
}

void CIcon::LoadFromFile(string file) {
	if (HIcon) DeleteObject(HIcon);
	LoadImage(GetModuleHandle(NULL), file.c_str(), 
		IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}


// ウィンドウの位置を取得する
void GetWindowXY(HWND hwnd, int* x, int* y) {
	RECT r;
	GetWindowRect(hwnd, &r);
	*x=r.left;
	*y=r.top;
}

// ウィンドウの位置を設定する
void SetWindowXY(HWND hwnd, int x, int y) {
	int w, h;
	GetWindowWH(hwnd, &w, &h);
	MoveWindow(hwnd, x, y, w, h, TRUE);
}

// ウィンドウの幅と高さを取得する
void GetWindowWH(HWND hwnd, int* w, int* h) {
	RECT r;
	GetWindowRect(hwnd, &r);
	*w=r.right-r.left;
	*h=r.bottom-r.top;
}

// ウィンドウの幅と高さを設定する
void SetWindowWH(HWND hwnd, int w, int h) {
	int x, y;
	GetWindowXY(hwnd, &x, &y);	
	MoveWindow(hwnd, x, y, w, h, TRUE);
}

// クライアント領域の幅と高さを取得する
void GetClientWH(HWND hwnd, int* w, int* h) {
	RECT r;
	GetClientRect(hwnd, &r);
	*w=r.right-r.left;
	*h=r.bottom-r.top;
}

// クライアント領域の幅と高さを設定する
void SetClientWH(HWND hwnd, int w, int h) {
	RECT wr, cr;
	GetWindowRect(hwnd, &wr);
	GetClientRect(hwnd, &cr);
	MoveWindow(hwnd, wr.left, wr.top, 
		(wr.right-wr.left)+w-(cr.right-cr.left),
		(wr.bottom-wr.top)+h-(cr.bottom-cr.top),
		TRUE);
}

// リージョンを複製する
HRGN CloneRgn(HRGN rgn) {
	HRGN ret;
	ret=CreateRectRgn(0, 0, 0, 0);
	CombineRgn(ret, rgn, ret, RGN_COPY);
	return ret;
}

