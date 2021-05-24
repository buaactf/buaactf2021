#include "pch.h"
//#include "stdafx.h"
#include "CProcessingPE.h"



CProcessingPE::CProcessingPE(void)
{
	ZeroMemory(&m_stcPeInfo, sizeof(PE_INFO));
}
CProcessingPE::~CProcessingPE(void)
{
}


/************************************************************************/
/* ��������:  RVAToOffset
/* ����ȫ�ƣ� CProcessingPE::RVAToOffset
/* ������	  ULONG uRvaAddr		RVA��ֵַ
/* ����ֵ��	  DWORD					�ɹ�����Offset��ʧ���򷵻�0
/* ˵����     ��������ַ(RVA)ת�ļ�ƫ��(Offset)
/************************************************************************/
DWORD CProcessingPE::RVAToOffset(ULONG uRvaAddr)
{
	//��ȡ����ͷ�� 
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNt_Header);

	//��ȡ���ε�����  --- nt���е��ļ�ͷ��  
	DWORD dwSize = m_pNt_Header->FileHeader.NumberOfSections;

	for (DWORD i = 0; i < dwSize; i++)
	{
		if ((pSectionHeader[i].VirtualAddress <= uRvaAddr) &&
			((pSectionHeader[i].VirtualAddress + pSectionHeader[i].Misc.VirtualSize) > uRvaAddr))
		{
			return (uRvaAddr - pSectionHeader[i].VirtualAddress + pSectionHeader[i].PointerToRawData);
		}
	}
	return 0;
}

/************************************************************************/
/* ��������:  OffsetToRVA
/* ����ȫ�ƣ� CProcessingPE::OffsetToRVA
/* ������	  ULONG uOffsetAddr		Offset��ֵַ
/* ����ֵ��	  DWORD					�ɹ�����RVA��ַ��ʧ���򷵻�0
/* ˵����     �ļ�ƫ��(Offset)ת��������ַ(RVA)
/************************************************************************/
DWORD CProcessingPE::OffsetToRVA(ULONG uOffsetAddr)
{
	//��ȡ����ͷ�� 
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNt_Header);

	//��ȡ���ε�����  --- nt���е��ļ�ͷ��  
	DWORD dwSize = m_pNt_Header->FileHeader.NumberOfSections;

	for (DWORD i = 0; i < dwSize; i++)
	{
		if ((pSectionHeader[i].PointerToRawData <= uOffsetAddr) &&
			(pSectionHeader[i].PointerToRawData + pSectionHeader[i].SizeOfRawData > uOffsetAddr))
		{
			return (uOffsetAddr - pSectionHeader[i].PointerToRawData + pSectionHeader[i].VirtualAddress);
		}
	}
	return 0;
}



