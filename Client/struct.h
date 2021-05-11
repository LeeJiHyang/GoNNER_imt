#pragma once

typedef struct tagInfo
{
	tagInfo() {
		vPos = { 0.f, 0.f, 0.f };
		vDir = { 1.f, 1.f, 0.f };
		vSize = { 1.f, 1.f, 0.f };
		vScale = { 1.f, 1.f, 0.f };
		vLook = { 1.f, 0.f, 0.f };
		vRot = { 0.f, 0.f, 0.f };
		vRev = { 0.f, 0.f, 0.f };
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matParent);
		color = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
	_vec3 vPos; // 위치 벡터 
	_vec3 vDir; // 방향 벡터
	_vec3 vSize; // 크기, 충돌체로 사용
	_vec3 vScale; // 비율
	_vec3 vLook;
	_vec3 vRot; // 자전
	_vec3 vRev; // 공전 - 구현안해서 될지안될지 모름
	_matrix matWorld;
	_matrix matParent;
	_color color;
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture; // 이미지 한 장을 제어하기 위한 컴객체
	D3DXIMAGE_INFO tImageInfo; //이미지의 정보를 담아둘 구조체
}TEXINFO;

typedef struct tagLine
{
	_vec3		vPos; // 위치
	LINE_DIR	dir;// 0: 0   (상)
						// 1: 90  (우) 
						// 2: 180 (하)
						// 3: 270 (좌)
	int			iImageNum[3] = { 0, 0, 0 }; // 이미지 (생성시점에 0~5 랜덤1)
	bool		bDraw[3] = { false, false, false };
	// Angle
	_vec3		vAngle[3] = { {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} }; // 목표 각도 (최종 그리는 각도)
	_vec3		vShowAngle[3]{ {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} }; // 현재 각도
	_vec3		vStartAngle[3]{ {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} }; // 시작할 때 변경되어있는 각도
	// Scale
	_vec3		vScale[3]; // 목표 스케일
	_vec3		vShowScale[3]; // 현재 스케일
	_vec3		vStartScale[3]; // 시작할 때 스케일
	_color		color;

} LINE;

enum PlayState
{
	ANI_STOP,	//정지
	ANI_LOOP,	//정방향 루프
	ANI_NOLOOP,	//정방향 1번재생
	ANI_REVERSE	//역방향 루프
};
