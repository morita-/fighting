#ifndef LIBUTIL_DIBH
#define LIBUTIL_DIBH

#include <windows.h>
#include <string>
using namespace std;


//==============================================================
// DIB
class CDIB {
protected:
	HBITMAP DIB;
	HDC DC;
	HRGN Rgn;
	DWORD* Pixel;
	BYTE* Alpha;
	int W, H;

	void Construct();
	void InitDIB();
	void InitRgn();
	void InitRgnSimple();

	void LoadBMPFile(string file_name);
	void LoadPNGFile(string file_name);
	void LoadJPEGFile(string file_name);

public:

	CDIB(int w, int h);
	CDIB(HWND hwnd);
	CDIB(string file_name);

	~CDIB();

	HBITMAP GetBitmap() { return DIB; }
	HDC GetDC() { return DC; }
	HRGN GetRgn() { return Rgn; }

	DWORD* GetPixel() { return Pixel; }
	DWORD GetPixel(int x, int y) { return Pixel[x+y*W]; }
	int GetW() { return W; }
	int GetH() { return H; }

	void Draw(
		int dx, int dy, int dw, int dh, 
		CDIB* dib, int sx, int sy, int sw, int sh);
	void Draw(int dx, int dy, int dw, int dh, CDIB* dib) {
		Draw(dx, dy, dw, dh, dib, 0, 0, dib->W, dib->H);
	}
	void Draw(
		int dx, int dy, int dw, int dh, 
		HBITMAP bmp, int sx, int sy, int sw, int sh);

	void Draw(HDC dc, int dx, int dy, int dw, int dh, 
		int sx, int sy, int sw, int sh);
	void Draw(HDC dc, int dx, int dy, int dw, int dh) {
		Draw(dc, dx, dy, dw, dh, 0, 0, W, H);
	}

	void ClipDraw(int dx, int dy, int dw, int dh, CDIB* dib, int sx, int sy);
	void ClipDraw(int dx, int dy, CDIB* dib) {
		ClipDraw(dx, dy, dib->W, dib->H, dib, 0, 0);
	}
	void ClipDraw(HDC dc, int dx, int dy, int dw, int dh, int sx, int sy);
	void ClipDraw(HDC dc, int dx, int dy) {
		ClipDraw(dc, dx, dy, W, H, 0, 0);
	}

	void Capture(HWND hwnd);
	void SetClipColor(DWORD xrgb);

	void LoadFromFile(string file_name);
};


#endif
