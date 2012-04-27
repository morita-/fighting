#pragma warning(disable:4819)
#include "AnimMesh.h"
#include "..\..\LibUtil\LibUtil\LibUtil.h"


//==============================================================
// AllocateHierarchy���s���ۂɎg�p����O���[�o���ϐ�
static LPDIRECT3DDEVICE9 Device;
static string Path;
static CAllocateHierarchy* AllocateHierarchy=NULL;


//==============================================================
// ���_�V�F�[�_�ɂ��X�L�j���O

// �G�t�F�N�g
static ID3DXEffect* AnimMeshEffect=NULL;

// �G�t�F�N�g�̃��[�h
static void LoadAnimMeshEffect(LPDIRECT3DDEVICE9 device) {
	D3DXCreateEffectFromFile(
		device, 
		(GetExePath()+"..\\Effect\\AnimMesh.fx").c_str(),
		NULL, NULL, D3DXFX_NOT_CLONEABLE, 
		NULL, &AnimMeshEffect, NULL);
}


//==============================================================
// �t���[���̐����Ɣj��

// �t���[���̐������ɌĂяo�����֐�
HRESULT CAllocateHierarchy::CreateFrame(
	LPCTSTR name, LPD3DXFRAME *new_frame
) {
	*new_frame=new CFrame(name);
	return S_OK;
}

// �t���[���̔j�����ɌĂяo�����֐�
HRESULT CAllocateHierarchy::DestroyFrame(
	LPD3DXFRAME frame
) {
	delete (CFrame*)frame;
	return S_OK;
}

// �t���[���̃R���X�g���N�^
CFrame::CFrame(LPCTSTR name) {
	ZeroMemory(this, sizeof(D3DXFRAME));

	// ��{�N���X�i�\���́j�̏�����
	pMeshContainer=NULL;
	pFrameSibling=NULL;
	pFrameFirstChild=NULL;

	// �t���[�����̐ݒ�
	if (name) {
		int n=lstrlen(name)+1;
		Name=(LPSTR)new TCHAR[n];
		memcpy(Name, name, n);
	} else {
		Name="";
	}

	// �s��
	D3DXMatrixIdentity(&TransformationMatrix);
	D3DXMatrixIdentity(&CombinedTransMatrix);
}

// �t���[���̃f�X�g���N�^
CFrame::~CFrame() {
	if (MeshContainer) delete MeshContainer;
	if (Sibling) delete Sibling;
	if (Child) delete Child;
}


//==============================================================
// ���b�V���R���e�i�̐����Ɣj��

// ���b�V���R���e�i�̐������ɌĂяo�����֐�
HRESULT CAllocateHierarchy::CreateMeshContainer(
	THIS_ LPCSTR name, 
	CONST D3DXMESHDATA *mesh_data,
	CONST D3DXMATERIAL *materials, 
	CONST D3DXEFFECTINSTANCE *effect_instances,
	DWORD num_materials, 
	CONST DWORD *adjacency, 
	LPD3DXSKININFO skin_info, 
	LPD3DXMESHCONTAINER *new_mesh_container
) {
	*new_mesh_container=new CMeshContainer(
		(LPCTSTR)name, mesh_data, materials, num_materials,
		skin_info, adjacency);
	return S_OK;
}

// ���b�V���R���e�i�̔j�����ɌĂяo�����֐�
HRESULT CAllocateHierarchy::DestroyMeshContainer(
	LPD3DXMESHCONTAINER mesh_container
) {
	delete (CMeshContainer*)mesh_container;
	return S_OK;
}

