// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PACKDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PACKDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef PACKDLL_EXPORTS
#define PACKDLL_API __declspec(dllexport)
#else
#define PACKDLL_API __declspec(dllimport)
#endif

// 此类是从 dll 导出的
class PACKDLL_API CPackDll {
public:
	CPackDll(void);
	// TODO: 在此处添加方法。
};

extern PACKDLL_API int nPackDll;
extern "C" PACKDLL_API BOOL Pack(CString strPath);

PACKDLL_API int fnPackDll(void);


// 用来保存壳（Stub）中用到的PE信息
typedef struct _GLOBAL_PARAM
{
	BOOL  bShowMessage; // 是否显示解密信息
	DWORD dwOEP;        // 程序入口点
	PBYTE lpStartVA;    // 起始虚拟地址（被异或加密区）
	PBYTE lpEndVA;      // 结束虚拟地址（被异或加密区）
}GLOBAL_PARAM, *PGLOBAL_PARAM;

