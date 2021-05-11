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
	_vec3 vPos;  // 위치벡터 
	_vec3 vDir; // 방향벡터 
	_vec3 vSize; // 크기
	_vec3 vLook;
	_vec3 vRot; // 자전
	_vec3 vRev; // 공전 - 구현안해서 될지안될지 모름
	_matrix matWorld;
	_matrix matParent;
	_color color;
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture; // 이미지 한장을 제어하기 위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo; //이미지의 정보를 담아둘 구조체. 
}TEXINFO;

typedef struct tagLine
{
	tagLine() {
		vPos = { 0.f, 0.f, 0.f };
		dir = LINE_DIR_U;
	}
	_vec3		vPos; // 위치
	LINE_DIR	dir;// 0: 0   (상)
						// 1: 90  (우) 
						// 2: 180 (하)
						// 3: 270 (좌)
} LINE;
