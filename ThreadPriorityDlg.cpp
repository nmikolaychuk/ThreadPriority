
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

LPVOID param1 = "first";		// параметр первого потока
LPVOID param2 = "second";		// параметр второго потока

int iter1 = 0;		// счетчик "количества работы" первого потока
int iter2 = 0;		// счетчик "количества работы" второго потока

BOOL sleep1;		// переменная для управляемого прерывания первого потока
BOOL sleep2;		// переменная для управляемого прерывания второго потока

DWORD WINAPI CThreadPriorityDlg::ThreadFunc(LPVOID pvParam)
{
	while (true)
	{
		pow(rand(), 2.0);

		if (pvParam == param1)
		{
			iter1++;
			if (sleep1)
			{
				Sleep(0);
			}
		}

		if (pvParam == param2)
		{
			iter2++;
			if (sleep2)
			{
				Sleep(0);
			}
		}
	}
	return 0;
}

CThreadPriorityDlg::CThreadPriorityDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADPRIORITY_DIALOG, pParent)
	, percent_of_first_thread_capacity(_T("0.0%"))
	, percent_of_second_thread_capacity(_T("0.0%"))
	, CheckSleep1(FALSE)
	, CheckSleep2(FALSE)
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
	DDX_Check(pDX, IDC_CHECK_FIRST_THREAD_SLEEP, CheckSleep1);
	DDX_Check(pDX, IDC_CHECK_SECOND_THREAD_SLEEP, CheckSleep2);
}

BEGIN_MESSAGE_MAP(CThreadPriorityDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CThreadPriorityDlg::OnBnClickedButtonStartStop)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CThreadPriorityDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_CHECK_FIRST_THREAD_SLEEP, &CThreadPriorityDlg::OnBnClickedCheckFirstThreadSleep)
	ON_BN_CLICKED(IDC_CHECK_SECOND_THREAD_SLEEP, &CThreadPriorityDlg::OnBnClickedCheckSecondThreadSleep)
END_MESSAGE_MAP()


// Обработчики сообщений CThreadPriorityDlg

BOOL CThreadPriorityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	SetProcessAffinityMask(GetCurrentProcess(), 1); // Программе доступно 1 ядро процессора
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST); // Задание приоритета первичного потока

	// TODO: добавьте дополнительную инициализацию

	// задание значений, шага и диапазонов слайдеров
	s_first_thread_priority.SetRange(THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_HIGHEST, TRUE);
	s_first_thread_priority.SetTicFreq(1);
	s_first_thread_priority.SetPos(THREAD_PRIORITY_NORMAL);
	s_second_thread_priority.SetRange(THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_HIGHEST, TRUE);
	s_second_thread_priority.SetTicFreq(1);
	s_second_thread_priority.SetPos(THREAD_PRIORITY_NORMAL);

	// задание диапазонов Progress Bar
	p_first_thread_capacity.SetRange(0, 100);
	p_first_thread_capacity.SetPos(0);
	p_second_thread_capacity.SetRange(0, 100);
	p_second_thread_capacity.SetPos(0);

	// начальные значения при запуске программы
	iter1 = 0;
	iter2 = 0;
	sleep1 = false;
	sleep2 = false;

	SetTimer(IDC_TIMER_CAPACITY, 350, NULL);		// таймер подсчета выполненной работы вторичными процессами (описание ниже)

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
	if (!bRunTh) {
		UpdateData(TRUE);
		StartStopButton.SetWindowTextW(stop);		// меняем имя кнопки при запуске программы

		for (int i = 0; i < ThreadNumber; i++)
		{
			DWORD dwThreadID = NULL;
			HANDLE hThread = NULL;

			switch (i)		// создаем оба процесса
			{
			case 0: hThread = CreateThread(NULL, 0, ThreadFunc, param1, 0, &dwThreadID); break;
			case 1: hThread = CreateThread(NULL, 0, ThreadFunc, param2, 0, &dwThreadID); break;
			}
			hThreadsCreated[i] = hThread;		// запоминаем handle потоков (записываем в массив)

			SetThreadPriority(hThreadsCreated[i], s_first_thread_priority.GetPos());		// выставляем приорит процессов по умолчанию
			SetThreadPriority(hThreadsCreated[i], s_second_thread_priority.GetPos());
		}
		bRunTh = true;
		sleep1 = CheckSleep1;		// установка значений прерывания потоков в зависимости от состояния чекпоинтов
		sleep2 = CheckSleep2;
	}
	else {
		UpdateData(TRUE);
		StartStopButton.SetWindowTextW(start);		// меняем имя кнопки при останове программы
		bRunTh = false;

		for (int i = 0; i < ThreadNumber; i++)
		{
			// получение кода ошибка для закрытия потока
			DWORD pdwExitCode = 0;
			::GetExitCodeThread(hThreadsCreated[i], &pdwExitCode);
			TerminateThread(hThreadsCreated[i], pdwExitCode);

			WaitForSingleObject(hThreadsCreated[i], INFINITE);
			CloseHandle(hThreadsCreated[i]);
			hThreadsCreated[i] = 0; 
		}
		sleep1 = true;		// работа потоков прервана при останове программы
		sleep2 = true;
	}
	UpdateData(FALSE);
}

void CThreadPriorityDlg::OnBnClickedButtonExit()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}

void CThreadPriorityDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (nIDEvent == IDC_TIMER_CAPACITY)
	{
		UpdateData(TRUE);
		if (bRunTh)
		{
			float Sum = iter1 + iter2;

			float FirstThreadCapacity = iter1 / Sum * 100.0;
			float SecondThreadCapacity = iter2 / Sum * 100.0;

			int FTC = round(FirstThreadCapacity);	 // для корректного отображения Progress Bar округляем до целого
			int STC = round(SecondThreadCapacity);

			percent_of_first_thread_capacity.Format(_T("%0.1f%%"), FirstThreadCapacity);
			percent_of_second_thread_capacity.Format(_T("%0.1f%%"), SecondThreadCapacity);

			p_first_thread_capacity.SetPos(FTC);
			p_second_thread_capacity.SetPos(STC);
		}
		UpdateData(FALSE);

		iter1 = 0;
		iter2 = 0;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CThreadPriorityDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	UpdateData(TRUE);
	SetThreadPriority(hThreadsCreated[0], s_first_thread_priority.GetPos());		// выставляем приоритета первого потока в зависимости от положения слайдера
	SetThreadPriority(hThreadsCreated[1], s_second_thread_priority.GetPos());		// выставляем приоритета второго потока в зависимости от положения слайдера

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CThreadPriorityDlg::OnBnClickedCheckFirstThreadSleep()		// реализация чекпоинтов прерывания потока
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	if (!sleep1) sleep1 = true; 
	else sleep1 = false;
	UpdateData(FALSE);
}


void CThreadPriorityDlg::OnBnClickedCheckSecondThreadSleep()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	if (!sleep2) sleep2 = true; 
	else sleep2 = false;
	UpdateData(FALSE);
}