// ���b�V���R���e�i�̃R���X�g���N�^
CMeshContainer::CMeshContainer(
	THIS_ LPCTSTR name,
	CONST D3DXMESHDATA *mesh_data,
	CONST D3DXMATERIAL *materials,
	DWORD num_materials,
	LPD3DXSKININFO skin_info,
	CONST DWORD *adjacency
)
:	Mesh(mesh_data->pMesh), NumMaterials(num_materials), 
	Materials(NULL), Textures(NULL), 
	SkinInfo(skin_info), NumBones(0), BoneOffsetMatrices(0),
	VPMode(VP_SOFTWARE)
{
	ZeroMemory(this, sizeof(D3DXMESHCONTAINER));

	// ���b�V���R���e�i���̐ݒ�
	if (name) {
		int n=lstrlen(name)+1;
		Name=(LPSTR)new TCHAR[n];
		memcpy(Name, name, n);
	} else {
		Name="";
	}

	// ���b�V����ǂݍ���
	if (!(Mesh->GetFVF()&D3DFVF_NORMAL)) {
		Mesh->CloneMeshFVF(
			Mesh->GetOptions(), 
			Mesh->GetFVF() | D3DFVF_NORMAL, 
			Device, &Mesh);
		D3DXComputeNormals(Mesh, NULL);
	} else {
		Mesh->AddRef();
	}

	// �}�e���A���ƃe�N�X�`����ǂݍ���
	Materials=new D3DMATERIAL9[NumMaterials];
	Textures=new LPDIRECT3DTEXTURE9[NumMaterials];
	for (DWORD i=0; i<NumMaterials; i++ ) {

		// �}�e���A�����R�s�[����
		Materials[i]=materials[i].MatD3D;

		// �A���r�G���g�F��ݒ肷��
		Materials[i].Ambient=Materials[i].Diffuse;

		// �e�N�X�`����ǂݍ���
		Textures[i]=NULL;
		char* texture_file=materials[i].pTextureFilename;
		if (texture_file && lstrlen(texture_file)>0) {
			if (FAILED(D3DXCreateTextureFromFile(
				Device, (Path+texture_file).c_str(), 
				&Textures[i]))) 
				continue;
		}
	}

	// �X�L�j���O����ǂݍ���
	if (SkinInfo) {
		SkinInfo->AddRef();
		
		// �{�[���ό`�s��A�{�[���I�t�Z�b�g�s��̍쐬
		NumBones=SkinInfo->GetNumBones();
		BoneTransMatrices=new D3DXMATRIX*[NumBones];
		BoneOffsetMatrices=new D3DXMATRIX[NumBones];
		for (DWORD i=0; i<NumBones; i++) {
			BoneOffsetMatrices[i]=
				*SkinInfo->GetBoneOffsetMatrix(i);
		}

		// �����Ɏg�p����{�[���s��̍ő吔�����߂�
		LPDIRECT3DINDEXBUFFER9 ib=NULL;
		Mesh->GetIndexBuffer(&ib);
		SkinInfo->GetMaxFaceInfluences(
			ib, Mesh->GetNumFaces(), &NumMaxInfls);
		ib->Release();
		NumMaxInfls=min(NumMaxInfls, 12);

		// �C���f�b�N�X�t�����_�u�����f�B���O��
		// �n�[�h�E�F�A���Ή����Ă��邩�ǂ����𒲂ׂ�
		DWORD flags=D3DXMESHOPT_VERTEXCACHE;
		D3DCAPS9 caps;
		Device->GetDeviceCaps(&caps);
		if (caps.MaxVertexBlendMatrixIndex+1<NumMaxInfls) {
			
			// �\�t�g�E�F�A���g�p����ꍇ
			NumPaletteEntries=min(256, NumBones);
			VPMode=VP_SOFTWARE;
			flags|=D3DXMESH_SYSTEMMEM;

		} else {

			// �n�[�h�E�F�A���g�p����ꍇ
			NumPaletteEntries=min(
				(caps.MaxVertexBlendMatrixIndex+1)/2, 
				NumBones);
			VPMode=VP_INDEXED;
			flags|=D3DXMESH_MANAGED;
		}

		// �X�L���Ή��̃��b�V�����쐬����
		SkinInfo->ConvertToIndexedBlendedMesh(
			Mesh, flags, NumPaletteEntries, adjacency, 
			NULL, NULL, NULL, &NumMaxInfls, 
			&NumBoneCombinations, &BoneCombinations, 
			&Mesh);
	}


/*	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		UINT MaxMatrices = 26; 
		pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
		if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}
		else
		{
			pMeshContainer->UseSoftwareVP = true;
			g_bUseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
												(
												pMeshContainer->pOrigMesh,
												Flags, 
												pMeshContainer->NumPaletteEntries, 
												pMeshContainer->pAdjacency, 
												NULL, NULL, NULL,			 
												&pMeshContainer->NumInfl,
												&pMeshContainer->NumAttributeGroups, 
												&pMeshContainer->pBoneCombinationBuf, 
												&pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;


		// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
		DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pd3dDevice, &pMesh);
			if (!FAILED(hr))
			{
				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMesh;
				pMesh = NULL;
			}
		}

		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		LPD3DVERTEXELEMENT9 pDeclCur;
		hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
		//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
		//		  this is more of a "cast" operation
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
		if( g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
		{
			g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

			// Allocate space for blend matrices
			delete[] g_pBoneMatrices; 
			g_pBoneMatrices  = new D3DXMATRIXA16[g_NumBoneMatricesMax];
			if( g_pBoneMatrices == NULL )
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}
		}
	}
*/
}

