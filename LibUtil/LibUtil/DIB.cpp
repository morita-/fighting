#include "LibUtil.h"
#include "libpng\\png.h"
extern "C" {
	#include "libjpeg\\jpeglib.h"
}

//==============================================================
// ������

// �������iDIB�Z�N�V�����j
void CDIB::InitDIB()
{
	if (DIB) DeleteObject(DIB);

	// DIB�Z�N�V�����̏���ێ�����BITMAPINFO�\���̂����
	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	BITMAPINFOHEADER* bih=&bi.bmiHeader;
	bih->biSize=sizeof(BITMAPINFOHEADER);
	bih->biWidth=W;
	bih->biHeight=-H;
	bih->biPlanes=1;
	bih->biBitCount=32;
	bih->biCompression=BI_RGB;

	// DIB�Z�N�V�����̍쐬
	DIB=CreateDIBSection(
		DC, &bi, DIB_RGB_COLORS,
		(void**)&Pixel, NULL, 0);
	SelectObject(DC, DIB);

	// �A���t�@�̈�̍쐬
	if (Alpha) delete [] Alpha;
	Alpha=new BYTE[W*H];
}

// �������i���[�W�����j�F
// �A���t�@�Ɋ�Â��ă��[�W�������쐬����B
void CDIB::InitRgn() {
	if (Rgn) DeleteObject(Rgn);
	Rgn=CreateRectRgn(0, 0, 0, 0);
	HRGN rgn;
	BYTE* a=Alpha;
	int left;
	for (int y=0; y<H; y++) {
		for (int x=0; x<W; ) {
			for (; x<W && *a==0; x++, a++) ;
			left=x;
			for (; x<W && *a!=0; x++, a++) ;
			if (x-left>0) {
				rgn=CreateRectRgn(left, y, x, y+1);
				CombineRgn(Rgn, Rgn, rgn, RGN_OR);
				DeleteObject(rgn);
			}
		}
	}
}

// �������i���[�W�����j�F
// �S�̂𕢂����[�W�������쐬����B
void CDIB::InitRgnSimple() {
	if (Rgn) DeleteObject(Rgn);
	Rgn=CreateRectRgn(0, 0, W, H);
}


//==============================================================
// �R���X�g���N�^�C�f�X�g���N�^

// �R���X�g���N�^�̋��ʏ���
void CDIB::Construct() {
	HDC hdc=::GetDC(NULL);
	DC=CreateCompatibleDC(hdc);
	ReleaseDC(NULL, hdc);
}

// �R���X�g���N�^�i�T�C�Y�j
CDIB::CDIB(int w, int h)
:	DIB(NULL), Alpha(NULL), Rgn(NULL), W(w), H(h)
{
	Construct();
	InitDIB();
	InitRgnSimple();
}

// �R���X�g���N�^�i�L���v�`���j
CDIB::CDIB(HWND hwnd)
:	DIB(NULL), Alpha(NULL), Rgn(NULL)
{
	Construct();
	Capture(hwnd);
}

// �R���X�g���N�^�i�t�@�C���j
CDIB::CDIB(string file_name)
:	DIB(NULL), Alpha(NULL), Rgn(NULL)
{
	Construct();
	LoadFromFile(file_name);
}

// �f�X�g���N�^
CDIB::~CDIB() {
	if (DC) DeleteDC(DC);
	if (DIB) DeleteObject(DIB);
	if (Alpha) delete[] Alpha;
	if (Rgn) DeleteObject(Rgn);
}


//==============================================================
// �R�s�[

// CDIB��CDIB
void CDIB::Draw(
	int dx, int dy, int dw, int dh, 
	CDIB* dib, int sx, int sy, int sw, int sh
) {
	StretchBlt(DC, dx, dy, dw, dh, dib->DC, sx, sy, sw, sh, SRCCOPY);
}

