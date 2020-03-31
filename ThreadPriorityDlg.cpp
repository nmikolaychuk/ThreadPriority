
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

DWORD WINAPI CThreadPriorityDlg::ThreadFunc(LPVOID pvParam)
{
	float arg = 100.0;
	while (true)
	{
		arg = sin(arg);
		arg = cos(arg);
		arg = tan(arg);
		arg = pow(arg, 0.5);
		arg = pow(arg, 0.6);

		InterlockedExchangeAdd(&theApp.hThreadValue[first], 1);
		InterlockedExchangeAdd(&theApp.hThreadValue[second], 1);
	}
	return 0;
}

CThreadPriorityDlg::CThreadPriorityDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADPRIORITY_DIALOG, pParent)
	, percent_of_first_thread_capacity(_T("0.0%"))
	, percent_of_second_thread_capacity(_T("0.0%"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadPriorityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_FIRST_THREAD_PRIORITY, s_first_thread_priority);
	DDX_Control(pDX, IDC_SLIDER_SECOND_THREAD_PRIORITY, s_second_thread_priority);
	DDX_Control(pDX, IDC_PROGRESS_FIRST_THREAD_CAPACITY, p_first_thread_capacity);
	DDX_Control(pDX, IDC_PROGRESS_SECOND_THREAD_CAPACITY, p_second_thread_capacity);
	DDX_Text(pDX, IDC_FIRST_THREAD_CAPACITY, percent_of_first_thread_capacity);
	DDX_Text(pDX, IDC_SECOND_THREAD_CAPACITY, percent_of_second_thread_capacity);
	DDX_Control(pDX, IDC_BUTTON_START_STOP, StartStopButton);
}

BEGIN_MESSAGE_MAP(CThreadPriorityDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CThreadPriorityDlg::OnBnClickedButtonStartStop)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CThreadPriorityDlg::OnBnClickedButtonExit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FIRST_THREAD_PRIORITY, &CThreadPriorityDlg::OnNMCustomdrawSliderFirstThreadPriority)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SECOND_THREAD_PRIORITY, &CThreadPriorityDlg::OnNMCustomdrawSliderSecondThreadPriority)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
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

	SetTimer(IDC_TIMER_CAPACITY, 200, NULL);

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
	UpdateData(TRUE);
	if (!bRunTh) {
		StartStopButton.SetWindowTextW(stop);

		for (int i = 0; i < ThreadNumber; i++)
		{
			theApp.hThreadValue.push_back(0);

			DWORD dwThreadID;

			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (LPVOID)i, 0, &dwThreadID);
			hThreadsCreated[i] = hThread;
		}
		bRunTh = true;
	}
	else {
		StartStopButton.SetWindowTextW(start);
		bRunTh = false;

		for (int i = 0; i < ThreadNumber; i++)
		{
			DWORD pdwExitCode = 0;
			::GetExitCodeThread(hThreadsCreated[i], &pdwExitCode);
			TerminateThread(hThreadsCreated[i], pdwExitCode);

			WaitForSingleObject(hThreadsCreated[i], INFINITE);
			CloseHandle(hThreadsCreated[i]);
		}
		
		for (int i = 0; i < ThreadNumber; i++)
		{
			hThreadsCreated[i] = 0;
			theApp.hThreadValue.clear();
		}
	}
	UpdateData(FALSE);
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
	*pResult = 3;
	UpdateData(TRUE);
	if (bRunTh)
	{
		// первый поток
		DWORD ThreadPriority = THREAD_PRIORITY_NORMAL;
		switch (s_second_thread_priority.GetPos())
		{
		case 1: ThreadPriority = THREAD_PRIORITY_LOWEST; break;
		case 2:	ThreadPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
		case 3:	ThreadPriority = THREAD_PRIORITY_NORMAL; break;
		case 4: ThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
		case 5:	ThreadPriority = THREAD_PRIORITY_HIGHEST; break;
		}
		SetThreadPriority(hThreadsCreated[0], ThreadPriority);
	}
	UpdateData(FALSE);
}


void CThreadPriorityDlg::OnNMCustomdrawSliderSecondThreadPriority(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 3;

	UpdateData(TRUE);
	if (bRunTh)
	{
		// второй поток
		DWORD ThreadPriority = THREAD_PRIORITY_NORMAL;
		switch (s_first_thread_priority.GetPos())
		{
		case 1: ThreadPriority = THREAD_PRIORITY_LOWEST; break;
		case 2:	ThreadPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
		case 3:	ThreadPriority = THREAD_PRIORITY_NORMAL; break;
		case 4: ThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
		case 5:	ThreadPriority = THREAD_PRIORITY_HIGHEST; break;
		}
		SetThreadPriority(hThreadsCreated[0], ThreadPriority);
	}
	UpdateData(FALSE);
}


void CThreadPriorityDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	UpdateData(TRUE);
	if (nIDEvent == IDC_TIMER_CAPACITY)
	{
		if (bRunTh)
		{
			double FirstThreadCapacity = InterlockedExchange(&theApp.hThreadValue[first], 0);
			double SecondThreadCapacity = InterlockedExchange(&theApp.hThreadValue[second], 0);

			FirstThreadCapacity = 100.0 * FirstThreadCapacity / (FirstThreadCapacity + SecondThreadCapacity);
			SecondThreadCapacity = 100.0 - FirstThreadCapacity;

			percent_of_first_thread_capacity.Format(_T("%0.1f"), FirstThreadCapacity);
			percent_of_second_thread_capacity.Format(_T("%0.1f"), SecondThreadCapacity);
			percent_of_first_thread_capacity.Insert(FirstThreadCapacity, L"%");
			percent_of_second_thread_capacity.Insert(SecondThreadCapacity, L"%");

			p_first_thread_capacity.SetPos(FirstThreadCapacity);
			p_second_thread_capacity.SetPos(SecondThreadCapacity);
			UpdateData(FALSE);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CThreadPriorityDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//UpdateData(TRUE);
	//if (bRunTh)
	//{
	//	// первый поток
	//	DWORD ThreadPriority = THREAD_PRIORITY_NORMAL;
	//	switch (s_second_thread_priority.GetPos())
	//	{
	//	case 1: ThreadPriority = THREAD_PRIORITY_LOWEST; break;
	//	case 2:	ThreadPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
	//	case 3:	ThreadPriority = THREAD_PRIORITY_NORMAL; break;
	//	case 4: ThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
	//	case 5:	ThreadPriority = THREAD_PRIORITY_HIGHEST; break;
	//	}
	//	SetThreadPriority(hThreadsCreated[0], ThreadPriority);

	//	// второй поток
	//	switch (s_first_thread_priority.GetPos())
	//	{
	//	case 1: ThreadPriority = THREAD_PRIORITY_LOWEST; break;
	//	case 2:	ThreadPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
	//	case 3:	ThreadPriority = THREAD_PRIORITY_NORMAL; break;
	//	case 4: ThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
	//	case 5:	ThreadPriority = THREAD_PRIORITY_HIGHEST; break;
	//	}
	//	SetThreadPriority(hThreadsCreated[0], ThreadPriority);
	//}
	//UpdateData(FALSE);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