// ���b�V���R���e�i�̃f�X�g���N�^
CMeshContainer::~CMeshContainer() {
	if (Mesh) Mesh->Release();
	if (Materials) delete[] Materials;
	if (Textures) {
		for (DWORD i=0; i<NumMaterials; i++) {
			if (Textures[i]) Textures[i]->Release();
		}
		delete[] Textures;
	}
	if (SkinInfo) SkinInfo->Release();
	if (BoneOffsetMatrices) delete[] BoneOffsetMatrices;
}


//==============================================================
// �t���[���̕`��
void CFrame::Draw() {
	
	// �t���[���ɑ�����S�Ẵ��b�V���R���e�i��`��
	for (CMeshContainer* mc=MeshContainer; mc; mc=mc->Next()) {
		MeshContainer->Draw(this);
	}

	// �Z��t���[���A�q�t���[���̕`��
	if (Sibling) Sibling->Draw();
	if (Child) Child->Draw();
}


//==============================================================
// ���b�V���R���e�i�̕`��
void CMeshContainer::Draw(CFrame* frame) {

	// �X�L�j���O���s���ꍇ�̕`��
	if (SkinInfo) {

		// �`������̐ݒ�
		if (VPMode==VP_SOFTWARE) {
			Device->SetSoftwareVertexProcessing(TRUE);
		}
		if (NumMaxInfls==1) {
			Device->SetRenderState(
				D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
		} else {
			Device->SetRenderState(
				D3DRS_VERTEXBLEND, NumMaxInfls-1);
		}
		if (NumMaxInfls) {
			Device->SetRenderState(
				D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
		}

		// �{�[���R���r�l�[�V����
		// �i�}�e���A���A�e�N�X�`���A�{�[���s��
		// ��v����|���S���̑g�j
		// ���Ƃɕ����ĕ`�悷��
		LPD3DXBONECOMBINATION bc=
			reinterpret_cast<LPD3DXBONECOMBINATION>(
				BoneCombinations->GetBufferPointer());
		D3DXMATRIXA16 mat;
		for (DWORD ibc=0; ibc<NumBoneCombinations; ibc++) {

			// �{�[���s��p���b�g�̐ݒ�
			for (DWORD ip=0; ip<NumPaletteEntries; ip++) {
				DWORD bone=bc[ibc].BoneId[ip];
				if (bone!=UINT_MAX) {
					
					// �{�[���I�t�Z�b�g�s��ƃ{�[���ό`�s��
					// ���|���ăp���b�g�ɐݒ肷��
					D3DXMatrixMultiply(
						&mat, &BoneOffsetMatrices[bone], 
						BoneTransMatrices[bone]);
					Device->SetTransform(
						D3DTS_WORLDMATRIX(ip), &mat);
				}
			}

			// �}�e���A���̐ݒ�
			Device->SetMaterial(&Materials[bc[ibc].AttribId]);
			Device->SetTexture(0, Textures[bc[ibc].AttribId]);

			// �|���S���̕`��
			Mesh->DrawSubset(ibc);
		}

		// �`������̕��A
		Device->SetRenderState(
			D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		Device->SetRenderState(
			D3DRS_VERTEXBLEND, 0);
		if (VPMode==VP_SOFTWARE) {
			Device->SetSoftwareVertexProcessing(FALSE);
		}

/*
        else if (g_SkinningMethod == D3DINDEXEDHLSLVS) 
        {
            if (pMeshContainer->UseSoftwareVP)
            {
                // If hw or pure hw vertex processing is forced, we can't render the
                // mesh, so just exit out.  Typical applications should create
                // a device with appropriate vertex processing capability for this
                // skinning method.
                if( g_dwBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
                    return;

                V( pd3dDevice->SetSoftwareVertexProcessing(TRUE) );
            }

            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            { 
                // first calculate all the world matrices
                for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if (iMatrixIndex != UINT_MAX)
                    {
                        D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
                        D3DXMatrixMultiply(&g_pBoneMatrices[iPaletteEntry], &matTemp, &g_matView);
                    }
                }
                V( g_pEffect->SetMatrixArray( "mWorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries) );

                // Sum of all ambient and emissive contribution
                D3DXCOLOR color1(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient);
                D3DXCOLOR color2(.25, .25, .25, 1.0);
                D3DXCOLOR ambEmm;
                D3DXColorModulate(&ambEmm, &color1, &color2);
                ambEmm += D3DXCOLOR(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Emissive);

                // set material color properties 
                V( g_pEffect->SetVector("MaterialDiffuse", (D3DXVECTOR4*)&(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse)) );
                V( g_pEffect->SetVector("MaterialAmbient", (D3DXVECTOR4*)&ambEmm) );

                // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
                V( pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] ) );

                // Set CurNumBones to select the correct vertex shader for the number of bones
                V( g_pEffect->SetInt( "CurNumBones", pMeshContainer->NumInfl -1) );

                // Start the effect now all parameters have been updated
                UINT numPasses;
                V( g_pEffect->Begin( &numPasses, D3DXFX_DONOTSAVESTATE ) );
                for( UINT iPass = 0; iPass < numPasses; iPass++ )
                {
                    V( g_pEffect->BeginPass( iPass ) );

                    // draw the subset with the current world matrix palette and material state
                    V( pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib ) );

                    V( g_pEffect->EndPass() );
                }

                V( g_pEffect->End() );

                V( pd3dDevice->SetVertexShader(NULL) );
            }

            // remember to reset back to hw vertex processing if software was required
            if (pMeshContainer->UseSoftwareVP)
            {
                V( pd3dDevice->SetSoftwareVertexProcessing(FALSE) );
            }
        }
*/
	}

	// �X�L�j���O���s��Ȃ��ꍇ�̕`��
	else {

		// �s��̐ݒ�
		Device->SetTransform(
			D3DTS_WORLD, frame->GetCombinedTransMatrix());

		// �}�e���A���A�e�N�X�`������v����|���S���̑g
		// ���Ƃɕ����ĕ`�悷��
		for (DWORD i=0; i<NumMaterials; i++) {
			Device->SetMaterial(&Materials[i]);
			Device->SetTexture(0, Textures[i]);
			Mesh->DrawSubset(i);
		}
	}	
}


//==============================================================
// �{�[���ό`�s��̐ݒ�ƃA�b�v�f�[�g

// �t���[���̌���
CFrame* CFrame::Find(LPCTSTR name) {
	return (CFrame*)D3DXFrameFind(this, (LPCSTR)name);
}

// �{�[���ό`�s��̏������i�t���[���j
void CFrame::InitBoneTransMatrices(CFrame* root) {
	
	// ���b�V���R���e�i�A�Z��t���[���A�q�t���[���ւ�
	// �|�C���^�̏�����
	MeshContainer=(CMeshContainer*)pMeshContainer;
	Sibling=(CFrame*)pFrameSibling;
	Child=(CFrame*)pFrameFirstChild;

	// ���b�V���R���e�i�A�Z��t���[���A�q�t���[���Ɋւ���
	// �{�[���ό`�s��̏�����
	if (MeshContainer) MeshContainer->InitBoneTransMatrices(root);
	if (Sibling) Sibling->InitBoneTransMatrices(root);
	if (Child) Child->InitBoneTransMatrices(root);
}

// �{�[���ό`�s��̏������i���b�V���R���e�i�j
void CMeshContainer::InitBoneTransMatrices(CFrame* root) {
	if (SkinInfo) {
		
		// �S�Ẵ{�[���ɂ��āA�Ή�����t���[����T��
		for (DWORD i=0; i<NumBones; i++) {
			CFrame* frame=root->Find((LPCTSTR)SkinInfo->GetBoneName(i));
			
			// �Ή�����t���[���̍����ςݕό`�s���
			// �{�[������Q�Ƃ���
			if (frame) {
				BoneTransMatrices[i]=
					frame->GetCombinedTransMatrix();
			}
		}
	}
}

// �ό`�s��̍X�V
void CFrame::UpdateMatrices(LPD3DXMATRIX parent_mat) {

	// ���̃t���[���Ɋւ���ό`�s��̍X�V
	if (parent_mat) {
		D3DXMatrixMultiply(
			&CombinedTransMatrix, 
			&TransformationMatrix, parent_mat);
	} else {
		CombinedTransMatrix=TransformationMatrix;
	}

	// �Z��t���[���A�q�t���[���Ɋւ���ό`�s��̍X�V
	if (Sibling) Sibling->UpdateMatrices(parent_mat);
	if (Child) Child->UpdateMatrices(&CombinedTransMatrix);
}


//==============================================================
// �A�j���[�V�����Ή����b�V���iCAnimMesh�N���X�j

// �R���X�g���N�^
CAnimMesh::CAnimMesh(LPDIRECT3DDEVICE9 device)
:	Device(device)
{
	if (!AllocateHierarchy) 
		AllocateHierarchy=new CAllocateHierarchy();
	if (!AnimMeshEffect) LoadAnimMeshEffect(device);
}

// �f�X�g���N�^
CAnimMesh::~CAnimMesh() {
	if (AnimController) AnimController->Release();
	delete FrameRoot;
}

// �t�@�C���̃��[�h
void CAnimMesh::LoadFromFile(string file) {

	// Direct3D�f�o�C�X�ƃt�@�C���p�X���O���[�o���ϐ��ɐݒ�
	::Device=Device;
	::Path=ExtractFilePath(file);

	// ���b�V���̃��[�h
	LPD3DXFRAME frame_root;
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		file.c_str(), D3DXMESH_MANAGED, 
		Device, AllocateHierarchy, 
		NULL, &frame_root, &AnimController))) return;
	FrameRoot=(CFrame*)frame_root;

	// �{�[���ό`�s��̐ݒ�
	FrameRoot->InitBoneTransMatrices(FrameRoot);
}