// CDIB��HBITMAP
void CDIB::Draw(
	int dx, int dy, int dw, int dh, 
	HBITMAP bmp, int sx, int sy, int sw, int sh
) {
	HDC null_dc=::GetDC(NULL);
	HDC bmp_dc=CreateCompatibleDC(null_dc);
	HGDIOBJ obj=SelectObject(bmp_dc, bmp);
	StretchBlt(DC, dx, dy, dw, dh, bmp_dc, sx, sy, sw, sh, SRCCOPY);
	SelectObject(bmp_dc, obj);
	DeleteDC(bmp_dc);
	ReleaseDC(NULL, null_dc);
}

// DC��CDIB
void CDIB::Draw(
	HDC dc, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh
) {
	StretchBlt(dc, dx, dy, dw, dh, DC, sx, sy, sw, sh, SRCCOPY);
}


//==============================================================
// �N���b�v���R�s�[�i���[�W�������g�p�j

// CDIB��CDIB
void CDIB::ClipDraw(
	int dx, int dy, int dw, int dh, CDIB* dib, int sx, int sy
) {
	SelectClipRgn(DC, dib->Rgn);
	OffsetClipRgn(DC, dx-sx, dy-sy);
	StretchBlt(DC, dx, dy, dw, dh, dib->DC, sx, sy, dw, dh, SRCCOPY);
	SelectClipRgn(DC, NULL);
}

// DC��CDIB
void CDIB::ClipDraw(
	HDC dc, int dx, int dy, int dw, int dh, int sx, int sy
) {
	POINT p;
	GetViewportOrgEx(dc, &p);
	SelectClipRgn(dc, Rgn);
	OffsetClipRgn(dc, p.x+dx-sx, p.y+dy-sy);
	StretchBlt(dc, dx, dy, dw, dh, DC, sx, sy, dw, dh, SRCCOPY);
	SelectClipRgn(dc, NULL);
}


//==============================================================
// ��ʃL���v�`��
void CDIB::Capture(HWND hwnd)
{
	// �摜�T�C�Y���E�B���h�E�T�C�Y�ɍ��킹��
	if (!hwnd) hwnd=GetDesktopWindow();	
	GetWindowWH(hwnd, &W, &H);
	InitDIB();
	InitRgnSimple();
	
	// ��ʂ��L���v�`������
	HDC hdc_win=::GetDC(NULL);
	BitBlt(DC, 0, 0, W, H, hdc_win, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hdc_win);
}


//==============================================================
// �����F�̐ݒ�F
// �w�肵���F�ƈ�v����s�N�Z���𓧖��ɂ���B
// ���̐F�̃s�N�Z���͕s�����ɂȂ�B
void CDIB::SetClipColor(DWORD xrgb) {
	xrgb&=0x00ffffff;
	DWORD* p=Pixel;
	BYTE* a=Alpha;
	for (int y=0; y<H; y++) {
		for (int x=0; x<W; x++, p++, a++) {
			*a=(*p==xrgb?0:255);
		}
	}
	InitRgn();
}


//==============================================================
// ���[�h
void CDIB::LoadFromFile(string file_name) {
	if (!FileExists(file_name)) return;
	file_name=ToUpper(file_name);	
	if (EndsWith(file_name, ".BMP")) LoadBMPFile(file_name); else
	if (EndsWith(file_name, ".PNG")) LoadPNGFile(file_name);
	if (EndsWith(file_name, ".JPG") || EndsWith(file_name, ".JPEG")) LoadJPEGFile(file_name);
}

// BMP�`���̃��[�h
void CDIB::LoadBMPFile(string file_name)
{
	// �r�b�g�}�b�v�̃��[�h
	HBITMAP hbmp=(HBITMAP)LoadImage(
		NULL, file_name.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// DIB�̏�����
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);
	W=bmp.bmWidth;
	H=bmp.bmHeight;
	InitDIB();
	FillMemory(Alpha, W*H, 255);
	InitRgnSimple();

	// �r�b�g�}�b�v����DIB�ւ̃R�s�[
	Draw(0, 0, W, H, hbmp, 0, 0, W, H);
	DeleteObject(hbmp);
}

