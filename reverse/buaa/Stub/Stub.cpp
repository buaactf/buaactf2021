// Stub.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "Stub.h"
#include "stdlib.h"

#pragma comment(linker, "/merge:.data=.text")        // 将.data合并到.text
#pragma comment(linker, "/merge:.rdata=.text")       // 将.rdata合并到.text
#pragma comment(linker, "/section:.text,RWE")        // 将.text段的属性设置为可读、可写、可执行

#pragma comment(linker, "/entry:\"StubEntryPoint\"") // 指定程序入口函数为StubEntryPoint()

// 这是导出变量的一个示例
STUB_API int nStub=0;

// 这是导出函数的一个示例。
STUB_API int fnStub(void)
{
    return 0;
}

// 这是已导出类的构造函数。
CStub::CStub()
{
    return;
}

STUB_API int fnGetStub(void)
{
	return nStub;
}

STUB_API void fnSetStub(int n)
{
	nStub = n;
}

extern __declspec(dllexport) GLOBAL_PARAM g_stcParam = { 0 };
extern __declspec(dllexport) SHARE ShareData;                    //导出变量，用于壳代码与加壳器之间的通讯
void start()
{
	//这里存放对被加壳文件的操作
	// 1. 初始化所有API
	if (!InitializationAPI())  return;

	// 2. 解密宿主程序
	Decrypt();
	Debugging();                    //反调试

	// 3. 跳转到OEP
	__asm jmp g_stcParam.dwOEP;
}

void __declspec(naked) StubEntryPoint()
{
	__asm sub esp, 0x50;        // 抬高栈顶，提高兼容性
	start();                    // 执行壳的主体部分
	__asm add esp, 0x50;        // 平衡堆栈

	if (g_funExitProcess)
	{
		g_funExitProcess(0);
	}

	__asm retn;
}

DWORD GetKernel32Base()
{
	DWORD dwKernel32Addr = 0;
	__asm
	{
		push eax
		mov eax, dword ptr fs : [0x30] // eax = PEB的地址
		mov eax, [eax + 0x0C]          // eax = 指向PEB_LDR_DATA结构的指针
		mov eax, [eax + 0x1C]          // eax = 模块初始化链表的头指针InInitializationOrderModuleList
		mov eax, [eax]               // eax = 列表中的第二个条目
		mov eax, [eax + 0x08]          // eax = 获取到的Kernel32.dll基址（Win7下获取的是KernelBase.dll的基址）
		mov dwKernel32Addr, eax
		pop eax
	}

	return dwKernel32Addr;
}

DWORD GetGPAFunAddr()
{
	DWORD dwAddrBase = GetKernel32Base();

	// 1. 获取DOS头、NT头
	PIMAGE_DOS_HEADER pDos_Header;
	PIMAGE_NT_HEADERS pNt_Header;
	pDos_Header = (PIMAGE_DOS_HEADER)dwAddrBase;
	pNt_Header = (PIMAGE_NT_HEADERS)(dwAddrBase + pDos_Header->e_lfanew);

	// 2. 获取导出表项
	PIMAGE_DATA_DIRECTORY   pDataDir;
	PIMAGE_EXPORT_DIRECTORY pExport;
	pDataDir = pNt_Header->OptionalHeader.DataDirectory;
	pDataDir = &pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT];
	pExport = (PIMAGE_EXPORT_DIRECTORY)(dwAddrBase + pDataDir->VirtualAddress);


	// 3、获取导出表的必要信息
	DWORD dwModOffset = pExport->Name;									// 模块的名称
	DWORD dwFunCount = pExport->NumberOfFunctions;						// 导出函数的数量
	DWORD dwNameCount = pExport->NumberOfNames;							// 导出名称的数量

	PDWORD pEAT = (PDWORD)(dwAddrBase + pExport->AddressOfFunctions);	// 获取地址表的RVA
	PDWORD pENT = (PDWORD)(dwAddrBase + pExport->AddressOfNames);		// 获取名称表的RVA
	PWORD pEIT = (PWORD)(dwAddrBase + pExport->AddressOfNameOrdinals);	//获取索引表的RVA

	// 4、获取GetProAddress函数的地址
	for (DWORD i = 0; i < dwFunCount; i++)
	{
		if (!pEAT[i])
		{
			continue;
		}
		// 4.1 获取序号
		DWORD dwID = pExport->Base + i;

		// 4.2 变量EIT 从中获取到 GetProcAddress的地址
		for (DWORD dwIdx = 0; dwIdx < dwNameCount; dwIdx++)
		{
			// 序号表中的元素的值 对应着函数地址表的位置

			if (pEIT[dwIdx] == i)
			{
				//根据序号获取到名称表中的名字
				DWORD dwNameOffset = pENT[dwIdx];
				char * pFunName = (char*)(dwAddrBase + dwNameOffset);
				//判断是否是GetProcAddress函数
				if (!strcmp(pFunName, "GetProcAddress"))
				{
					// 获取EAT的地址 并将GetProcAddress地址返回
					DWORD dwFunAddrOffset = pEAT[i];
					return dwAddrBase + dwFunAddrOffset;
				}
			}
		}
	}
	return -1;
}

