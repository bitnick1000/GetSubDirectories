
// MFCApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationDlg dialog



CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplicationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN32771, &CMFCApplicationDlg::OnFileOpen32771)
END_MESSAGE_MAP()


// CMFCApplicationDlg message handlers
void GetSubDirectories(CString& path, CList<CString> &listDirectories)
{
    CString findString = path + _T("\\*.*");
    CFileFind finder;
    BOOL isFind = finder.FindFile(findString);
     while (isFind)
    {
        isFind = finder.FindNextFileW();
        if(finder.IsDots())
            continue;
        else if(finder.IsDirectory())
        {
			listDirectories.AddTail(finder.GetFilePath());
			GetSubDirectories(finder.GetFilePath(),listDirectories);
        }
    }
}
void CMFCApplicationDlg::SelectFolder()
{
	CFolderPickerDialog picker;
	CString path;
	if(picker.DoModal()==IDOK)
	{
		path=picker.GetPathName();
	}
	else
	{
		CDialog::OnOK();
		return;
	}
    CList<CString> listDirectories;
	listDirectories.AddTail(path);
    //CString content;
	GetSubDirectories(path,listDirectories);
	CString clipboard=_T("");
	CString content=_T("");
	POSITION pos=listDirectories.GetHeadPosition();
	while(pos!=NULL)
	{
		CString directory=listDirectories.GetNext(pos);
		content+=directory;
		content+=_T("\r\n");
		clipboard+=directory;
		clipboard+=_T(";");
	}
	if(OpenClipboard())
	{
		EmptyClipboard();
		int len=(clipboard.GetLength()+1)*sizeof(TCHAR);
		HGLOBAL clipbuffer=GlobalAlloc(GMEM_DDESHARE,len);
		char* buffer=(char*)GlobalLock(clipbuffer);
		USES_CONVERSION;
		memcpy(buffer,T2A(clipboard),len);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
	SetDlgItemText(IDC_EDIT1, content);
	MessageBox(_T("Copied to clipboard!"));
}
BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SelectFolder();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplicationDlg::OnFileOpen32771()
{
	// TODO: Add your command handler code here
	SelectFolder();
}