/************************************************************************/
/* ��������:  GetPeInfo
/* ����ȫ�ƣ� CProcessingPE::GetPeInfo
/* ������	  LPVOID lpImageData	Ŀ���ļ����ڻ�������ָ��
/* ������	  DWORD dwImageSize		Ŀ���ļ��Ĵ�С
/* ������	  PPE_INFO pPeInfo		[OUT]���ڴ���Ŀ���ļ��Ĺؼ�PE��Ϣ
/* ����ֵ��	  BOOL					�ɹ�����true��ʧ���򷵻�false
/* ˵����     ��ȡPE�ļ���Ϣ
/************************************************************************/
BOOL CProcessingPE::GetPeInfo(LPVOID lpImageData, DWORD dwImageSize, PPE_INFO pPeInfo)
{
	// 1���ж�ӳ��ָ���Ƿ���Ч
	if (m_stcPeInfo.dwOEP)
	{
		CopyMemory(pPeInfo, &m_stcPeInfo, sizeof(PE_INFO));
		return true;
	}

	if (!lpImageData)
	{
		return false;
	}

	// 2����Ҫ��ȡPE���ļ��ĵ�ַ�ʹ�С��������
	m_dwFileDataAddr = (DWORD)lpImageData;
	m_dwFileDataSize = dwImageSize;


	// 3. ��ȡ������Ϣ
	// 3.1 ��ȡDOSͷ��NTͷ
	m_pDos_Header = (PIMAGE_DOS_HEADER)lpImageData;

	// ���PE�ļ��Ƿ���Ч
	if (m_pDos_Header->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}

	m_pNt_Header = (PIMAGE_NT_HEADERS)((DWORD)lpImageData + m_pDos_Header->e_lfanew);

	//  ���PE�ļ��Ƿ���Ч
	if (m_pNt_Header->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}

	// 3.2 ��ȡOEP
	m_stcPeInfo.dwOEP = m_pNt_Header->OptionalHeader.AddressOfEntryPoint;
	// 3.3 ��ȡӳ���ַ
	m_stcPeInfo.dwImageBase = m_pNt_Header->OptionalHeader.ImageBase;
	// 3.4 ��ȡ�ؼ�����Ŀ¼�������
	PIMAGE_DATA_DIRECTORY lpDataDir = m_pNt_Header->OptionalHeader.DataDirectory;
	m_stcPeInfo.pDataDir = lpDataDir;
	CopyMemory(&m_stcPeInfo.stcExport, lpDataDir + IMAGE_DIRECTORY_ENTRY_EXPORT, sizeof(IMAGE_DATA_DIRECTORY));

	// 3.5 ��ȡ���α���������ϸ��Ϣ
	m_stcPeInfo.pSectionHeader = IMAGE_FIRST_SECTION(m_pNt_Header);

	// 4. ����������
	CopyMemory(pPeInfo, &m_stcPeInfo, sizeof(PE_INFO));

	return true;
}


/************************************************************************/
/* ��������:  FixReloc
/* ����ȫ�ƣ� CProcessingPE::FixReloc
/* ������	  DWORD dwLoadImageAddr		��ӳ�񱻼��غ��Ԥ��ģ���ַ
/* ����ֵ��	  void
/* ˵����     �޸��ض�λ�� �˺���������RVAToOffset������
/* ע�⣺
/*		1. dwLoadImageAddrָ�Ĳ������䱾��ImageBase��ֵ�������䱻���غ��Ԥ
/*		��ģ���ַ��
/*		2. ���ض�λ������δ���ǵ��޸��������⣬���Ҫ��߼����ԣ�Ӧ�÷ֱ��
/*		�����ض�λ���ͽ�������Դ���
/************************************************************************/
void CProcessingPE::FixReloc(DWORD dwLoadImageAddr)
{
	// 1. ��ȡӳ���ַ������ָ��
	DWORD             dwImageBase;
	PVOID             lpCode;
	dwImageBase = m_pNt_Header->OptionalHeader.ImageBase;
	lpCode = (PVOID)(m_dwFileDataAddr + RVAToOffset(m_pNt_Header->OptionalHeader.BaseOfCode));

	// 2. ��ȡ�ض�λ�����ڴ��еĵ�ַ
	PIMAGE_DATA_DIRECTORY  pDataDir;
	PIMAGE_BASE_RELOCATION pReloc;
	pDataDir = &(m_pNt_Header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	pReloc = (PIMAGE_BASE_RELOCATION)(m_dwFileDataAddr + RVAToOffset(pDataDir->VirtualAddress));

	// 3. �����ض�λ������Ŀ���������ض�λ
	while (pReloc->SizeOfBlock)
	{
		// 3.1 ȡ���ض�λ��TypeOffset��������
		PWORD  pTypeOffset = (PWORD)((DWORD)pReloc + sizeof(IMAGE_BASE_RELOCATION));
		DWORD  dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

		// 3.2 ѭ������ض�λ��
		for (DWORD i = 0; i < dwCount; i++)
		{
			if (!*pTypeOffset) continue;
			// 3.2.1 ��ȡ���ض�λ��ָ���ָ��
			DWORD  dwPointToRVA = (*pTypeOffset & 0x0FFF) + pReloc->VirtualAddress;
			PDWORD pPtr = (PDWORD)(m_dwFileDataAddr + RVAToOffset(dwPointToRVA));
			// 3.2.2 �����ض�λ����ֵ
			DWORD dwIncrement = dwLoadImageAddr - dwImageBase;
			// 3.2.3 �޸����ض�λ�ĵ�ַ����
			*((PDWORD)pPtr) += dwIncrement;
			pTypeOffset++;
		}

		// 3.3 ָ����һ���ض�λ�飬��ʼ��һ��ѭ��
		pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pReloc + pReloc->SizeOfBlock);
	}
}


