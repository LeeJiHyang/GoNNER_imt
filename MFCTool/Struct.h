#pragma once

typedef struct tagInfo
{
	tagInfo() {
		vPos = { 0.f, 0.f, 0.f };
		vDir = { 1.f, 1.f, 0.f };
		vSize = { 1.f, 1.f, 0.f };
		vLook = { 1.f, 0.f, 0.f };
		vRot = { 0.f, 0.f, 0.f };
		vRev = { 0.f, 0.f, 0.f };
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matParent);
		color = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
	_vec3 vPos;  // ��ġ���� 
	_vec3 vDir; // ���⺤�� 
	_vec3 vSize; // ũ��
	_vec3 vLook;
	_vec3 vRot; // ����
	_vec3 vRev; // ���� - �������ؼ� �����ȵ��� ��
	_matrix matWorld;
	_matrix matParent;
	_color color;
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture; // �̹��� ������ �����ϱ� ���� �İ�ü. 
	D3DXIMAGE_INFO tImageInfo; //�̹����� ������ ��Ƶ� ����ü. 
}TEXINFO;

typedef struct tagLine
{
	tagLine() {
		vPos = { 0.f, 0.f, 0.f };
		dir = LINE_DIR_U;
	}
	_vec3		vPos; // ��ġ
	LINE_DIR	dir;// 0: 0   (��)
						// 1: 90  (��) 
						// 2: 180 (��)
						// 3: 270 (��)
} LINE;
