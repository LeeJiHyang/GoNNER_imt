#pragma once
#ifdef _AFX
#define ERR_MSG(Message) AfxMessageBox(Message)
#else
#define ERR_MSG(Message) MessageBox(nullptr,Message,L"System_Error",MB_OK)
#endif
extern HWND g_hWND; 
#define WINCX 800
#define WINCY 600
#define TILECX 128
#define TILECY 128
#define TILEGAP	120
#define TILEX 38
#define TILEY 11

#define ITEM_SWORD	0x01
#define	ITEM_STICK 	0x02
#define	ITEM_KNIFE	0x04

#define DECLARE_SINGLETON(ClassName)							\
public:															\
static ClassName* GetInstance()								\
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