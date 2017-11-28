
// Cell_Movement_calc.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CCell_Movement_calcApp:
// Siehe Cell_Movement_calc.cpp für die Implementierung dieser Klasse
//

class CCell_Movement_calcApp : public CWinApp
{
public:
	CCell_Movement_calcApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CCell_Movement_calcApp theApp;