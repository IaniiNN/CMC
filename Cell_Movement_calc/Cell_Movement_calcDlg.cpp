//*************************************************************
//Example Dialog for migration analysis 
//Input data: Simi Biocell Data File
//C. Hennig
//***************************************************************
// Cell_Movement_calcDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Cell_Movement_calc.h"
#include "Cell_Movement_calcDlg.h"
#include "afxdialogex.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCell_Movement_calcDlg-Dialogfeld




CCell_Movement_calcDlg::CCell_Movement_calcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCell_Movement_calcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCell_Movement_calcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_LB_data);
	DDX_Control(pDX, IDC_ZSCAL, m_zscal);
	DDX_Control(pDX, IDC_FILLIN, m_Buttonfillin);
	DDX_Control(pDX, IDC_PATH, m_path);
	DDX_Control(pDX, IDC_TIMESTART, m_time_start);
	DDX_Control(pDX, IDC_TIMEEND, m_time_end);
	DDX_Control(pDX, IDC_CHARREF2, m_charref2);
	DDX_Control(pDX, IDC_NORM1, m_norm1);
	DDX_Control(pDX, IDC_MOD, m_mod1);
	DDX_Control(pDX, IDC_3D, m_DimCheckButtons[0]);
	DDX_Control(pDX, IDC_1D, m_DimCheckButtons[1]);
	DDX_Check(pDX,  IDC_3D, m_3dcheck);
	DDX_Check(pDX,  IDC_1D, m_2dcheck);
}

BEGIN_MESSAGE_MAP(CCell_Movement_calcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BN_LOAD, &CCell_Movement_calcDlg::OnBnClickedBnLoad)
	ON_BN_CLICKED(IDC_CENTERREF, &CCell_Movement_calcDlg::OnBnClickedCenterref)
	ON_BN_CLICKED(IDC_CALCMIGRATION, &CCell_Movement_calcDlg::OnBnClickedCalcmigration)
	ON_BN_CLICKED(IDC_2D, &CCell_Movement_calcDlg::OnBnClicked2d)
	ON_BN_CLICKED(IDC_3D, &CCell_Movement_calcDlg::OnBnClicked3d)
	ON_BN_CLICKED(IDC_BTN_FINDFRAMES, &CCell_Movement_calcDlg::OnBnClickedFindFrames)

END_MESSAGE_MAP()


// CCell_Movement_calcDlg-Meldungshandler

BOOL CCell_Movement_calcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	
	OnBnClicked2d();
	m_norm1.SetWindowText("500");	
	m_mod1.SetWindowText("3");
	m_charref2.SetWindowText("MSa");
	return TRUE;  
}