/************************************************************************/
/* ��������:  GetExpVarAddr
/* ����ȫ�ƣ� CProcessingPE::GetExpVarAddr
/* ������	  LPCTSTR strVarName
/* ����ֵ��	  PVOID
/* ˵����     ��ȡ����ȫ�ֱ������ļ�ƫ��
/************************************************************************/
PVOID CProcessingPE::GetExpVarAddr(LPCTSTR strVarName)
{
	// 1����ȡ�������ַ����������strVarNameתΪASCII��ʽ������ԱȲ���
	CHAR szVarName[MAX_PATH] = { 0 };
	PIMAGE_EXPORT_DIRECTORY lpExport = (PIMAGE_EXPORT_DIRECTORY)(m_dwFileDataAddr +
		RVAToOffset(m_stcPeInfo.stcExport.VirtualAddress));
	WideCharToMultiByte(CP_ACP, NULL, strVarName, -1, szVarName, _countof(szVarName), NULL, FALSE);

	// 2��ѭ����ȡ�����������������������������szVarName���ȶԣ������ͬ����ȡ�����Ӧ�ĺ�����ַ
	for (DWORD i = 0; i < lpExport->NumberOfNames; i++)
	{
		PDWORD pNameAddr = (PDWORD)(m_dwFileDataAddr + RVAToOffset((DWORD)lpExport->AddressOfNames + 4 * i));
		PCHAR strTempName = (PCHAR)(m_dwFileDataAddr + RVAToOffset(*pNameAddr));
		if (!strcmp(szVarName, strTempName))
		{
			PDWORD pFunAddr = (PDWORD)(m_dwFileDataAddr + RVAToOffset((DWORD)lpExport->AddressOfFunctions + 4 * i));
			return (PVOID)(m_dwFileDataAddr + RVAToOffset(*pFunAddr));
		}
	}
	return 0;
}



