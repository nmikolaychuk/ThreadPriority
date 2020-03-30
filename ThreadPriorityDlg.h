
// ThreadPriorityDlg.h: файл заголовка
//

#pragma once


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
	BOOL bRunTh;
};