LPGETPROCADDRESS  g_funGetProcAddress = nullptr;
LPLOADLIBRARYEX   g_funLoadLibraryEx = nullptr;

LPEXITPROCESS     g_funExitProcess = nullptr;
LPMESSAGEBOX      g_funMessageBox = nullptr;
LPGETMODULEHANDLE g_funGetModuleHandle = nullptr;
LPVIRTUALPROTECT  g_funVirtualProtect = nullptr;

LPISDEBUGGERPRESENT  g_funIsDebuggerPresent = nullptr;
LPGETWINDOWTEXTA g_funGetWindowTextA = nullptr;
LPGETFOREGROUNDWINDOWA g_funGetForegroundWindowA = nullptr;
LPSTRSTR g_funstrstr = nullptr;
bool InitializationAPI()
{
	HMODULE hModule;

	// 1. 初始化基础API 这里使用的是LoadLibraryExW
	g_funGetProcAddress = (LPGETPROCADDRESS)GetGPAFunAddr();
	g_funLoadLibraryEx = (LPLOADLIBRARYEX)g_funGetProcAddress((HMODULE)GetKernel32Base(), "LoadLibraryExW");

	// 2. 初始化其他API
	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL)))  return false;
	g_funExitProcess = (LPEXITPROCESS)g_funGetProcAddress(hModule, "ExitProcess");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"user32.dll", NULL, NULL)))  return false;
	g_funMessageBox = (LPMESSAGEBOX)g_funGetProcAddress(hModule, "MessageBoxW");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL)))  return false;
	g_funGetModuleHandle = (LPGETMODULEHANDLE)g_funGetProcAddress(hModule, "GetModuleHandleW");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL)))  return false;
	g_funVirtualProtect = (LPVIRTUALPROTECT)g_funGetProcAddress(hModule, "VirtualProtect");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL)))  return false;
	g_funIsDebuggerPresent = (LPISDEBUGGERPRESENT)g_funGetProcAddress(hModule, "IsDebuggerPresent");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"user32.dll", NULL, NULL)))  return false;
	g_funGetWindowTextA = (LPGETWINDOWTEXTA)g_funGetProcAddress(hModule, "GetWindowTextA");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"user32.dll", NULL, NULL)))  return false;
	g_funGetForegroundWindowA = (LPGETFOREGROUNDWINDOWA)g_funGetProcAddress(hModule, "GetForegroundWindow");

	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"shlwapi.dll", NULL, NULL)))  return false;
	g_funstrstr = (LPSTRSTR)g_funGetProcAddress(hModule, "StrStrA");

	return true;
}

void Decrypt()
{
	// 在导出的全局变量中读取需解密区域的起始于结束VA
	PBYTE lpStart = g_stcParam.lpStartVA;
	PBYTE lpEnd = g_stcParam.lpEndVA;
	int num[] = { 0x15,0x16,0x17, 0x18, 0x19,0xAA,0xBB, 0xCC, 0xDD, 0xEE };
	int count = 0;
	// 循环解密
	while (lpStart < lpEnd)
	{
		*lpStart = *lpStart ^ num[count % 10];
		lpStart++;
		count++;
	}
}

void Debugging() 
{
	char sz_wnd_text[260];
	const char* sz_wnd_ary[] =
	{
		"Ddg",
		"Olly",
		"WinDbg",
		"x64_dbg",
		"OllyICE",
		"OllyDBG",
		"Immunity",
		"吾爱破解"
	};

	if (g_funIsDebuggerPresent())
	{
		g_funExitProcess(0);
	}
	g_funGetWindowTextA(g_funGetForegroundWindowA(), sz_wnd_text, 1023);
	for (int n_index = 0; n_index < 8; n_index++)
	{
		if (0 != g_funstrstr(sz_wnd_text, sz_wnd_ary[n_index]))
		{
			g_funExitProcess(0);
		}

	}
}
