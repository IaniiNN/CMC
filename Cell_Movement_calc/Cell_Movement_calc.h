
// Cell_Movement_calc.h: Hauptheaderdatei f�r die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei f�r PCH einschlie�en"
#endif

#include "resource.h"		// Hauptsymbole


// CCell_Movement_calcApp:
// Siehe Cell_Movement_calc.cpp f�r die Implementierung dieser Klasse
//

class CCell_Movement_calcApp : public CWinApp
{
public:
	CCell_Movement_calcApp();

// �berschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CCell_Movement_calcApp theApp;