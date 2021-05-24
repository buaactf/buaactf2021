// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 STUB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// STUB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef STUB_EXPORTS
#define STUB_API __declspec(dllexport)
#else
#define STUB_API __declspec(dllimport)
#endif

// 此类是从 dll 导出的
class STUB_API CStub {
public:
	CStub(void);
	// TODO: 在此处添加方法。
};

extern STUB_API int nStub;

typedef struct _GLOBAL_PARAM
{
	BOOL  bShowMessage; // 是否显示解密信息
	DWORD dwOEP;        // 程序入口点
	PBYTE lpStartVA;    // 起始虚拟地址（被异或加密区）
	PBYTE lpEndVA;      // 结束虚拟地址（被异或加密区）
}GLOBAL_PARAM, *PGLOBAL_PARAM;

extern "C" __declspec(dllexport) GLOBAL_PARAM g_stcParam;

extern "C"
{
	//获取和设置Stub
	STUB_API int fnGetStub(void);
	STUB_API void fnSetStub(int n);
}

STUB_API int fnStub(void);

extern DWORD GetKernel32Base();   // 获取Kernel32.dll的模块基址
extern DWORD GetGPAFunAddr();     // 获取GetProcAddress的函数地址


extern bool  InitializationAPI(); // 初始化各个API

// 基础API定义声明
typedef DWORD(WINAPI *LPGETPROCADDRESS)(HMODULE, LPCSTR);        // GetProcAddress
typedef HMODULE(WINAPI *LPLOADLIBRARYEX)(LPCTSTR, HANDLE, DWORD); // LoadLibaryEx
extern LPGETPROCADDRESS g_funGetProcAddress;
extern LPLOADLIBRARYEX  g_funLoadLibraryEx;


// 其他API定义声明
typedef VOID(WINAPI *LPEXITPROCESS)(UINT);                          // ExitProcess
typedef int (WINAPI *LPMESSAGEBOX)(HWND, LPCTSTR, LPCTSTR, UINT);       // MessageBox
typedef HMODULE(WINAPI *LPGETMODULEHANDLE)(LPCWSTR);                // GetModuleHandle
typedef BOOL(WINAPI *LPVIRTUALPROTECT)(LPVOID, SIZE_T, DWORD, PDWORD); // VirtualProtect
typedef BOOL(WINAPI *LPISDEBUGGERPRESENT)(VOID);                          // IsDebuggerPresent
typedef int(WINAPI *LPGETWINDOWTEXTA)(HWND, LPSTR, INT);
typedef HWND(WINAPI *LPGETFOREGROUNDWINDOWA)();
typedef PTSTR(WINAPI *LPSTRSTR)(LPSTR, LPCSTR);

extern LPEXITPROCESS     g_funExitProcess;
extern LPMESSAGEBOX      g_funMessageBox;
extern LPGETMODULEHANDLE g_funGetModuleHandle;
extern LPVIRTUALPROTECT  g_funVirtualProtect;
extern LPISDEBUGGERPRESENT  g_funIsDebuggerPresent;
extern LPGETWINDOWTEXTA g_funGetWindowTextA;
extern LPGETFOREGROUNDWINDOWA g_funGetForegroundWindowA;
extern LPSTRSTR g_funstrstr;


extern void  Decrypt();           // 解密函数
extern void  Debugging();           // 反调试

struct SHARE {
	int DestOEP;
	int ImpTableVA;
	int dwRelocRva;
	int dwRelocSize;
	int OldImageBase;
	int TlsVirtualAddress;
	int TlsCallBackTableVa;
	int TlsDataSize;



	int UncompressAddr;
	int TextCompressSize;
	int TextRVA;
	int SizeOfRawData;
};