void CCell_Movement_calcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);


		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CCell_Movement_calcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCell_Movement_calcDlg::OnBnClickedBnLoad()
{
float difftime,diffx,diffy,diffz,incremx,incremy,incremz;
int zae,ji,newanz,randanz;
char filename[150],fileext[10], str[300],str1[300],str3[300],str4[300],namesbc[150],namesbd[300],str4f[300];
char  comment[1000];
char * str2, *str5, *str5a, *stra, *stranz;
int i,anzue,anzz,gesanzz,j,k,len,len2,len3,len4,len5,len6,len7,ij,platzzae, result,var;
int nuflag;
int blocknrrandom[20000],rindex,ik;
bool ReadFile=false;
int anzahlcellmigAB;

char buffer[500];
char *pdest;
DWORD dwBytes;
	char szFile[256];

BOOL FILLUP;
FILLUP=FALSE;


	if (m_Buttonfillin.GetCheck()==TRUE)
	{
		FILLUP=TRUE;
	}

{
  CFileDialog ldFile(TRUE, ".sbd", NULL, OFN_READONLY , "sbd|*.sbd|Alle Dateien|*.*||", NULL);
    if (ldFile.DoModal() == IDOK)
	{
		m_strResults = ldFile.GetFileTitle();
		m_strResults2 = ldFile.GetPathName();
		m_strResults3 = ldFile.GetFileExt();
		sprintf_s(namesbd,"%s", m_strResults2);
		sprintf_s(filename,"%s", m_strResults);
		sprintf_s(fileext,"%s", m_strResults3);
		ReadFile=true;
		gesanzz=0;
     UpdateData(FALSE);
	}


}

if (ReadFile==true && strcmp(fileext, "sbc")==0)
{
int length=strlen(namesbd);
namesbd[length-1]='d';
strcpy_s(fileext,"sbd");
}

if (ReadFile==true && strcmp(fileext, "sbd")==0)
{
int celldeathnr=0;

UpdateData(FALSE);						
m_path.SetWindowText(namesbd);
strcpy_s(namesbc,namesbd);
namesbc[strlen(namesbd)-1]='c';

m_LB_data.ResetContent();

if (newfile==true)					//Delete Structure if new File
{
	free (cellblock);
}



cellblock=(cells*)malloc(NTIME*sizeof(cells));			// preserve some memory

CStdioFile dateisbc(namesbc,CFile::modeRead);
dateisbc.ReadString(str,sizeof(str));

// Load some Info from Simi Description File *sbc
while ( !(str[0]=='[' && str[1]=='3' && str[2]=='D') ) 
{
dateisbc.ReadString(str,sizeof(str));
if (str[0]=='S' && str[1]=='C' && str[2]=='A' && str[3]=='N' && str[4]=='T')		//SCANTIME
	{
	stra=strpbrk(str,"=");
	stra++;
		len=strlen(str);
		len=stra-str;
		int scantime=atoi(stra);
		sprintf_s(str4,"%d" ,scantime/10);						//get timebase
	GetDlgItem(IDC_TIMEBASE)->SetWindowText(str4);	
	}

else if (str[0]=='L' && str[1]=='E' && str[2]=='V' && str[3]=='E' && str[5]=='C')		//LEVELCOUNT use if neccessary
	{
		stra=strpbrk(str,"=");
	stra++;
		len=strlen(str);
		len=stra-str;
		int layer=atoi(stra);
	}

else if (str[0]=='C' && str[1]=='O' && str[2]=='M' && str[3]=='M' && str[4]=='E')		//COMMENT use if neccessary
	{
		strcpy_s(comment,"");
		while ( !(str[0]=='[' && str[1]=='F' && str[2]=='A') ) 
				{
				strcat_s(comment, str);
				dateisbc.ReadString(str,sizeof(str));
				
				}
	}
else if (str[0]=='W' && str[1]=='I' && str[2]=='D' && str[3]=='T' && str[4]=='H')		//WIDTH use if neccessary
	{
		stra=strpbrk(str,"=");
	stra++;
		len=strlen(str);
		len=stra-str;
		int width=atoi(stra);
	}
else if (str[0]=='H' && str[1]=='E' && str[2]=='I' && str[3]=='G' && str[4]=='H')		//HEIGHT use if neccessary
	{
		stra=strpbrk(str,"=");
		stra++;
		len=strlen(str);
		len=stra-str;
		int height=atoi(stra);
	}
else if (str[0]=='L' && str[1]=='E' && str[2]=='V' && str[3]=='E' && str[5]=='S')		//LEVELSPACE zscal important (3D)
	{
		stra=strpbrk(str,"=");
	stra++;
		len=strlen(str);
		len=stra-str;
		int levelspace=atoi(stra);
		sprintf_s(str4,"%d" ,levelspace);						
	//	m_zscal.SetWindowText(str4);	//automatically set via data file

	}

}



CStdioFile datei(namesbd,CFile::modeRead);

m_LB_data.ResetContent();

datei.ReadString(str,sizeof(str));
datei.ReadString(str,sizeof(str));

if (str[0]=='2') anzue=5;
else if (str[0]=='3') anzue=7;	  // Version 3	
else if (str[0]=='4') anzue=5;   // Version 4 !
else 
{
m_LB_data.ResetContent();
m_LB_data.AddString("unknown File");
}

for (i=0;i<anzue;i++)	//skip some lines
	{
	datei.ReadString(str,sizeof(str));
	}
 

anzahlcellmigAB=0;
indexk=0;
nuflag=0;
maxcells =0;			

for (k=0;k<12000;k++)  
	{//1a
			for (i=0;i<5;i++)   // nr of lines per block
				{//2a

				if	( (datei.ReadString(str,sizeof(str)))==NULL)	
					{//3a
					maxcells=k;
					nuflag=1;
					break;
					}//3a
				
					else if (i<4) 
					{//3b
					nuflag=0;   
					exit;
					}//3b
				//*************************	//Read Name of cell
				if (i==1)										
					{//3c
					str5=(char *)malloc(30*sizeof(char));
					
					char *result = NULL;
					char *result1 = NULL;
					if (result= strpbrk( str, "ABCDEMPVXSIabcdempvxsi" ))
						{
						result--;
						int zeichenleer = strncmp( result, " " , 1 );
						if (zeichenleer==0)
							{//4a
							result++;
							}//4a

						else if (zeichenleer!=0)
							{//4b
							if (!(result1= strpbrk(result, "0123456789" )))
								{
										result++;
								}
							else
								{
								result= strpbrk(result, "0123456789");
								}
														
							}//4b
						if (result!=NULL) 
							{
							str5a=strpbrk(result,"\n");
							if (str5a==NULL)		
								{
								strncpy_s(cellblock[k].name,result,40);
								len=strlen(result);
								cellblock[k].name[len]='\0';
								indexk++;
								}
							}
						if (result!=NULL && str5a!=NULL)
							{//4c
							len=strlen(result);
							len=str5a-result;
							strncpy_s(cellblock[k].name,result,40);
							if (result[len]=='\n' && result[len-1]==0xD) cellblock[k].name[len-1]='\0';
							else cellblock[k].name[len]='\0';
							indexk++;

							len=strlen(cellblock[k].name);
							if (cellblock[k].name[0] == 'A') cellblock[k].gen = len-1;
							if (cellblock[k].name[0] == 'M') cellblock[k].gen = len+1;
							if (cellblock[k].name[0] == 'E' || cellblock[k].name[0] == 'C') cellblock[k].gen = len+3;
							if (cellblock[k].name[0] == 'D') cellblock[k].gen = len+4;
							if (cellblock[k].name[0] == 'P' && cellblock[k].name[1] == '1') cellblock[k].gen = 1;
							if (cellblock[k].name[0] == 'P' && cellblock[k].name[1] == '2') cellblock[k].gen = 2;
							if (cellblock[k].name[0] == 'P' && cellblock[k].name[1] == '3') cellblock[k].gen = 3;
							if (strcmp(cellblock[k].name,"ABalaaaala")==0)
								{
									int dummy=1;
							}
			
							}//4c
						cellblock[k].Ref=TRUE;
						}
					else if (!(result= strpbrk( str, "ABCDEMPVXSIabcdempvxsi" )))
						{
						strncpy_s(cellblock[k].name,"unknownCell",40);
						sprintf_s(str3,"Error at:  %s", str);
						m_LB_data.AddString(str3);
						indexk++;
						}
					}//3c
				//*********************************************End Name of cell
				if (i==2)	
					{
						len=strcspn(str," ");
						strncpy_s(str4,str,len);								
						str4[len]='\0';
						cellblock[k].birthframe=atoi(str4);
						len2=len;
						len=0;

						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len2+1);
						len3=strcspn(str," ");									
						strncpy_s(str1,str2,len3-len2);
						if (len3>len2) str1[len3-len2]='\0';
						var=atoi(str1);
						
						str2=(char *)malloc(4*sizeof(char));	//   FATE
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len3+1);
						len2=strcspn(str," ");									
						strncpy_s(str1,str2,len2-len3);
						if (len2>len3) str1[len2-len3]='\0';
						var=atoi(str1);
						
					}	
	
					if (i==3)	
					{
						len=strcspn(str," ");
						strncpy_s(str4,str,len);					//  BIRTHTIME
						str4[len]='\0';
						cellblock[k].birthframe=atoi(str4);
						len2=len;
						len=0;

						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len2+1);
						len3=strcspn(str," ");									
						strncpy_s(str1,str2,len3-len2);
						if (len3>len2) str1[len3-len2]='\0';
						var=atoi(str1);
						
						str2=(char *)malloc(4*sizeof(char));         //   FATE 
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len3+1);
						len2=strcspn(str," ");									
						strncpy_s(str1,str2,len2-len3);
						if (len2>len3) str1[len2-len3]='\0';
						var=atoi(str1);

						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len2+1);
						len3=strcspn(str," ");									
						strncpy_s(str1,str2,len3-len2);
						if (len3>len2) str1[len3-len2]='\0';
						var=atoi(str1);

						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len3+1);
						len2=strcspn(str," ");									
						strncpy_s(str1,str2,len2-len3);
						if (len2>len3) str1[len2-len3]='\0';
						cellblock[k].shapecell=atoi(str1);


						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len2+1);
						len3=strcspn(str," ");									
						strncpy_s(str1,str2,len3-len2);
						if (len2>len3) str1[len3-len2]='\0';
						cellblock[k].colorcell=atoi(str1);
	
					
					}


				if (i==4)											// NUMBER OF VALUES
					{//3d 
					var=0;
					anzz=atoi(str);
					gesanzz+=anzz;

					cellblock[k].nrofcoord=anzz;
					stranz=(char *)malloc(29);
					itoa(anzz,stranz,10);
					strcat(stranz,"\n");
					for (j=0;j<anzz;j++)							// READ COORDINATES
						{//4d
						
						datei.ReadString(str,sizeof(str));
						len=strcspn(str," ");
						strncpy_s(str4,str,len);									//   VALUE
						str4[len]='\0';
						var=atoi(str4);
						if ((var>=0)&& (var<10000)) cellblock[k].time[j]=atoi(str4);
						
				
						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len+1);
						len2=strcspn(str," ");									// X - VALUE
						strncpy_s(str1,str2,len2-len);
						str1[len2-len]='\0';
						var=atoi(str1);
						if ((var>=0)&& (var<10000)) cellblock[k].xvalue[j]=atoi(str1);

	
						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len2+1);								// Y - VALUE
						len3=strcspn(str," ");
						strncpy_s(str1,str2,len3-len2);
						str1[len3-len2]='\0';
						var=atoi(str1);
						if ((var>=0)&& (var<10000)) cellblock[k].yvalue[j]=atoi(str1);


						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len3+1);
						len4=strcspn(str," ");									// level ("Z")
						strncpy_s(str1,str2,len4-len3);
						if (len4>len3) str1[len4-len3]='\0';
						var=atoi(str1);
						if ((var>=0)&& (var<10000)) cellblock[k].level[j]=(atoi(str1));
						if (cellblock[k].name[0]=='A' && strlen(cellblock[k].name) <=10 ) anzahlcellmigAB++;
						
						
						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len4+1);
						len5=strcspn(str," ");									//SIZE
						strncpy_s(str1,str2,len5-len4);
						if (len5>len4) str1[len5-len4]='\0';
						var=atoi(str1);
						if ((var>=-1)&& (var<10000)) cellblock[k].size[j]=var;
						
						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len5+1);
						len6=strcspn(str," ");									//SHAPE
						strncpy_s(str1,str2,len6-len5);
						if (len6>len5) str1[len6-len5]='\0';
						var=atoi(str1);
						if ((var>=-1)&& (var<10000)) cellblock[k].shape[j]=var;
					
						str2=(char *)malloc(4*sizeof(char));
						str2=strpbrk(str," ");
						str2++;
						strnset(str,'x',len6+1);
						len7=strcspn(str," ");									//COLOR
						strncpy_s(str1,str2,len7-len6);
						if (len7>len6) str1[len7-len6]='\0';
						var=atoi(str1);
						cellblock[k].color[j]=var;
		
						}//4d

					//}//3d

					if (FILLUP==TRUE)															//interpolation active			
						{//4e		
					randanz=1;
					if (anzz>1)
						{//5b
					
						for (ik=0;ik<randanz;ik++)
						{//6a
							if (blocknrrandom[ik]==k)
								{//7a
								rindex=ik;
								}//7a
							newanz=(cellblock[k].time[anzz-1]-cellblock[k].time[0])+1;				//number of cellcoordinates rises
							cellblock[k].nrofcoord=newanz;
							cell_short=(m_cell_structure*)malloc(newanz*sizeof(m_cell_structure));				//memory for parallel structure
							platzzae=1;

							for (zae=0;zae<(anzz-1);zae++)

								{//7b
								difftime=cellblock[k].time[zae+1]-cellblock[k].time[zae];				//difftime between  1. and 2. value
								diffx=abs(int (cellblock[k].xvalue[zae+1]-cellblock[k].xvalue[zae]));			//delta x between  1. and 2. value
								incremx=(diffx/difftime);												//  gradient (delta x/ delta t)
								diffy=abs(int(cellblock[k].yvalue[zae+1]-cellblock[k].yvalue[zae]));			//delta y between  1. and 2. value
								incremy=(diffy/difftime);												//  gradient (delta y/ delta t)
								diffz=abs(int(cellblock[k].level[zae+1]-cellblock[k].level[zae]));			//delta z between  1. and 2. value
								incremz=(diffz/difftime);												//  gradient (delta z/ delta t)

								if (cellblock[k].xvalue[zae]>cellblock[k].xvalue[zae+1]) incremx=0.-incremx;		//change algebraic sign if  necessary
								if (cellblock[k].yvalue[zae]>cellblock[k].yvalue[zae+1]) incremy=0.-incremy;
								if (cellblock[k].level[zae]>cellblock[k].level[zae+1]) incremz=0.-incremz;

								for (ji=1;ji<(difftime+1);ji++)
									{//8b
									if (ji<difftime)
										{//9a
										cell_short[platzzae].time=cellblock[k].time[zae]+ji;						//new values in parallel structure
										cell_short[platzzae].xvalue=cellblock[k].xvalue[zae]+(int)(incremx*ji);
										cell_short[platzzae].yvalue=cellblock[k].yvalue[zae]+(int)(incremy*ji);
										cell_short[platzzae].level=cellblock[k].level[zae]+(int)(incremz*ji);
										platzzae++;
										}//9a
									else
										{//9b
										cell_short[platzzae].time=cellblock[k].time[zae]+ji;
										cell_short[platzzae].xvalue=cellblock[k].xvalue[zae+1];
										cell_short[platzzae].yvalue=cellblock[k].yvalue[zae+1];
										cell_short[platzzae].level=cellblock[k].level[zae+1];
										platzzae++;
										
										}//9b
									}//8b
								
								}//7b

							for (ij=1;ij<(newanz);ij++)								//Write Values back in structure
								{//7c
								cellblock[k].time[ij]=cell_short[ij].time;
								cellblock[k].xvalue[ij]=cell_short[ij].xvalue;
								cellblock[k].yvalue[ij]=cell_short[ij].yvalue;
								cellblock[k].level[ij]=cell_short[ij].level;
								cellblock[k].size[ij]=-1;
								cellblock[k].shape[ij]=-1;
								cellblock[k].color[ij]=-1;

								sprintf_s(str3,"%ld %d %d %d Cellname: %s",cellblock[k].time[ij],cellblock[k].xvalue[ij],cellblock[k].yvalue[ij],cellblock[k].level[ij],cellblock[k].name);
								m_LB_data.AddString(str3);
								
								}//7c
							free(cell_short);
							}//6a



						}//5b


					}//4e
				//}									
			}//3d
	}//2a

  if (maxcells > 0) break;

}//1a