//==============================================================
// �A�j���[�V�����iCAnimMesh�N���X�j

// ���Ԃ̎擾
double CAnimMesh::GetAnimTime() {
	if (AnimController) return AnimController->GetTime();
	return 0;
}

// ���Ԃ̐ݒ�
void CAnimMesh::AdvanceAnimTime(double time) {
	if (AnimController) AnimController->AdvanceTime(time, NULL);
}

// �A�j���[�V�����Z�b�g�̐ݒ�
void CAnimMesh::SetAnimSet(DWORD track, DWORD set) {
	if (AnimController) {
		LPD3DXANIMATIONSET aset;
		AnimController->GetAnimationSet(set, &aset);
		AnimController->SetTrackAnimationSet(track, aset);
	}
}

// �A�j���[�V�����Z�b�g���̎擾
DWORD CAnimMesh::GetAnimSetCount() {
	if (AnimController) return AnimController->GetNumAnimationSets();
	return 0;
}

// �g���b�N�̗L����
void CAnimMesh::SetAnimEnabled(DWORD track, bool enabled) {
	if (AnimController) 
		AnimController->SetTrackEnable(track, enabled);
}

// �g���b�N�̏d�݂̐ݒ�
void CAnimMesh::SetAnimWeight(DWORD track, float weight) {
	if (AnimController) 
		AnimController->SetTrackWeight(track, weight);
}

