
// ThreadPriorityDlg.h: файл заголовка
//

#pragma once

#include <vector>

#define IDC_TIMER_CAPACITY 1

using namespace std;


// Диалоговое окно CThreadPriorityDlg
class CThreadPriorityDlg : public CDialogEx
{
// Создание
public:
	CThreadPriorityDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREADPRIORITY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	// функции/обработчики
	afx_msg void OnBnClickedButtonStartStop();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckFirstThreadSleep();
	afx_msg void OnBnClickedCheckSecondThreadSleep();
	
	CSliderCtrl s_first_thread_priority;				// переменные, связанные с элементами диалога
	CSliderCtrl s_second_thread_priority;
	CProgressCtrl p_first_thread_capacity;
	CProgressCtrl p_second_thread_capacity;
	CString percent_of_first_thread_capacity;
	CString percent_of_second_thread_capacity;

	static DWORD WINAPI ThreadFunc(LPVOID pvParam);		// функция потока

	static const int ThreadNumber = 2;		// число потоков
	
	HANDLE hThreadsCreated[ThreadNumber] = { 0, 0 };		// созданные потоки

	// смена имени кнопки Старт/Стоп
	CButton StartStopButton;		// переменная "управления" кнопкой <<Запуск>>
	BOOL bRunTh = false;		// переменная, показывающая, запущен ли процесс
	CString start = L"Запуск";		// строки с именем кнопки для каждого случая
	CString stop = L"Остановка";

	BOOL CheckSleep1;		// переменные, связанные с чекпоинтами Sleep
	BOOL CheckSleep2;
};
