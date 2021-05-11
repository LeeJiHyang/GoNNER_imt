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
	_vec3 vPos; // ��ġ ���� 
	_vec3 vDir; // ���� ����
	_vec3 vSize; // ũ��, �浹ü�� ���
	_vec3 vScale; // ����
	_vec3 vLook;
	_vec3 vRot; // ����
	_vec3 vRev; // ���� - �������ؼ� �����ȵ��� ��
	_matrix matWorld;
	_matrix matParent;
	_color color;
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture; // �̹��� �� ���� �����ϱ� ���� �İ�ü
	D3DXIMAGE_INFO tImageInfo; //�̹����� ������ ��Ƶ� ����ü
}TEXINFO;

typedef struct tagLine
{
	_vec3		vPos; // ��ġ
	LINE_DIR	dir;// 0: 0   (��)
						// 1: 90  (��) 
						// 2: 180 (��)
						// 3: 270 (��)
	int			iImageNum[3] = { 0, 0, 0 }; // �̹��� (���������� 0~5 ����1)
	bool		bDraw[3] = { false, false, false };
	// Angle
	_vec3		vAngle[3] = { {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} }; // ��ǥ ���� (���� �׸��� ����)
	_vec3		vShowAngle[3]{ {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} }; // ���� ����
	_vec3		vStartAngle[3]{ {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} }; // ������ �� ����Ǿ��ִ� ����
	// Scale
	_vec3		vScale[3]; // ��ǥ ������
	_vec3		vShowScale[3]; // ���� ������
	_vec3		vStartScale[3]; // ������ �� ������
	_color		color;

} LINE;

enum PlayState
{
	ANI_STOP,	//����
	ANI_LOOP,	//������ ����
	ANI_NOLOOP,	//������ 1�����
	ANI_REVERSE	//������ ����
};
