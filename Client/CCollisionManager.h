#pragma once
class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();
public:
	static bool CollisionRect(list<RECT*>& rDstList, list<CObj*>& rSrcList, CObj** pObj = nullptr);
	static void CollisionRect(list<CObj*>& rDstList, list<CObj*>& rSrcList);
	static void CollisionRect(vector<CObj*>& rDstList, list<CObj*>& rSrcList);
	static void CollisionRect(vector<RECT*>& rDstList, list<CObj*>& rSrcList);
	static void CollisionSphere(list<CObj*>& rDstList, list<CObj*>& rSrcList);
	static void CollisionRectEX(list<CObj*>& rDstList, list<CObj*>& rSrcList);
	static bool CollisionRectEX(vector<RECT>& rDstList, list<CObj*>& rSrcList, CObj* pObj = nullptr);

private:
	static bool CheckRect(CObj * pDstObject, CObj & rSrcObject, float * pMoveX, float * pMoveY);
	static bool CheckRect(RECT * pDstRECT, CObj & rSrcObject, float * pMoveX, float * pMoveY);
	static bool CheckSphere(CObj* pDstObject, CObj& rSrcObject);
};
