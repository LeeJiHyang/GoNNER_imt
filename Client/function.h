#pragma once

template<class T>
void SafeDelete(T& rObj)
{
	if (rObj)
	{
		delete rObj;
		rObj = nullptr;
	}
}
template<class T>
void SafeDeleteArray(T& rObj)
{
	if (rObj)
	{
		delete[] rObj;
		rObj = nullptr;
	}
}
template<class T>
void SafeRelease(T& rObj)
{
	if (rObj)
		rObj->Release();
}

template<class T>
void SafeReleaseDelete(T& rObj)
{
	if (rObj) {
		rObj->Release();
		delete rObj;
		rObj = nullptr;
	}

}
static int GetCX(RECT& rc) { return (rc.right - rc.left); }
static int GetCY(RECT& rc) { return (rc.bottom - rc.top); }

static float GetDistance(float x1, float y1, float x2, float y2)
{			//∑Á∆Æ   a2 + b2    =c
	return sqrt((x1 - x2)*(x1 - x2) + (y2 - y1)*(y2 - y1));
}

static float GetDistance(_vec3 v1, _vec3 v2)
{
	return GetDistance(v1.x, v1.y, v2.x, v2.y);
}