if (maxcells > 0) 
	{//1
	datei.Close();
//	newfile=TRUE;
	exit;
	}//!1


cellblocksave = new cellsmirror[NTIME];
//random=FALSE;									





}
 
else 
{
m_LB_data.ResetContent();
m_LB_data.AddString("Unbekanntes File");
  UpdateData(FALSE);
}

}


void CCell_Movement_calcDlg::OnBnClickedCenterref()
{
	int k,l;
	int countermax=0;
	char str[150];
	char charref2[50];

int offsetx=200;
int offsety=200;
int offsetlevel=8;

m_charref2.GetWindowText(charref2,50);

m_time_end.GetWindowText(str,100);
int zeitp1=atoi(str);
m_time_start.GetWindowText(str,100);
int zeitp2=atoi(str);

m_LB_data.ResetContent();

	for (int v=0;v<2000;v++)						
			{
			cellblocksave[0].time[v]=0;
			cellblocksave[0].xvalue[v]=0;
			cellblocksave[0].yvalue[v]=0;
			cellblocksave[0].level[v]=0;
			cellblocksave[0].anz[v]=0;
			}

			int counter=0;
countermax=zeitp1-zeitp2+1;

for (int p=zeitp2; p<zeitp1+1; p++)
{
counter=0;
	for (k=0;k<(maxcells);k++)
	{
	//if (strncmp(cellblock[k].name,"ABal",4)==0)			//Schwerpunkt gesamt
	//if (strncmp(cellblock[k].name,"ABalp",5)==0)	//Schwerpunkt Shift_ref
	if (strncmp(cellblock[k].name,charref2,strlen(charref2))==0)	//Schwerpunkt Shift_ref
	{
		if (cellblock[k].nrofcoord>0)
		{
			for (l=0;l<cellblock[k].nrofcoord;l++)	
			{
				if (p==cellblock[k].time[l])
				{
				counter++;
				cellblocksave[0].time[p-zeitp2]=cellblock[k].time[l];
				cellblocksave[0].xvalue[p-zeitp2]+=cellblock[k].xvalue[l];
				cellblocksave[0].yvalue[p-zeitp2]+=cellblock[k].yvalue[l];
				cellblocksave[0].level[p-zeitp2]+=cellblock[k].level[l];
				}
			}
		}
	}
	}
	cellblocksave[0].anz[p-zeitp2]=counter;
	if (counter==0) counter =1; // Null abfangen
	cellblocksave[0].xvalue[p-zeitp2]=cellblocksave[0].xvalue[p-zeitp2]/counter;
	cellblocksave[0].yvalue[p-zeitp2]=cellblocksave[0].yvalue[p-zeitp2]/counter;
	cellblocksave[0].level[p-zeitp2]=cellblocksave[0].level[p-zeitp2]/counter;
}



for (k=0;k<(maxcells);k++)
{

	if (strcmp(cellblock[k].name,"Dppp")==0) 
	{
	cellblock[k].nrofcoord= countermax;
	for (l=0;l<cellblock[k].nrofcoord;l++)						
			{
			cellblock[k].time[l]=cellblocksave[0].time[l];
			cellblock[k].xvalue[l]=cellblocksave[0].xvalue[l];
			cellblock[k].yvalue[l]=cellblocksave[0].yvalue[l];
			cellblock[k].level[l]=cellblocksave[0].level[l];
			cellblock[k].shape[l]=2;
			cellblock[k].color[l]=255+256*255;
			cellblock[k].colorcell=255+256*255;
			sprintf_s(str,"%s %d %d %d %d %d\r\n",cellblock[k].name, cellblocksave[0].time[l], cellblocksave[0].xvalue[l], cellblocksave[0].yvalue[l], cellblocksave[0].level[l] ,cellblocksave[0].anz[l] );
			m_LB_data.AddString(str);
			}
	}
}

for (int p=zeitp2; p<zeitp1+1; p++)
{
	for (k=0;k<(maxcells);k++)
		{
		if (strncmp(cellblock[k].name,"AB",2)==0 && strncmp(cellblock[k].name,"ABpla",5)!=0)	//Nur die Zellen mit dem Anfang "AB..." werden korrigiert ABpla wird ignoriert
	
			{
			for (int y=0;y<cellblock[k].nrofcoord;y++)						
				{
				if(cellblock[k].time[y]==cellblocksave[0].time[p-zeitp2])
					{
					cellblock[k].xvalue[y]=cellblock[k].xvalue[y]-cellblocksave[0].xvalue[p-zeitp2]+offsetx;
					cellblock[k].yvalue[y]=cellblock[k].yvalue[y]-cellblocksave[0].yvalue[p-zeitp2]+offsety;
					cellblock[k].level[y]=cellblock[k].level[y]-cellblocksave[0].level[p-zeitp2]+offsetlevel;
					}
				}
			}


		if (strcmp(cellblock[k].name,"Daaa")==0 ||  strcmp(cellblock[k].name,"Daap")==0)	//Daaa und Daap sind Zellen die eine Körperachse markieren
	
			{
			for (int y=0;y<cellblock[k].nrofcoord;y++)						
				{
				if(cellblock[k].time[y]==cellblocksave[0].time[p-zeitp2])
					{
					cellblock[k].xvalue[y]=cellblock[k].xvalue[y]-cellblocksave[0].xvalue[p-zeitp2]+offsetx;
					cellblock[k].yvalue[y]=cellblock[k].yvalue[y]-cellblocksave[0].yvalue[p-zeitp2]+offsety;
					cellblock[k].level[y]=cellblock[k].level[y]-cellblocksave[0].level[p-zeitp2]+offsetlevel;
					cellblock[k].shape[l]=4;
				if (strcmp(cellblock[k].name,"Daaa")==0)
						{
						cellblock[k].color[l]=255;
						cellblock[k].colorcell=255;
						}
				if (strcmp(cellblock[k].name,"Daap")==0)
						{
						cellblock[k].color[l]=256*255;
						cellblock[k].colorcell=256*255;
						}
					}
				}
			}
		}
	}
	GetDlgItem(IDC_CENTERREF)->EnableWindow(FALSE);
}