// �g���b�N�̑��x�̐ݒ�
void CAnimMesh::SetAnimSpeed(DWORD track, float speed) {
	if (AnimController) 
		AnimController->SetTrackSpeed(track, speed);
}

// �g���b�N���̎擾
DWORD CAnimMesh::GetAnimNumTracks() {
	if (AnimController) return AnimController->GetMaxNumTracks();
	return 0;
}


//==============================================================
// �`��

// �ϊ��s����w�肵�ĕ`��
void CAnimMesh::Draw(LPD3DXMATRIX mat) {
	::Device=Device;
	FrameRoot->UpdateMatrices(mat);
	FrameRoot->Draw();
}

// �f�t�H���g�̍s��i�P�ʍs��j�ŕ`��
void CAnimMesh::Draw() {
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	Draw(&mat);
}

#define TURN_X \
	if (tx) {\
		D3DXMatrixRotationX(&mat0, D3DX_PI*2*tx);\
		D3DXMatrixMultiply(&mat, &mat, &mat0);\
	}
#define TURN_Y \
	if (ty) {\
		D3DXMatrixRotationY(&mat0, D3DX_PI*2*ty);\
		D3DXMatrixMultiply(&mat, &mat, &mat0);\
	}
#define TURN_Z \
	if (tz) {\
		D3DXMatrixRotationZ(&mat0, D3DX_PI*2*tz);\
		D3DXMatrixMultiply(&mat, &mat, &mat0);\
	}