/************************************************************************/
/* ��������:  AddSection
/* ����ȫ�ƣ� CProcessingPE::AddSection
/* ������	  LPCTSTR strName						�����ε�����
/* ������	  DWORD dwSize							�����ε���С���
/* ������	  DWORD dwChara							�����ε�����
/* ������	  PIMAGE_SECTION_HEADER pNewSection		[OUT]�����εĶνṹָ��
/* ������	  PDWORD lpSize							[OUT]�����ε����մ�С
/* ����ֵ��	  PVOID									�ɹ�����ָ�����������������ڴ��ָ��
/* ˵����     ������κ���
/* ע��
/*		�˺�����δ���ǵ�Ŀ�꺯�����ڸ������ݵ�ϸ�����⡣
/************************************************************************/
PVOID CProcessingPE::AddSection(LPCTSTR strName, DWORD dwSize, DWORD dwChara, PIMAGE_SECTION_HEADER pNewSection, PDWORD lpSize)
{
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNt_Header);

	// 1. ��ȡ������Ϣ
	DWORD dwDosSize = m_pDos_Header->e_lfanew;
	DWORD dwPeSize = sizeof(IMAGE_NT_HEADERS32);
	DWORD dwStnSize = m_pNt_Header->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
	DWORD dwHeadSize = dwDosSize + dwPeSize + dwStnSize;

	// 2. �����α��м��������ε���Ϣ
	// 2.1 ��ȡ������Ϣ
	CHAR  szVarName[7] = { 0 };
	DWORD dwFileAlign = m_pNt_Header->OptionalHeader.FileAlignment;    // �ļ�����
	DWORD dwSectAlign = m_pNt_Header->OptionalHeader.SectionAlignment; // ��������   
	WORD  dwNumOfsect = m_pNt_Header->FileHeader.NumberOfSections;     // ������Ŀ

	// 2.2 ��ȡ���һ�����ε���Ϣ
	IMAGE_SECTION_HEADER stcLastSect = { 0 };
	CopyMemory(&stcLastSect, &pSectionHeader[dwNumOfsect - 1], sizeof(IMAGE_SECTION_HEADER));

	// 2.3 �����������ȼ�����Ӧ��ַ��Ϣ
	DWORD dwVStart = 0;                                                        // �����ַ��ʼλ��
	DWORD dwFStart = stcLastSect.SizeOfRawData + stcLastSect.PointerToRawData; // �ļ���ַ��ʼλ��

	if (stcLastSect.Misc.VirtualSize%dwSectAlign)
		dwVStart = (stcLastSect.Misc.VirtualSize / dwSectAlign + 1) * dwSectAlign + stcLastSect.VirtualAddress;
	else
		dwVStart = (stcLastSect.Misc.VirtualSize / dwSectAlign) * dwSectAlign + stcLastSect.VirtualAddress;

	DWORD dwVirtualSize = 0; // ���������С
	DWORD dwSizeOfRawData = 0; // �����ļ���С



	if (dwSize%dwSectAlign)
		dwVirtualSize = (dwSize / dwSectAlign + 1) * dwSectAlign;
	else
		dwVirtualSize = (dwSize / dwSectAlign) * dwSectAlign;

	if (dwSize%dwFileAlign)
		dwSizeOfRawData = (dwSize / dwFileAlign + 1) * dwFileAlign;
	else
		dwSizeOfRawData = (dwSize / dwFileAlign) * dwFileAlign;

	WideCharToMultiByte(CP_ACP, NULL, strName, -1, szVarName, _countof(szVarName), NULL, FALSE);

	// 2.4 ��װһ���µ�����ͷ
	IMAGE_SECTION_HEADER stcNewSect = { 0 };
	CopyMemory(stcNewSect.Name, szVarName, 7);     // ��������
	stcNewSect.Misc.VirtualSize = dwVirtualSize;   // �����С
	stcNewSect.VirtualAddress = dwVStart;         // �����ַ
	stcNewSect.SizeOfRawData = dwSizeOfRawData;   // �ļ���С
	stcNewSect.PointerToRawData = dwFStart;        // �ļ���ַ
	stcNewSect.Characteristics = dwChara;         // ��������

	// 2.5 д��ָ��λ��
	CopyMemory((PVOID)(m_dwFileDataAddr + dwHeadSize), &stcNewSect, sizeof(IMAGE_SECTION_HEADER));

	// 3. �޸�������Ŀ�ֶ�NumberOfSections
	m_pNt_Header->FileHeader.NumberOfSections++;

	// 4. �޸�PE�ļ��ľ���ߴ��ֶ�SizeOfImage
	m_pNt_Header->OptionalHeader.SizeOfImage += dwVirtualSize;

	// 5. ���������ε���ϸ��Ϣ����С���Լ���ֱ�ӷ��ʵĵ�ַ
	CopyMemory(pNewSection, &stcNewSect, sizeof(IMAGE_SECTION_HEADER));
	*lpSize = dwSizeOfRawData;
	return (PVOID)(m_dwFileDataAddr + dwFStart);
}


/************************************************************************/
/* ��������:  SetOEP
/* ����ȫ�ƣ� CProcessingPE::SetOEP
/* ������	  DWORD dwOEP		��OEP
/* ����ֵ��	  void
/* ˵����     �޸�Ŀ���ļ�OEP
/************************************************************************/
void CProcessingPE::SetOEP(DWORD dwOEP)
{
	m_pNt_Header->OptionalHeader.AddressOfEntryPoint = dwOEP;
}

/************************************************************************/
/* ��������:  SetDLL
/* ����ȫ�ƣ� CProcessingPE::SetDLL
/* ����ֵ��	  void
/* ˵����     ȥ��DLL��̬���ر�ʶ
/************************************************************************/
void CProcessingPE::SetDLL()
{
	m_pNt_Header->OptionalHeader.DllCharacteristics = 0x0;
}