// PNG�`���̃��[�h
void CDIB::LoadPNGFile(string file_name)
{
	unsigned int sig_read=0;
	int bit_depth, color_type, interlace_type;
	
	// �t�@�C�����J��
	FILE* fp=fopen(file_name.c_str(), "rb");
	if (!fp) return;

	// png_struct�̏�����
	png_structp png_ptr=png_create_read_struct(
		PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return;
	}
	png_init_io(png_ptr, fp);

	// png_info�̏�����
	png_infop info_ptr=png_create_info_struct(png_ptr);
	if (!info_ptr) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return;
	}

	// �摜���̎擾
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, 
		(unsigned long*)&W, (unsigned long*)&H, 
		&bit_depth, &color_type, &interlace_type, NULL, NULL);

	// DIB�̏�����
	InitDIB();
	
	// �ϊ����@�̐ݒ�F
	// �p���b�g��RGB�ցB
	if (color_type==PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// �����F���A���t�@�ցB
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// �r�b�g�[�x��8�r�b�g�ցB
	if (bit_depth<8) png_set_packing(png_ptr); else
	if (bit_depth==16) png_set_strip_16(png_ptr);
	
	// 24�r�b�g�iRGB�j��32�r�b�g�iRGB0�j�ցB
	if (color_type==PNG_COLOR_TYPE_RGB)
		png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);

	// RGB��BGR�ցB
	if (color_type==PNG_COLOR_TYPE_PALETTE ||
		color_type==PNG_COLOR_TYPE_RGB ||
		color_type==PNG_COLOR_TYPE_RGB_ALPHA)
		png_set_bgr(png_ptr);

	// �摜�̓ǂݍ���
	png_bytep *row_pointers=new png_bytep[H];
	for (int row=0; row<H; row++) row_pointers[row]=(png_bytep)(Pixel+row*W);
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, info_ptr);
	delete [] row_pointers;

	// ��ЂÂ�
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	// �A���t�@�����o��
	DWORD* p=Pixel;
	BYTE* a=Alpha;
	for (int y=0; y<H; y++) {
		for (int x=0; x<W; x++, p++, a++) {
			*a=(BYTE)(*p>>24);
			*p&=0x00ffffff;
		}
	}

	// ���[�W�����̏�����
	InitRgn();
}

// JPEG�`���̃��[�h
static void JpegErrorHandler(j_common_ptr) {}

void CDIB::LoadJPEGFile(string file_name)
{
	// �G���[�n���h���̐ݒ�
	jpeg_decompress_struct jd;
	jpeg_error_mgr jem;
	jd.err=jpeg_std_error(&jem);
	jem.error_exit=JpegErrorHandler;

	// �t�@�C�����J��
	FILE* fp=fopen(file_name.c_str(), "rb");
	if (!fp) return;

	// �f�R�[�h�O����
	jpeg_create_decompress(&jd);
	jpeg_stdio_src(&jd, fp);
	jpeg_read_header(&jd, TRUE);
	jpeg_start_decompress(&jd);

	// DIB�̏�����
	W=jd.image_width;
	H=jd.image_height;
	InitDIB();
	
	// �f�R�[�h
	for (int row=0; row<H; row++) {
		JSAMPROW jsr=(JSAMPROW)(Pixel+row*W);
		jpeg_read_scanlines(&jd, &jsr, 1);

		// 24�r�b�g����32�r�b�g�ɕϊ�����ilibjpeg�ōs�����@�����肻���j
		char *p=(char*)(Pixel+row*W)+3*(W-1), *q=(char*)(Pixel+row*W)+4*(W-1);
		do {
			q[0]=p[0];
			q[1]=p[1];
			q[2]=p[2];
			q[3]=0;
			p-=3;
			q-=4;
		} while (p!=q);
	}

	// �f�R�[�h�㏈��
	jpeg_finish_decompress(&jd);
	jpeg_destroy_decompress(&jd);

	// �A���t�@�̏�����
	BYTE* a=Alpha;
	for (int y=0; y<H; y++) {
		for (int x=0; x<W; x++, a++) *a=1;
	}

	// ���[�W�����̏�����
	InitRgn();
}
