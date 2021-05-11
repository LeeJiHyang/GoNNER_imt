#include "stdafx.h"
#include "CFIleInfo.h"

CFileInfo::CFileInfo()
{
}

CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString & strAbsolutePath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szCurDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirectory);

	PathRelativePathTo(
		szRelativePath, // 최종 결과물
		szCurDirectory,
		FILE_ATTRIBUTE_DIRECTORY,
		strAbsolutePath,
		FILE_ATTRIBUTE_DIRECTORY
	);// 드라이브가 다르면 안됨

	return CString(szRelativePath);
}