void CCell_Movement_calcDlg::OnBnClickedCalcmigration()
{
char str[256], refname [50];
m_time_start.GetWindowText(str,100);
int zeitp1=atoi(str);
m_time_end.GetWindowText(str,100);
int zeitp2=atoi(str);

vektor koordi1,koordi2,koordi3,koordi6,koordi4,koordi5;

	int n=1;
char buffer[256];
char filename[256];
char *pdest;
char migfile[256];
int result;
SYSTEMTIME st;
GetLocalTime(&st);
st.wMilliseconds;
int k, l, counter;
float migration, beeline, deviation,norm, zscal;
float migrationav, beelineav, deviationav, vmigav, vbeeav;
m_zscal.GetWindowText(str, 20);
zscal=atof(str);

m_norm1.GetWindowText(str,20);
norm = atof (str);

m_charref2.GetWindowTextA(refname, 50);

sprintf_s(filename,"%s_%s_%d_Ref%s", m_strResults,str, st.wMilliseconds, refname);

m_mod1.GetWindowText(str,10);
n=atoi(str);


DWORD dwBytes;
GetModuleFileName(NULL,buffer,256);			//Find actual path

pdest = strrchr(buffer, 'C');				//Overwrite String of .exe  ....
result = (int)(pdest - buffer + 1);
strncpy_s(str, buffer, (result-1));
str[result-1]='\0';							

strcpy_s(migfile, str);
strcat_s(migfile, "results/");
strcat_s(migfile,filename);					//...with result folder and result filename
strcat_s(migfile,"_mig.txt");


hmig=CreateFile(migfile,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);		//create resultfile

sprintf_s(str,"%s\r\n", filename);
WriteFile ( hmig, str,sizeof(char)*strlen(str),&dwBytes,NULL); 



if (m_3dcheck==true) sprintf_s(str,"zscal:\t %3.3f \tnorm:\t %3.3f\tmod:\t%d\t 3D\r\n", zscal, norm, n);
if (m_2dcheck==true) sprintf_s(str,"zscal:\t %3.3f \tnorm:\t %3.3f\tmod:\t%d\t 2D\r\n", zscal, norm, n);
WriteFile ( hmig, str,sizeof(char)*strlen(str),&dwBytes,NULL); 

m_LB_data.ResetContent();
sprintf_s(str,"name mig bee dev vmig vbee time0 timel nr\r\n");
m_LB_data.AddString(str);

sprintf_s(str,"name\t mig\t bee\t dev\t vmig\t vbee\t time0\t timel\t nr\t proj-t0 \t proj-tl\r\n");
WriteFile ( hmig, str,sizeof(char)*strlen(str),&dwBytes,NULL); 




			migrationav=0;
			beelineav=0;
			deviationav=0;
			vmigav=0;
			vbeeav=0;
			counter=0;



for (k=0;k<(maxcells);k++)
{
if (strncmp(cellblock[k].name,"AB",2)==0 && strncmp(cellblock[k].name,"ABpla",5)!=0)
	{
		if (cellblock[k].nrofcoord>0)
		{
			migration=0;
			beeline=0;
			deviation=0;
		
			int wert=fmod((float )cellblock[k].nrofcoord,(float) n);
			if (wert==0)
			{
				for (l=0;l<cellblock[k].nrofcoord-n;l=l+n)	
					{
				
					cellblock[k].time[l];
					koordi1.x=cellblock[k].xvalue[l];
					koordi1.y=cellblock[k].yvalue[l];
					koordi1.z=cellblock[k].level[l]*zscal;	
					koordi2.x=cellblock[k].xvalue[l+n];
					koordi2.y=cellblock[k].yvalue[l+n];
					koordi2.z=(cellblock[k].level[l+n])*zscal;
					if (m_3dcheck==true)	migration+=leastsqrvek(koordi1,koordi2, norm); //3D
					if (m_2dcheck==true)	migration+=leastsqrvek2D(koordi1,koordi2, norm);
					}
			}
			else
			{
					if(cellblock[k].nrofcoord>n)
					{
							for (l=0;l<cellblock[k].nrofcoord-n;l=l+n)	
								{
							
								cellblock[k].time[l];
								koordi1.x=cellblock[k].xvalue[l];
								koordi1.y=cellblock[k].yvalue[l];
								koordi1.z=cellblock[k].level[l]*zscal;	
								koordi2.x=cellblock[k].xvalue[l+n];
								koordi2.y=cellblock[k].yvalue[l+n];
								koordi2.z=(cellblock[k].level[l+n])*zscal;
								if (m_3dcheck==true) migration+=leastsqrvek(koordi1,koordi2, norm); //3D
								if (m_2dcheck==true) migration+=leastsqrvek2D(koordi1,koordi2, norm);//2D
								}
							//Rest
								l =cellblock[k].nrofcoord-n;
								int ende=cellblock[k].nrofcoord-1;
								cellblock[k].time[l];
								koordi1.x=cellblock[k].xvalue[l];
								koordi1.y=cellblock[k].yvalue[l];
								koordi1.z=cellblock[k].level[l]*zscal;	
								koordi2.x=cellblock[k].xvalue[ende];
								koordi2.y=cellblock[k].yvalue[ende];
								koordi2.z=(cellblock[k].level[ende])*zscal;
								if (m_3dcheck==true) migration+=leastsqrvek(koordi1,koordi2, norm); //3D
								if (m_2dcheck==true) migration+=leastsqrvek2D(koordi1,koordi2, norm);//2D
					}
					else			//if there a fewer cells left as rate n take as one step
					{
					l =0;
					int ende=cellblock[k].nrofcoord-1;
					cellblock[k].time[l];
					koordi1.x=cellblock[k].xvalue[l];
					koordi1.y=cellblock[k].yvalue[l];
					koordi1.z=cellblock[k].level[l]*zscal;	
					koordi2.x=cellblock[k].xvalue[ende];
					koordi2.y=cellblock[k].yvalue[ende];
					koordi2.z=(cellblock[k].level[ende])*zscal;
					if (m_3dcheck==true) migration+=leastsqrvek(koordi1,koordi2, norm); //3D
					if (m_2dcheck==true) migration+=leastsqrvek2D(koordi1,koordi2, norm);
					}

			}

			if(migration>0 && migration <1000)
			{

			koordi1.x=cellblock[k].xvalue[0];
			koordi1.y=cellblock[k].yvalue[0];
			koordi1.z=cellblock[k].level[0]*zscal;	
			koordi2.x=cellblock[k].xvalue[cellblock[k].nrofcoord-1];
			koordi2.y=cellblock[k].yvalue[cellblock[k].nrofcoord-1];
			koordi2.z=(cellblock[k].level[cellblock[k].nrofcoord-1])*zscal;
			if (m_3dcheck==true) beeline=leastsqrvek(koordi1,koordi2, norm);	//3D
			if (m_2dcheck==true) beeline=leastsqrvek2D(koordi1,koordi2, norm);
			deviation=migration/beeline;

			//projection on body axis*****************************************************
			
			for (int i=0;i<(maxcells);i++)
			{
				if (strcmp(cellblock[i].name,"Daaa")==0)	
					{
					for (int m=0;m<cellblock[i].nrofcoord;m++)	
						{
							if	(cellblock[i].time[m]==cellblock[k].time[0])		//body axis at first frame t=0: upper position
							{
							koordi3.x=cellblock[i].xvalue[m];
							koordi3.y=cellblock[i].yvalue[m];
							koordi3.z=cellblock[i].level[m]*zscal;	
							}
						
							if	(cellblock[i].time[m]==cellblock[k].time[cellblock[k].nrofcoord-1])		//body axis last frame t=last: upper pos
							{
							koordi4.x=cellblock[i].xvalue[m];
							koordi4.y=cellblock[i].yvalue[m];
							koordi4.z=(cellblock[i].level[m])*zscal;
							}
						}
					}
				if (strcmp(cellblock[i].name,"Daap")==0)
					{
					for (int m=0;m<cellblock[i].nrofcoord;m++)	
						{
							if	(cellblock[i].time[m]==cellblock[k].time[0])		//body axis at first frame t=0: lower position
							{
							koordi5.x=cellblock[i].xvalue[m];
							koordi5.y=cellblock[i].yvalue[m];
							koordi5.z=cellblock[i].level[m]*zscal;	
							}
						
							if	(cellblock[i].time[m]==cellblock[k].time[cellblock[k].nrofcoord-1])		//body axis at last frame t=last: lower position last frame
							{
							koordi6.x=cellblock[i].xvalue[m];
							koordi6.y=cellblock[i].yvalue[m];
							koordi6.z=(cellblock[i].level[m])*zscal;
							}
						}
					}

			}
		//Vector between upper reference point and cell at t=0
		koordi1.x=cellblock[k].xvalue[0]-koordi3.x;
		koordi1.y=cellblock[k].yvalue[0]-koordi3.y;
		koordi1.z=cellblock[k].level[0]*zscal-koordi3.z;	

		//body axis at  t=0:
		koordi3.x=koordi5.x-koordi3.x;
		koordi3.y=koordi5.y-koordi3.y;
		koordi3.z=koordi5.z-koordi3.z;

		 float SkProdukt0=koordi1.x*koordi3.x+koordi1.y*koordi3.y; //2-Dim
		 float BetragVek0=sqrt(koordi3.x*koordi3.x+koordi3.y*koordi3.y); //2-Dim
		 float Proj0=SkProdukt0/BetragVek0;
		 Proj0=Proj0/BetragVek0;
		
		//Vector between upper reference point and cell at t=last;  
		koordi2.x=cellblock[k].xvalue[cellblock[k].nrofcoord-1]-koordi4.x;
		koordi2.y=cellblock[k].yvalue[cellblock[k].nrofcoord-1]-koordi4.y;
		koordi2.z=cellblock[k].level[cellblock[k].nrofcoord-1]*zscal-koordi4.z;	

			//body axis at  t=last:
		koordi4.x=koordi6.x-koordi4.x;
		koordi4.y=koordi6.y-koordi4.y;
		koordi4.z=koordi6.z-koordi4.z;

		 float SkProduktl=koordi2.x*koordi4.x+koordi2.y*koordi4.y; //2-Dim
		 float BetragVekl=sqrt(koordi4.x*koordi4.x+koordi4.y*koordi4.y); //2-Dim
		 float Projl=SkProduktl/BetragVekl;
		 Projl=Projl/BetragVekl;

		 //end projection on body axis*****************************************************


		

			sprintf_s(str,"%s %2.5f %2.5f %2.2f %2.2f %2.2f %d %d %d %2.3f % 2.3f\r\n",cellblock[k].name, migration, beeline, deviation, migration/cellblock[k].nrofcoord,beeline/cellblock[k].nrofcoord, cellblock[k].time[0],cellblock[k].time[cellblock[k].nrofcoord-1],cellblock[k].nrofcoord,Proj0,Projl );
			m_LB_data.AddString(str);
			sprintf_s(str,"%s\t %2.5f\t %2.5f\t %2.2f\t %2.5f\t %2.5f\t %d\t %d\t %d\t %2.3f\t %2.3f\r\n",cellblock[k].name, migration, beeline, deviation, migration/cellblock[k].nrofcoord,beeline/cellblock[k].nrofcoord, cellblock[k].time[0],cellblock[k].time[cellblock[k].nrofcoord-1],cellblock[k].nrofcoord,Proj0,Projl );
			WriteFile ( hmig, str,sizeof(char)*strlen(str),&dwBytes,NULL); 

			migrationav+=migration;
			beelineav+=beeline;
			deviationav+=deviation;
			vmigav+=migration/cellblock[k].nrofcoord;
			vbeeav+=beeline/cellblock[k].nrofcoord;
			counter++;
			}

		}
	}
		if (strcmp(cellblock[k].name,"Dppp")==0) 
		{
		if (cellblock[k].nrofcoord>0)
		{
			migration=0;
			beeline=0;
			deviation=0;
		

			int wert=fmod((float )cellblock[k].nrofcoord,(float) n);
			for (l=0;l<cellblock[k].nrofcoord-n;l=l+n)	
			{
				
				cellblock[k].time[l];
				koordi1.x=cellblock[k].xvalue[l];
				koordi1.y=cellblock[k].yvalue[l];
				koordi1.z=cellblock[k].level[l]*zscal;	
				koordi2.x=cellblock[k].xvalue[l+n];
				koordi2.y=cellblock[k].yvalue[l+n];
				koordi2.z=(cellblock[k].level[l+n])*zscal;
				if (m_3dcheck==true) migration+=leastsqrvek(koordi1,koordi2, norm);	//3D
				if (m_2dcheck==true) migration+=leastsqrvek2D(koordi1,koordi2, norm);
			}

			koordi1.x=cellblock[k].xvalue[0];
			koordi1.y=cellblock[k].yvalue[0];
			koordi1.z=cellblock[k].level[0]*zscal;	
			koordi2.x=cellblock[k].xvalue[cellblock[k].nrofcoord-1];
			koordi2.y=cellblock[k].yvalue[cellblock[k].nrofcoord-1];
			koordi2.z=(cellblock[k].level[cellblock[k].nrofcoord-1])*zscal;
			if (m_3dcheck==true) beeline=leastsqrvek(koordi1,koordi2, norm);	//3D
			if (m_2dcheck==true) beeline=leastsqrvek2D(koordi1,koordi2, norm);
			deviation=migration/beeline;

			
			sprintf_s(str,"Control %2.5f %2.5f %2.2f %2.2f %2.2f %d %d %d\r\n", migration, beeline, deviation, migration/cellblock[k].nrofcoord,beeline/cellblock[k].nrofcoord, cellblock[k].time[0],cellblock[k].time[cellblock[k].nrofcoord-1],cellblock[k].nrofcoord );
			m_LB_data.AddString(str);
			sprintf_s(str,"Control \t%2.5f\t %2.5f\t %2.2f\t %2.5f\t %2.5f\t %d\t %d\t %d\r\n", migration, beeline, deviation, migration/cellblock[k].nrofcoord,beeline/cellblock[k].nrofcoord, cellblock[k].time[0],cellblock[k].time[cellblock[k].nrofcoord-1],cellblock[k].nrofcoord );
			WriteFile ( hmig, str,sizeof(char)*strlen(str),&dwBytes,NULL); 

			}
		}
	}


migrationav=migrationav/counter;
beelineav=beelineav/counter;
deviationav=deviationav/counter;
vmigav=vmigav/counter;
vbeeav=vbeeav/counter;
sprintf_s(str,"avmig: %2.2f  avbee: %2.2f  avdev: %2.2f vmigav: %2.5f vbeeav: %2.5f nrofcells: %d\r\n",migrationav, beelineav, deviationav,vmigav,vbeeav, counter );
m_LB_data.AddString(str);
sprintf_s(str,"avmig:\t %2.5f\t  avbee:\t %2.5f \t avdev:\t %2.2f\t vmigav:\t %2.5f\t vbeeav:\t %2.5f\t nrofcells:\t %d\r\n",migrationav, beelineav, deviationav,vmigav,vbeeav, counter );
WriteFile ( hmig, str,sizeof(char)*strlen(str),&dwBytes,NULL); 

CloseHandle(hmig);	

}


