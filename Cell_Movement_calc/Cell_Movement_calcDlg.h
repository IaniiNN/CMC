//*************************************************************
//Example Dialog for migration analysis 
//Input data: Simi Biocell Data File
//C. Hennig
//***************************************************************
// Cell_Movement_calcDlg.h: Headerdatei
//

#pragma once
#include "afxwin.h"

#define NKERNT 3000 		
#define NTIME 3000 
#define  PI 3.1415926

typedef struct {
    unsigned int time[NKERNT];		//array of time
	unsigned int xvalue[NKERNT];	//array of xvalues	
	unsigned int yvalue[NKERNT];	//array of yvalues	
	unsigned int level[NKERNT];		//array of level
	unsigned int index[NKERNT];		//array of indices
	int size[NKERNT];				
	int shape[NKERNT];
	int color[NKERNT];
	int shapecell;
	unsigned int colorcell;
	BOOL FaultFlag;
	BOOL Ref;
	unsigned int birthframe;
	unsigned int nrofcoord;
	unsigned char gen;
	unsigned char genpos;
    char name[256];
} cells;



typedef struct {
    unsigned long time[NKERNT];
	unsigned int xvalue[NKERNT];
	unsigned int yvalue[NKERNT];
	unsigned int level[NKERNT];
	unsigned int index[NKERNT];
	unsigned int nrofcoord;
	byte anz[NKERNT];
    char name[25];
} cellsmirror;


typedef struct {
     unsigned long time;
	 unsigned int xvalue;
	 unsigned int yvalue;
	 unsigned int level;
} m_cell_structure;



typedef struct {
        float x;
		float y;
        float z;
}vektor;


typedef struct {
	unsigned long timeframe;
    unsigned int rate;
} timehistogram;




// CCell_Movement_calcDlg-Dialogfeld
class CCell_Movement_calcDlg : public CDialogEx
{
// Konstruktion
public:
	CCell_Movement_calcDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_CELL_MOVEMENT_CALC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnLoad();
	afx_msg double leastsqrvek(vektor,vektor, double);
	afx_msg double leastsqrvek2D(vektor,vektor, double);
	afx_msg void OnBnClickedCenterref();
	afx_msg void OnBnClickedCalcmigration();
	afx_msg void OnBnClicked2d();
	afx_msg void OnBnClicked3d();
	afx_msg void OnBnClickedFindFrames();
	int dualexp(int argum);	

	CListBox m_LB_data;
	cells * cellblock;
	m_cell_structure * cell_short;
	timehistogram *m_frames;
	CEdit m_zscal;
	CButton m_Buttonfillin;
	CString m_strResults, m_strResults2, m_strResults3;
	CStatic m_path;
	unsigned int indexk, maxcells;
	BOOL newfile;
	HANDLE hmig;
	cellsmirror * cellblocksave;
	CEdit m_time_start;
	CEdit m_time_end;
	CEdit m_charref2;
	CEdit m_norm1;
	CEdit m_mod1;
	CButton m_DimCheckButtons[2];		
	BOOL m_2dcheck,m_3dcheck;
	unsigned long framearray[100];


};
