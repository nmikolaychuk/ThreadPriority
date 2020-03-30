
// ThreadPriorityDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ThreadPriority.h"
#include "ThreadPriorityDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CThreadPriorityDlg

DWORD WINAPI ThreadFunc1(PVOID pvParam)
{
	float arg = 1.0;
	while (arg > 0)
	{
		arg = cos(arg);
		arg = sin(arg);
		arg = asin(arg);

		if (theApp.)
	}
}

CThreadPriorityDlg::CThreadPriorityDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADPRIORITY_DIALOG, pParent)
	, percent_of_first_thread_capacity(_T("0.0%"))
	, percent_of_second_thread_capacity(_T("0.0%"))
	, bRunTh(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadPriorityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_FIRST_THREAD_PRIORITY, s_first_thread_priority);
	DDX_Control(pDX, IDC_SLIDER_SECOND_THREAD_PRIORITY, s_second_thread_priority);
	//  DDX_Control(pDX, IDC_BUTTON_START_STOP, button_start_stop);
	DDX_Control(pDX, IDC_PROGRESS_FIRST_THREAD_CAPACITY, p_first_thread_capacity);
	DDX_Control(pDX, IDC_PROGRESS_SECOND_THREAD_CAPACITY, p_second_thread_capacity);
	DDX_Text(pDX, IDC_FIRST_THREAD_CAPACITY, percent_of_first_thread_capacity);
	DDX_Text(pDX, IDC_SECOND_THREAD_CAPACITY, percent_of_second_thread_capacity);
	DDX_Control(pDX, IDC_BUTTON_START_STOP, bRunTh);
}

BEGIN_MESSAGE_MAP(CThreadPriorityDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CThreadPriorityDlg::OnBnClickedButtonStartStop)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CThreadPriorityDlg::OnBnClickedButtonExit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FIRST_THREAD_PRIORITY, &CThreadPriorityDlg::OnNMCustomdrawSliderFirstThreadPriority)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SECOND_THREAD_PRIORITY, &CThreadPriorityDlg::OnNMCustomdrawSliderSecondThreadPriority)
END_MESSAGE_MAP()


// Обработчики сообщений CThreadPriorityDlg

BOOL CThreadPriorityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	s_first_thread_priority.SetRange(1, 5, FALSE);
	s_first_thread_priority.SetPos(3);
	s_second_thread_priority.SetRange(1, 5, FALSE);
	s_second_thread_priority.SetPos(3);


	p_first_thread_capacity.SetRange(0, 100);
	p_first_thread_capacity.SetPos(0);
	p_second_thread_capacity.SetRange(0, 100);
	p_second_thread_capacity.SetPos(0);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CThreadPriorityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CThreadPriorityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CThreadPriorityDlg::OnBnClickedButtonStartStop()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!bRunTh)
	{
		
	}
}

void CThreadPriorityDlg::OnBnClickedButtonExit()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void CThreadPriorityDlg::OnNMCustomdrawSliderFirstThreadPriority(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CThreadPriorityDlg::OnNMCustomdrawSliderSecondThreadPriority(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}