double CCell_Movement_calcDlg::leastsqrvek(vektor koord1, vektor koord2, double norm)			
{																		
double erg;
float deltax,deltay,deltaz;
deltax=(float)(koord1.x-koord2.x)/norm;
deltay=(float)(koord1.y-koord2.y)/norm;
deltaz=(float)(koord1.z-koord2.z)/norm;
erg=sqrt( ((deltax)*(deltax)) + ((deltay)*(deltay) )+ ( (deltaz)*(deltaz) ) );
return(erg);
}


double CCell_Movement_calcDlg::leastsqrvek2D(vektor koord1, vektor koord2, double norm)
{
double erg;
float deltax,deltay;
deltax=(float)(koord1.x-koord2.x)/norm;
deltay=(float)(koord1.y-koord2.y)/norm;
erg=sqrt( ((deltax)*(deltax)) + ((deltay)*(deltay) ));
return(erg);
}

void CCell_Movement_calcDlg::OnBnClicked2d()
{
m_2dcheck=TRUE;
m_3dcheck=FALSE;
m_DimCheckButtons[1].SetCheck(1);
m_DimCheckButtons[0].SetCheck(0);
}


void CCell_Movement_calcDlg::OnBnClicked3d()
{
m_2dcheck=FALSE;
m_3dcheck=TRUE;
m_DimCheckButtons[1].SetCheck(0);
m_DimCheckButtons[0].SetCheck(1);
}