// ���W�A�T�C�Y�A��]���w�肵�ĕ`��
void CAnimMesh::Draw(
	float x, float y, float z, 
	float sx, float sy, float sz, 
	float tx, float ty, float tz, TURN_ORDER order
) {
	D3DXMATRIX mat, mat0;
	
	// �g��k��
	D3DXMatrixScaling(&mat, sx, sy, sz);

	// ��]
	switch (order) {
		case TO_ZY: TURN_Z TURN_Y break;
		case TO_X: TURN_X break;
		case TO_Y: TURN_Y break;
		case TO_Z: TURN_Z break;
		case TO_XY: TURN_X TURN_Y break;
		case TO_XZ: TURN_X TURN_Z break;
		case TO_YX: TURN_Y TURN_X break;
		case TO_YZ: TURN_Y TURN_Z break;
		case TO_ZX: TURN_Z TURN_X break;
		case TO_XYZ: TURN_X TURN_Y TURN_Z break;
		case TO_XZY: TURN_X TURN_Z TURN_Y break;
		case TO_YXZ: TURN_Y TURN_X TURN_Z break;
		case TO_YZX: TURN_Y TURN_Z TURN_X break;
		case TO_ZXY: TURN_Z TURN_X TURN_Y break;
		case TO_ZYX: TURN_Z TURN_Y TURN_X break;
	}

	// ���W
	D3DXMatrixTranslation(&mat0, x, y, z);
	D3DXMatrixMultiply(&mat, &mat, &mat0);
	
	// �`��
	Device->SetTransform(D3DTS_WORLD, &mat);
	Draw(&mat);
}
