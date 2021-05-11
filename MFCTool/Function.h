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
