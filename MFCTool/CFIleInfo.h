#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

	static CString ConvertRelativePath(const CString& strAbsolutePath);
};

