#pragma once

extern HWND g_hWND; 

#ifdef _AFX
#define ERR_MSG(Message) AfxMessageBox(Message)
#else
#define ERR_MSG(Message) MessageBox(nullptr, Message, L"System_Error", MB_OK)
#endif

#define DECLARE_SINGLETON(ClassName)							\
public:															\
static ClassName* GetInstance()									\
{																\
	if (nullptr == m_pInstance)									\
		m_pInstance = new ClassName;							\
	return m_pInstance;											\
}																\
static void DestroyInstance()									\
{																\
	if (m_pInstance)											\
	{															\
		delete m_pInstance;										\
		m_pInstance = nullptr;									\
	}															\
}																\
private:														\
	static ClassName* m_pInstance;				

#define IMPLEMENT_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr;