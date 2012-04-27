#include "..\\..\\LibUtil\\LibUtil\\LibUtil.h"
#include "Model.h"
#include <stdio.h>

#define MODEL_PATH "%s..\\Model\\%s"

//============================================================================
// ���_�`��

#define MODEL_BLEND_COUNT 4

struct ORIGINAL_VERTEX {
	D3DXVECTOR3 Pos;
	float Weight[MODEL_BLEND_COUNT];
	unsigned char Index[MODEL_BLEND_COUNT];
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
#define ORIGINAL_FVF (D3DFVF_XYZB5|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)

struct ANIMATED_VERTEX {
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
#define ANIMATED_FVF (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


//============================================================================
// �e�N�X�`��
static vector<IDirect3DTexture9*> Texture;
static vector<char*> TextureName;

IDirect3DTexture9* GetTexture(IDirect3DDevice9* device, const char* file_name) {
	for (size_t i=0; i<TextureName.size(); i++) {
		if (strcmp(TextureName[i], file_name)==0) return Texture[i];
	}
	string s=GetExePath()+"..\\Model\\"+file_name;
	IDirect3DTexture9* texture=NULL;
	D3DXCreateTextureFromFileEx(
		device, s.c_str(),
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &texture);
	Texture.push_back(texture);
	char* t=new char[strlen(file_name)+1];
	strcpy(t, file_name);
	TextureName.push_back(t);
	return texture;
}


//============================================================================
// �g�[�N��
static char* Pointer;
static char Token[1024];

static void Error(const char* message) {
	MessageBox(NULL, message, "���f���̓ǂݍ��݃G���[", MB_OK);
	PostQuitMessage(0);
}

static void GetToken() {
	char* p=Pointer;
	char* q=Token;
	while (*p!='\0' && strchr(" \t\r\n,;\"", *p)) p++;
	if (*p=='{' || *p=='}') {
		*q++=*p++;
	} else {
		while (*p!='\0' && !strchr(" \t\r\n,;\"{}", *p)) *q++=*p++;
	}
	Pointer=p;
	*q='\0';
}

static void GetToken(const char* token) {
	GetToken();
	if (strcmp(Token, token)!=0) {
		char s[1024];
		sprintf_s(s, sizeof(s), "�z��g�[�N���F%s\n�A�ǂݍ��݃g�[�N���F%s", token, Token);
		Error(s);
	}
}

static float GetFloatToken() {
	GetToken();
	return atof(Token);
}

static int GetIntToken() {
	GetToken();
	return atoi(Token);
}

static void SkipNode() {
	while (*Pointer!='\0') {
		GetToken();
		if (strchr(Token, '{')) break;
	}
	int count=1;
	while (*Pointer!='\0' && count>0) {
		GetToken();
		if (strchr(Token, '{')) count++; else
		if (strchr(Token, '}')) count--;
	}
	if (count>0) {
		Error("���ʂ̔�Ή�");
		return;
	}
}


//============================================================================
// ���f���̍쐬
CModel::CModel(IDirect3DDevice9* device, const char* file_name, bool anim_only)
:	m_pDevice(device), 
	m_pOriginalVertexBuffer(NULL), m_pAnimatedVertexBuffer(NULL), m_nVertexCount(0), 
	m_pIndexBuffer(NULL), m_nFaceCount(0), m_fSmootherWeight(0)
{
	string s=GetExePath()+"..\\Model\\"+file_name;

	// �t�@�C���̓ǂݍ���
	HANDLE file=CreateFile(
		s.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (file==INVALID_HANDLE_VALUE) {
		char s[1024];
		sprintf_s(s, sizeof(s), "�t�@�C��%s�̓ǂݍ��݂Ɏ��s", file_name);
		Error(s);
		return;
	}

	DWORD size=GetFileSize(file, NULL);
	char* buffer=new char[size+1];
	buffer[size]='\0';

	DWORD read_size;
	ReadFile(file, buffer, size, &read_size, NULL);

	CloseHandle(file);
	if (read_size!=size) return;
	
	// �t���[���A�A�j���[�V����
	Pointer=buffer;
	while (*Pointer!='\0') {
		GetToken();

		// �e���v���[�g
		if (strcmp(Token, "template")==0) {
			SkipNode();
		} else
	
		// �t���[��
		if (strcmp(Token, "Frame")==0) {
			if (!m_Frame.empty()) {
				Error("�t�@�C�����ɕ����̃��[�g�t���[��");
				return;
			}
			new CModelFrame(&m_Frame);
		} else

		// �A�j���[�V����
		if (strcmp(Token, "AnimationSet")==0) {
			CModelAnimation* anim=new CModelAnimation();
			m_Animation.push_back(anim);
			anim->m_fTime=0;
			anim->m_fWeight=0;
			
			GetToken();
			anim->m_pName=new char[strlen(Token)+1];
			strcpy(anim->m_pName, Token);
			
			GetToken("{");
			
			while (*Pointer!='\0') {
				GetToken();
				if (strchr(Token, '}')) break;
				
				if (strcmp(Token, "Animation")==0) {
					CModelAnimationKey* key=new CModelAnimationKey();
					anim->m_Key.push_back(key);
					
					GetToken();
					GetToken("{");

					GetToken("{");
					GetToken();
					key->m_pFrameName=new char[strlen(Token)+1];
					strcpy(key->m_pFrameName, Token);
					key->m_FrameIndex=FindFrame(key->m_pFrameName)->m_Index;
					GetToken("}");
					
					GetToken("AnimationKey");
					GetToken("{");
					GetToken("4");
					DWORD key_count=GetIntToken();
					for (DWORD i=0; i<key_count; i++) {
						key->m_Time.push_back((float)GetIntToken());
						GetToken("16");
						float f[16];
						for (DWORD j=0; j<16; j++) f[j]=GetFloatToken();
						key->m_Matrix.push_back(new D3DXMATRIXA16(f));
					}
					GetToken("}");
					
					GetToken("}");
				}
			}
		}
	}

	// ���b�V��
	Pointer=buffer;
	if (!anim_only) {
		while (*Pointer!='\0') {
			GetToken();
			
			// �e���v���[�g
			if (strcmp(Token, "template")==0) {
				SkipNode();
			} else
		
			// ���b�V��
			if (strcmp(Token, "Mesh")==0) {
				if (m_pOriginalVertexBuffer) {
					Error("�t�@�C�����ɕ����̃��b�V��");
					return;
				}
				
				GetToken();			
				GetToken("{");

				// ���_
				m_nVertexCount=GetIntToken();
				device->CreateVertexBuffer(
					sizeof(ORIGINAL_VERTEX)*m_nVertexCount, 0, 
					ORIGINAL_FVF, D3DPOOL_SYSTEMMEM,
					&m_pOriginalVertexBuffer, NULL);
				device->CreateVertexBuffer(
					sizeof(ANIMATED_VERTEX)*m_nVertexCount, 0, 
					ANIMATED_FVF, D3DPOOL_MANAGED,
					&m_pAnimatedVertexBuffer, NULL);
				if (!m_pOriginalVertexBuffer || !m_pAnimatedVertexBuffer) {
					Error("���_�o�b�t�@�̍쐬�Ɏ��s");
					return;
				}
				void* vb;
				m_pOriginalVertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v=(ORIGINAL_VERTEX*)vb;
				for (DWORD i=0; i<m_nVertexCount; i++) {
					v[i].Pos.x=GetFloatToken();
					v[i].Pos.y=GetFloatToken();
					v[i].Pos.z=GetFloatToken();
					for (DWORD j=0; j<MODEL_BLEND_COUNT; j++) v[i].Index[j]=0;
					v[i].Weight[0]=1;
					for (DWORD j=1; j<MODEL_BLEND_COUNT; j++) v[i].Weight[j]=0;
				}
				m_pOriginalVertexBuffer->Unlock();

				// �C���f�b�N�X
				m_nFaceCount=GetIntToken();
				device->CreateIndexBuffer(
					sizeof(WORD)*3*m_nFaceCount, 0, 
					D3DFMT_INDEX16, D3DPOOL_MANAGED, 
					&m_pIndexBuffer, NULL);
				if (!m_pIndexBuffer) {
					Error("�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");
					return;
				}
				void* ib;
				m_pIndexBuffer->Lock(0, 0, &ib, 0);
				WORD* index=(WORD*)ib;
				for (DWORD i=0, n=m_nFaceCount*3; i<n; i+=3) {
					GetToken("3");
					for (DWORD j=0; j<3; j++) {
						index[i+j]=GetIntToken();
					}
				}
				m_pIndexBuffer->Unlock();
			} else
			
			// �@��
			if (strcmp(Token, "MeshNormals")==0) {
				GetToken("{");
				
				if (GetIntToken()!=m_nVertexCount) {
					Error("���_���Ɩ@�������s��v");
					return;
				}

				void* vb;
				m_pOriginalVertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v=(ORIGINAL_VERTEX*)vb;
				for (DWORD i=0; i<m_nVertexCount; i++) {
					v[i].Normal.x=GetFloatToken();
					v[i].Normal.y=GetFloatToken();
					v[i].Normal.z=GetFloatToken();
				}
				m_pOriginalVertexBuffer->Unlock();
				
				if (GetIntToken()!=m_nFaceCount) {
					Error("�ʐ��Ɩ@���C���f�b�N�X�����s��v");
					return;
				}
				for (DWORD i=0, n=m_nFaceCount*4; i<n; i++) GetToken();
			} else
							
			// �e�N�X�`�����W
			if (strcmp(Token, "MeshTextureCoords")==0) {
				GetToken("{");
				
				if (GetIntToken()!=m_nVertexCount) {
					Error("���_���ƃe�N�X�`�����W�����s��v");
					return;
				}
				
				void* ovb;
				m_pOriginalVertexBuffer->Lock(0, 0, &ovb, 0);			
				ORIGINAL_VERTEX* ov=(ORIGINAL_VERTEX*)ovb;

				void* avb;
				m_pAnimatedVertexBuffer->Lock(0, 0, &avb, 0);			
				ANIMATED_VERTEX* av=(ANIMATED_VERTEX*)avb;

				for (DWORD i=0; i<m_nVertexCount; i++) {
					ov[i].UV.x=av[i].UV.x=GetFloatToken();
					ov[i].UV.y=av[i].UV.y=GetFloatToken();
				}

				m_pOriginalVertexBuffer->Unlock();
				m_pAnimatedVertexBuffer->Unlock();
			} else

			// �}�e���A�����X�g
			if (strcmp(Token, "MeshMaterialList")==0) {
				GetToken("{");
				DWORD material_count=GetIntToken();
				
				if (GetIntToken()!=m_nFaceCount) {
					Error("�ʐ��ƃ}�e���A�����X�g���̕s��v");
					return;
				}

				// �T�u�Z�b�g�̍쐬
				CModelSubset* subset=NULL;
				DWORD prev_index=-1;
				for (DWORD i=0; i<m_nFaceCount; i++) {
					DWORD index=GetIntToken();
					if (index!=prev_index) {
						subset=new CModelSubset();
						subset->m_nFaceCount=1;
						subset->m_nFaceIndex=i;
						subset->m_nMaterialIndex=index;
						m_Subset.push_back(subset);
						prev_index=index;
					} else {
						subset->m_nFaceCount++;
					}
				}

				// �}�e���A��
				for (DWORD i=0; i<material_count; i++) {
					CModelMaterial* material=new CModelMaterial();
					m_Material.push_back(material);

					GetToken("Material");
					GetToken("{");
					
					// �F
					material->m_Color=D3DXVECTOR4(
						GetFloatToken(), GetFloatToken(), 
						GetFloatToken(), GetFloatToken());
					for (int j=0; j<7; j++) GetToken();

					// �e�N�X�`��
					GetToken("TextureFilename");
					GetToken("{");
					GetToken();
					material->m_pTexture=GetTexture(m_pDevice, Token);
					GetToken("}");

					GetToken("}");
				}
			} else

			// �X�L���E�F�C�g
			if (strcmp(Token, "SkinWeights")==0) {
				GetToken("{");
				GetToken();
				CModelFrame* frame=FindFrame(Token);
				DWORD count=GetIntToken();
				DWORD* vertex=new DWORD[count];
				float* weight=new float[count];
				float matrix[16];
				for (DWORD i=0; i<count; i++) vertex[i]=GetIntToken();
				for (DWORD i=0; i<count; i++) weight[i]=GetFloatToken();
				for (DWORD i=0; i<16; i++) matrix[i]=GetFloatToken();
				GetToken("}");
				
				if (frame) {
					void* vb;
					m_pOriginalVertexBuffer->Lock(0, 0, &vb, 0);
					ORIGINAL_VERTEX* v=(ORIGINAL_VERTEX*)vb;
					for (DWORD i=0; i<count; i++) {
						DWORD j=0;
						while (j<3 && v[vertex[i]].Weight[j]>0 && v[vertex[i]].Weight[j]<1) j++;
						if (j<3) {
							v[vertex[i]].Weight[j]=weight[i];
							v[vertex[i]].Index[j]=(unsigned char)frame->m_Index;
						}
					}
					m_pOriginalVertexBuffer->Unlock();
					
					frame->m_mOffsetMatrix=D3DXMATRIXA16(matrix);
				}

				delete[] vertex;
				delete[] weight;
			}
		}
	}
	
	// ��ЂÂ�
	delete[] buffer;
}


//============================================================================
// �t���[���̍쐬
CModelFrame::CModelFrame(vector<CModelFrame*>* frame)
{
	// �z��ɒǉ�
	m_Index=frame->size();
	frame->push_back(this);

	// �s��
	D3DXMatrixIdentity(&m_mTransformMatrix);
	D3DXMatrixIdentity(&m_mOffsetMatrix);
	
	// ���O
	GetToken();
	m_pName=new char[strlen(Token)+1];
	strcpy(m_pName, Token);
	
	GetToken("{");
	
	while (*Pointer!='\0') {
		GetToken();
		if (strchr(Token, '}')) break;
		
		// �q�t���[��
		if (strcmp(Token, "Frame")==0) {
			m_Child.push_back(new CModelFrame(frame));
		} else
		
		// �ό`�s��
		if (strcmp(Token, "FrameTransformMatrix")==0) {
			GetToken("{");
			float f[16];
			for (int i=0; i<16; i++) f[i]=GetFloatToken();
			m_mTransformMatrix=D3DXMATRIXA16(f);
			GetToken("}");
		} else
		
		// ���̑��̃m�[�h
		{
			SkipNode();
		}
	}
}


//============================================================================
// �A�j���[�V�����i�s��̍X�V�ƒ��_�ϊ��j

// �t���[���̍X�V
void CModel::AnimateFrame(D3DXMATRIXA16* world) {

	// �A�j���[�V�����L�[���֌W����t���[���̕ό`�s���������
	for (size_t i=0; i<m_Animation.size(); i++) {
		CModelAnimation* anim=m_Animation[i];
		if (anim->m_fWeight==0) continue;

		// 0�ŏ�����
		for (size_t j=0; j<anim->m_Key.size(); j++) {
			CModelAnimationKey* key=anim->m_Key[j];
			ZeroMemory(&m_Frame[key->m_FrameIndex]->m_mTransformMatrix, sizeof(D3DXMATRIXA16));
		}
	}

	// �A�j���[�V�����L�[���֌W����t���[���̕ό`�s����v�Z
	for (size_t i=0; i<m_Animation.size(); i++) {
		CModelAnimation* anim=m_Animation[i];
		if (anim->m_fWeight==0) continue;
		
		for (size_t j=0; j<anim->m_Key.size(); j++) {
			CModelAnimationKey* key=anim->m_Key[j];
			CModelFrame* frame=m_Frame[key->m_FrameIndex];
			if (key->m_Time.empty()) continue;
			
			// �L�[�t���[���̕��
			float time=anim->m_fTime;
			if (time<key->m_Time.front()) {
				frame->m_mTransformMatrix+=*key->m_Matrix.front()*anim->m_fWeight;
			} else
			if (time>=key->m_Time.back()) {
				frame->m_mTransformMatrix+=*key->m_Matrix.back()*anim->m_fWeight;
			} else {
				for (size_t k=1; k<key->m_Time.size(); k++) {
					if (time<key->m_Time[k] && key->m_Time[k-1]!=key->m_Time[k]) {
						float r=(key->m_Time[k]-time)/(key->m_Time[k]-key->m_Time[k-1]);
						frame->m_mTransformMatrix+=
							(*key->m_Matrix[k-1]*r+*key->m_Matrix[k]*(1-r))*
							anim->m_fWeight;
						break;
					}
				}
			}
		}
	}

	// �t���[���̍����s����v�Z
	m_Frame[0]->Animate(world);

	// �A�j���[�V�����؂�ւ����
	if (m_fSmootherWeight>0) {
		for (size_t i=0; i<m_Frame.size(); i++) {
			D3DXMATRIXA16& a=m_Frame[i]->m_mCombinedMatrix;
			D3DXMATRIXA16& b=m_Frame[i]->m_mSmootherMatrix;
			a=a*(1-m_fSmootherWeight)+b*m_fSmootherWeight;
			m_Frame[i]->m_mSkinningMatrix=m_Frame[i]->m_mOffsetMatrix*a;
		}
	}
}

	
// ���_�ϊ�
void CModel::AnimateVertex() {
	void* ovb;
	m_pOriginalVertexBuffer->Lock(0, 0, &ovb, D3DLOCK_READONLY);			
	ORIGINAL_VERTEX* ov=(ORIGINAL_VERTEX*)ovb;

	void* avb;
	m_pAnimatedVertexBuffer->Lock(0, 0, &avb, 0);			
	ANIMATED_VERTEX* av=(ANIMATED_VERTEX*)avb;

	for (DWORD i=0; i<m_nVertexCount; i++) {
	
		// ���_
		D3DXVECTOR4 pos=D3DXVECTOR4(0, 0, 0, 0);
		for (DWORD j=0; j<MODEL_BLEND_COUNT; j++) {
			D3DXVECTOR4 v;
			D3DXVec3Transform(&v, &ov[i].Pos, &m_Frame[ov[i].Index[j]]->m_mSkinningMatrix);
			pos+=v*ov[i].Weight[j];
		}
		av[i].Pos=(D3DXVECTOR3)pos;

		// �@��
		D3DXVECTOR4 normal=D3DXVECTOR4(0, 0, 0, 0);
		for (DWORD j=0; j<MODEL_BLEND_COUNT; j++) {
			D3DXVECTOR3& n=ov[i].Normal;
			D3DXVECTOR4 v(n.x, n.y, n.z, 0);
			D3DXVec4Transform(&v, &v, &m_Frame[ov[i].Index[j]]->m_mSkinningMatrix);
			normal+=v*ov[i].Weight[j];
		}
		D3DXVec4Normalize(&normal, &normal);
		av[i].Normal=(D3DXVECTOR3)normal;
	}

	m_pOriginalVertexBuffer->Unlock();
	m_pAnimatedVertexBuffer->Unlock();	
}

// �t���[��
void CModelFrame::Animate(D3DXMATRIXA16* parent) {
	m_mCombinedMatrix=m_mTransformMatrix*(*parent);
	for (size_t i=0; i<m_Child.size(); i++) {
		m_Child[i]->Animate(&m_mCombinedMatrix);
	}
	m_mSkinningMatrix=m_mOffsetMatrix*m_mCombinedMatrix;
}


//============================================================================
// ���f���̕`��

// �V�F�[�_�p
void CModel::Draw(ID3DXEffect* effect, D3DXMATRIXA16* view, D3DXMATRIXA16* proj) {

	// �o�b�t�@�AFVF
	m_pDevice->SetStreamSource(0, m_pAnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->SetFVF(ANIMATED_FVF);
	
	// �s��
	D3DXMATRIXA16 vproj=(*view)*(*proj);
	effect->SetMatrix("VProj", &vproj);
	
	// �T�u�Z�b�g�̕`��
	for (size_t i=0; i<m_Subset.size(); i++) {
		CModelSubset* subset=m_Subset[i];

		// �F�A�e�N�X�`��
		DWORD mi=subset->m_nMaterialIndex;
		effect->SetVector("Diffuse", &m_Material[mi]->m_Color);
		effect->SetTexture("MeshTex", m_Material[mi]->m_pTexture);

		// �`��
		UINT np;
		effect->Begin(&np, 0);
		for (UINT p=0; p<np; p++) {
			effect->BeginPass(p);
			m_pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST, 0, 0, m_nVertexCount, 
				subset->m_nFaceIndex*3, subset->m_nFaceCount);
			effect->EndPass();
		}
		effect->End();
	}
}

// �Œ�p�C�v���C���p
void CModel::Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj) {

	// �o�b�t�@�AFVF
	m_pDevice->SetStreamSource(0, m_pAnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->SetFVF(ANIMATED_FVF);

	// �s��
	D3DXMATRIXA16 world;
	D3DXMatrixIdentity(&world);
	m_pDevice->SetTransform(D3DTS_WORLD, &world);
	m_pDevice->SetTransform(D3DTS_VIEW, view);
	m_pDevice->SetTransform(D3DTS_PROJECTION, proj);

	// �T�u�Z�b�g�̕`��
	for (size_t i=0; i<m_Subset.size(); i++) {
		CModelSubset* subset=m_Subset[i];

		// �F�A�e�N�X�`��
		DWORD mi=subset->m_nMaterialIndex;
		D3DMATERIAL9 material;
		D3DXVECTOR4 c=m_Material[mi]->m_Color;
		material.Diffuse.r=c.x;
		material.Diffuse.g=c.y;
		material.Diffuse.b=c.z;
		material.Diffuse.a=c.w;
		material.Specular=material.Diffuse;
		material.Ambient=material.Diffuse;
		material.Emissive.r=0;
		material.Emissive.g=0;
		material.Emissive.b=0;
		material.Emissive.a=0;
		m_pDevice->SetMaterial(&material);
		m_pDevice->SetTexture(0, m_Material[mi]->m_pTexture);

		// �`��
		m_pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0, 0, m_nVertexCount, 
			subset->m_nFaceIndex*3, subset->m_nFaceCount);
	}
}


//============================================================================
// �t���[���E�A�j���[�V�����Ȃǂ̑���

CModelFrame* CModel::FindFrame(const char* name) {
	for (size_t i=0; i<m_Frame.size(); i++) {
		if (strcmp(m_Frame[i]->m_pName, name)==0) return m_Frame[i];
	}
	return NULL;
}

CModelAnimation* CModel::FindAnimation(const char* name) {
	for (size_t i=0; i<m_Animation.size(); i++) {
		if (strcmp(m_Animation[i]->m_pName, name)==0) return m_Animation[i];
	}
	return NULL;
}

void CModel::AddAnimation(CModelAnimation* anim) {
	m_Animation.push_back(new CModelAnimation(anim));
}

CModelAnimation::CModelAnimation(const CModelAnimation* anim)
:	m_pName(anim->m_pName), m_fTime(anim->m_fTime), m_fWeight(anim->m_fWeight), m_Key(anim->m_Key)
{}

void CModel::ResetAnimationWeight() {
	for (size_t i=0; i<m_Animation.size(); i++) {
		m_Animation[i]->m_fWeight=0;
	}
}

void CModel::SaveSmoother() {
	for (size_t i=0; i<m_Frame.size(); i++) {
		m_Frame[i]->m_mSmootherMatrix=m_Frame[i]->m_mCombinedMatrix;
	}
}


