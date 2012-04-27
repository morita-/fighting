#ifndef FONTH
#define FONTH

#include "..\..\LibUtil\LibUtil\Archive.h"
#include "CTexture.h"
#include <string>
using namespace std;


//==============================================================
// �t�H���g
class CFont {
protected:

	// �摜
	CTextureRef* Texture;

	// �e�����̍��W
	float X[256], Y[256];

	// �e�N�X�`����̃T�C�Y�A�\���T�C�Y�A�e�̈ʒu
	float SW, SH, DW, DH, ShadeX, ShadeY;

public:

	// �R���X�g���N�^
	CFont(LPDIRECT3DDEVICE9 device, 
		string file, CArchiveLoader* archive, 
		string chars, float sw, float sh, float dw, float dh, 
		float shadex, float shadey);
	
	// �`��
	void DrawChar(unsigned char c, float x, float y, D3DCOLOR color);
	void DrawChar(unsigned char c, float x, float y, D3DCOLOR color, D3DCOLOR shade);
	void DrawText(string s, float x, float y, D3DCOLOR color);
	void DrawText(string s, float x, float y, D3DCOLOR color, D3DCOLOR shade);
	void DrawText(string s, float x, float y, float sx, float sy, D3DCOLOR color);

	// �T�C�Y
	float GetTextW(string s) { return s.length()*DW; }
	float GetTextH() { return DH; }
};




#endif