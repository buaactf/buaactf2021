
// PackBaseDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PackBase.h"
#include "PackBaseDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPackBaseDlg 对话框



CPackBaseDlg::CPackBaseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PACKBASE_DIALOG, pParent)
	, m_pathStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPackBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_pathEdit);
	DDX_Text(pDX, IDC_EDIT1, m_pathStr);
}

BEGIN_MESSAGE_MAP(CPackBaseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPackBaseDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPackBaseDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPackBaseDlg 消息处理程序

BOOL CPackBaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPackBaseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPackBaseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPackBaseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPackBaseDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
	//拖动单个文件的时候
	WCHAR wcStr[MAX_PATH];
	DragQueryFile(hDropInfo, 0, wcStr, MAX_PATH);//获得拖曳的文件的文件名  
	m_pathEdit.SetWindowText(wcStr);             //将路基添加到文本编辑框中  

	DragFinish(hDropInfo);                       //拖放结束后,释放内存  
	UpdateData(TRUE);                            //将界面上的信息保存到变量中

	MessageBox(m_pathStr, m_pathStr, 0);         //测试是否成功
}


void CPackBaseDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//设置要浏览文件的类型
	static TCHAR BASED_CODE szFilter[] = _T("可执行文件 (*.exe)|*.exe|")
		_T("DLL文件 (*.dll)|*.dll|")
		_T("All Files (*.*)|*.*||");

	CFileDialog fileDlg(TRUE, _T("exe"), _T(""),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);

	if (fileDlg.DoModal() == IDOK)
	{
		//将获取的路径信息赋给变量
		m_pathStr = fileDlg.GetPathName();
	}
	//更新控件显示信息
	UpdateData(FALSE);
}


#include "../Pack_Dll/Pack_Dll.h"
#ifdef _DEBUG
#pragma comment(lib, "./Debug/PACKDLL.lib")
#else
#pragma comment(lib, "../Release/PACKDLL.lib")
#endif



void CPackBaseDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(m_pathStr, m_pathStr, 0);
	//Pack(m_pathStr);
	UpdateData(true);
	//MessageBox(m_pathStr, m_pathStr, 0);

	if (!Pack(m_pathStr))
	{
		MessageBox(L"加密失败-_-!");
	}
	else
	{
		MessageBox(L"加密成功！");
	}
}
