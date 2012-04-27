#ifndef GDIH
#define GDIH

#include <windows.h>
#include <string>
using namespace std;


class CIcon {
protected:
	HICON HIcon;
public:
	CIcon();
	~CIcon();
	void LoadFromFile(string file);
	HICON GetIcon() { return HIcon; }
};


void GetWindowXY(HWND hwnd, int* x, int* y);
void SetWindowXY(HWND hwnd, int x, int y);
void GetWindowWH(HWND hwnd, int* w, int* h);
void SetWindowWH(HWND hwnd, int w, int h);
void GetClientWH(HWND hwnd, int* w, int* h);
void SetClientWH(HWND hwnd, int w, int h);

HRGN CloneRgn(HRGN rgn);


#endif