void CCell_Movement_calcDlg::OnBnClickedFindFrames()
{
char str[20];
int a=0;
int last=0;
int var, numgen;
numgen=10;
m_frames=(timehistogram*)malloc(100000*sizeof(timehistogram));


int timecount=0;

{
	for (unsigned int k=0; k<maxcells; k++)
	{
		if ((cellblock[k].nrofcoord>0) && (cellblock[k].nrofcoord <100000)) 
			{
				var = cellblock[k].nrofcoord;
			
				for (unsigned int j=0; j<var; j++)
					{
					if ((cellblock[k].time[j]>0) && (cellblock[k].time[j] <100000))
						{
							if (cellblock[k].xvalue[j]>0 && cellblock[k].xvalue[j]<10000 && cellblock[k].yvalue[j]>0 && cellblock[k].yvalue[j]<10000 /*&& cellblock[k].level[j]>0 && cellblock[k].level[j]<100*/)
							{
							m_frames[timecount].timeframe=cellblock[k].time[j];
							timecount++;
							
							}
						}
					}
			
		
			}
		
		else var=0;
	}
}


for (unsigned int i=0;i<(timecount);i++)					//Compare alle framevalues
	{
		m_frames[i].rate=0;
	for (unsigned int l=0;l<timecount;l++)
		{	
		if ((m_frames[i].timeframe == m_frames[l].timeframe)) 
			{
			m_frames[i].rate++;							//count equality
			}
		}
	}

m_LB_data.ResetContent();  


	for (unsigned int m=0;m<100;m++)
			{
			framearray[m]=0;
			}

for (unsigned int i=0;i<(timecount);i++)							
	{
	if (m_frames[i].rate > (dualexp(numgen-2)-1) )
		{
		for (unsigned int m=0;m<100;m++)
			{
			if (framearray[m]==m_frames[i].timeframe) break;
			else if (framearray[m]==0) 
					{
					framearray[m]=m_frames[i].timeframe;
					sprintf_s(str,"%d  %d",m_frames[i].timeframe,m_frames[i].rate);
					m_LB_data.AddString(str);	
					a = i;
					break;
					}
			}
		}
	else 
		{
			{
			sprintf_s(str,"%d  %d",m_frames[i].timeframe,m_frames[i].rate);
			m_LB_data.AddString(str);
			last=m_frames[i].timeframe;
			}
		}
		
	}

sprintf_s(str,"%d" ,m_frames[a].timeframe);		// show first frame (time)
GetDlgItem(IDC_TIMESTART)->SetWindowText(str);

sprintf_s(str,"%d" ,last);						// show last frame (time)
GetDlgItem(IDC_TIMEEND)->SetWindowText(str);

free(m_frames);

}


int CCell_Movement_calcDlg::dualexp(int argum)

{

int i,basis;

basis=2;

for (i=0;i<(argum-1);i++)

basis*=2;

return(basis);

}

