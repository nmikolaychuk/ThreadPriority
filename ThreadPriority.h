
// ThreadPriority.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include <vector>


// CThreadPriorityApp:
// Сведения о реализации этого класса: ThreadPriority.cpp
//

class CThreadPriorityApp : public CWinApp
{
public:
	CThreadPriorityApp();

	std::vector <unsigned int> hThreadValue;
// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CThreadPriorityApp theApp;
