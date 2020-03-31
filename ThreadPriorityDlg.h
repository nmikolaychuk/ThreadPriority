
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
	afx_msg void OnBnClickedButtonStartStop();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnNMCustomdrawSliderFirstThreadPriority(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderSecondThreadPriority(NMHDR* pNMHDR, LRESULT* pResult);
	
	CSliderCtrl s_first_thread_priority;
	CSliderCtrl s_second_thread_priority;
	CProgressCtrl p_first_thread_capacity;
	CProgressCtrl p_second_thread_capacity;
	CString percent_of_first_thread_capacity;
	CString percent_of_second_thread_capacity;

	static DWORD WINAPI ThreadFunc(LPVOID pvParam);

	static const int ThreadNumber = 2;		// число потоков
	
	HANDLE hThreadsCreated[ThreadNumber];		// созданные потоки
	//vector<unsigned int> hThreadValue;

	static const int first = 0;
	static const int second = 1;

	// смена имени кнопки Старт/Стоп
	CButton StartStopButton;
	BOOL bRunTh = false;

	CString start = L"Запуск";
	CString stop = L"Остановка";
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
