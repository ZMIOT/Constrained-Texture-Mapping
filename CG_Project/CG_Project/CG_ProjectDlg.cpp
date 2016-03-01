
// CG_ProjectDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "CG_Project.h"
#include "CG_ProjectDlg.h"
#include "afxdialogex.h"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<float> d;
vector<vector<float>> vn;
vector<float> vnn;

float triangleA( float x1, float y1, float x2, float y2, float x3, float y3) {
    float result = abs((x1 * y2 + x2 * y3 + x3 * y1 - x2 * y1 - x3 * y2 - x1 * y3) / 2);
    return result;
}

bool inTri(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3) {
    float triangleArea = triangleA(x1, y1, x2, y2, x3, y3);
    float area = triangleA(px ,py, x1, y1, x2, y2);
    area += triangleA(px ,py, x1, y1, x3, y3);
    area += triangleA(px ,py, x2, y2, x3, y3);
    float epsilon = 0.0001;
    if (abs(triangleArea - area) < epsilon) {
        return true;
    }
    return false;
}

float Distance(float dX0, float dY0, float dX1, float dY1)
{
    return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
}


float FindDistanceToSegment(float x1, float y1, float x2, float y2, float pointX, float pointY)
{
    float diffX = x2 - x1;
    float diffY = y2 - y1;
    if ((diffX == 0) && (diffY == 0))
    {
        diffX = pointX - x1;
        diffY = pointY - y1;
        return sqrt(diffX * diffX + diffY * diffY);
    }

    float t = ((pointX - x1) * diffX + (pointY - y1) * diffY) / (diffX * diffX + diffY * diffY);

    if (t < 0)
    {
        //point is nearest to the first point i.e x1 and y1
        diffX = pointX - x1;
        diffY = pointY - y1;
    }
    else if (t > 1)
    {
        //point is nearest to the end point i.e x2 and y2
        diffX = pointX - x2;
        diffY = pointY - y2;
    }
    else
    {
        //if perpendicular line intersect the line segment.
        diffX = pointX - (x1 + t * diffX);
        diffY = pointY - (y1 + t * diffY);
    }

    //returning shortest distance
    //return sqrt(diffX * diffX + diffY * diffY);

	float a = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2) );
	float b = sqrt( (x2-pointX) * (x2-pointX) + (y2-pointY) * (y2-pointY) );
	float c = sqrt( (pointX-x1) * (pointX-x1) + (pointY-y1) * (pointY-y1) );
	float s = (a+b+c)/2;
	float area = sqrt(s*(s-a)*(s-b)*(s-c));

	return area*2/a;
}

void MovePoint( float &tmpx, float &tmpy, float &x1, float &y1, float x2, float y2, float x3, float y3, float x_vector, float y_vector, int order)
{
	float height1 = FindDistanceToSegment(x2, y2, x3, y3, x1, y1);
	float height2 = FindDistanceToSegment(x2, y2, x3, y3, tmpx, tmpy);
	float proportion_y = height2/height1;
	
	float height3 = FindDistanceToSegment(x1, y1, x2, y2, tmpx, tmpy);
	float height4 = FindDistanceToSegment(x3, y3, x1, y1, tmpx, tmpy);
	if(height1==0 || height2==0 || height3==0 || height4==0)
	{
		tmpx += x_vector*d[order];
		tmpy -= y_vector*d[order];
	}
	else
	{
		tmpx += x_vector*proportion_y;
		tmpy -= y_vector*proportion_y;
		d[order] = proportion_y;
	}
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<int> feature;
vector<vector<int>> tri;

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCG_ProjectDlg 對話方塊



CCG_ProjectDlg::CCG_ProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCG_ProjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCG_ProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//////////////////////////////////////////////////////////////////////////////
	DDX_Control(pDX, IDC_OPENGL_SHOW, m_Show);     //??????????????????????????????????????
	DDX_Control(pDX, IDC_TEXTURE, m_TEXTURE);
	//////////////////////////////////////////////////////////////////////////////
	
}

BEGIN_MESSAGE_MAP(CCG_ProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOADFILE, &CCG_ProjectDlg::OnBnClickedLoadfile)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON1, &CCG_ProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CCG_ProjectDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CCG_ProjectDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CCG_ProjectDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CCG_ProjectDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_BUTTON2, &CCG_ProjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK5, &CCG_ProjectDlg::OnBnClickedCheck5)
END_MESSAGE_MAP()


// CCG_ProjectDlg 訊息處理常式

BOOL CCG_ProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	////////////////////////////////////////////////////////////////////////////////////////////????
	WINDOWPLACEMENT aa;                      //抓位置?
	m_Show.GetWindowPlacement(&aa);
	SHOW_RECT = aa.rcNormalPosition;

	qwnd = GetDlgItem(IDC_OPENGL_SHOW);     // DLG Call Picture
	m_hDC = ::GetDC(qwnd->m_hWnd);			// Get the Device context

	qwnd->GetClientRect(&AreaRect);         //??
	qwnd->GetWindowRect(&WinRect);          //??
	ScreenToClient(&WinRect);               //??

	SetupPixelFormat();                     //opengl to mfc??
	m_hRC = wglCreateContext(m_hDC);        //??

	wglMakeCurrent(m_hDC, m_hRC);           //??

	GLReshapeWindow(AreaRect.right,AreaRect.bottom);
	width = AreaRect.right;
	height = AreaRect.bottom;

	//////////////////////////////////////////////////////////////////////////  5/25

	WINDOWPLACEMENT bb;                      //抓位置?
	m_TEXTURE.GetWindowPlacement(&bb);
	SHOW_RECT2 = bb.rcNormalPosition;

	qwnd2 = GetDlgItem(IDC_TEXTURE);     // DLG Call Picture
	m_hDC2 = ::GetDC(qwnd2->m_hWnd);			// Get the Device context

	qwnd2->GetClientRect(&AreaRect2);         //??
	qwnd2->GetWindowRect(&WinRect2);          //??
	ScreenToClient(&WinRect2);               //??

	SetupPixelFormat();                     //opengl to mfc??
	m_hRC2 = wglCreateContext(m_hDC2);        //??

	wglMakeCurrent(m_hDC2, m_hRC2);           //??

	GLReshapeWindow(AreaRect2.right,AreaRect2.bottom);
	width2 = AreaRect2.right;
	height2 = AreaRect2.bottom;

	whichvt=-1;
	//////////////////////////////////////////////////////////////////////////

	GLInit(NULL);
	////////////////////////////////////////////////////////////////////////////////////////////

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CCG_ProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CCG_ProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		GLDisplay();/////////////////////////////////////////////////////////////////////////////////////
		GLDisplay2();/////////////////////////////////////////////////////////////////////////////////////
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CCG_ProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCG_ProjectDlg::SetupPixelFormat()
{
	// Local Variables 
	int PixelFormat;					// Pixel format index

	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		24,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		32,                    // 32-bit z-buffer	 
		0,                     // no stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	}; 

	////////////////////////////////////////////////////////////////////////////
	if((PixelFormat = ChoosePixelFormat(m_hDC, &pfd)) == 0) 
		return FALSE;

	if(SetPixelFormat(m_hDC, PixelFormat, &pfd) == FALSE)
			return FALSE;

	if((PixelFormat = ChoosePixelFormat(m_hDC2, &pfd)) == 0) 
		return FALSE;

	if(SetPixelFormat(m_hDC2, PixelFormat, &pfd) == FALSE)
			return FALSE;
	/////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

//Reshape Window Handler
void CCG_ProjectDlg::GLReshapeWindow(GLsizei w,GLsizei h)
{
	wglMakeCurrent(m_hDC, m_hRC); //****************************
	//wglMakeCurrent(m_hDC2, m_hRC2); //****************************

	glViewport( 0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -25);

}

//Set Texture Mapping Parameters
void CCG_ProjectDlg::GLInit(void *pData)
{
	wglMakeCurrent(m_hDC, m_hRC);
	wglMakeCurrent(m_hDC2, m_hRC2);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//將清除螢幕的顏色設成黑色	
	glShadeModel(GL_SMOOTH);

	// Load file and decode image.
	const char* filename = "lion.png";
	unsigned image_w, image_h;
	unsigned error = lodepng::decode(image, image_w, image_h, filename);
	show_tex = true;
	show_map = true;
	show_feature = true;
	show_fmesh = true;
	tex = true;
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(TRUE);
}

void CCG_ProjectDlg::GLDisplay(void)
{
	wglMakeCurrent(m_hDC, m_hRC);//****************************************************
	//wglMakeCurrent(m_hDC2, m_hRC2);//****************************************************
	
	// Enable the texture for OpenGL.
	if(tex)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	//Light
	GLfloat light_position[]={0.0, 0.0, 30.0, 1.0};
	GLfloat	light_ambient[]={0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat	light_specular[]={ 0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat light_diffuse[]={0.3f, 0.3f, 0.3f, 1.0f };

	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	
	GLfloat	ms[]={1.0, 1.0, 1.0};
	GLfloat shininess[]={30.0};
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,ms);
	glMaterialfv(GL_FRONT,GL_SHININESS,shininess);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	//透视投影变换  
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();  
	gluPerspective(80, width/height, 1, 200.0);

	//视角变换  
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();
    gluLookAt(0,0,60, 0,0,0, 0,1,0);  

	glTranslatef(xTrans, yTrans, 0.0f);
	glTranslatef(0.0f, 0.0f, -zTrans);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		for(int i=0;i<ft.size();i++)
		{
			glNormal3f(vn[fv[i][0]][0],vn[fv[i][0]][1],vn[fv[i][0]][2]);
			glTexCoord2f(vt[ft[i][0]][0],1-vt[ft[i][0]][1]);
			glVertex3f(v[fv[i][0]][0]*30,v[fv[i][0]][1]*30,v[fv[i][0]][2]*30);
			
			glNormal3f(vn[fv[i][1]][0],vn[fv[i][1]][1],vn[fv[i][1]][2]);
			glTexCoord2f(vt[ft[i][1]][0],1-vt[ft[i][1]][1]);
			glVertex3f(v[fv[i][1]][0]*30,v[fv[i][1]][1]*30,v[fv[i][1]][2]*30);
			
			glNormal3f(vn[fv[i][2]][0],vn[fv[i][2]][1],vn[fv[i][2]][2]);
			glTexCoord2f(vt[ft[i][2]][0],1-vt[ft[i][2]][1]);
			glVertex3f(v[fv[i][2]][0]*30,v[fv[i][2]][1]*30,v[fv[i][2]][2]*30);
		}
	glEnd();

	SwapBuffers(wglGetCurrentDC());
}

void CCG_ProjectDlg::GLDisplay2(void)
{
	//wglMakeCurrent(m_hDC, m_hRC);//****************************************************
	wglMakeCurrent(m_hDC2, m_hRC2);//****************************************************
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

	// Enable the texture for OpenGL.
	
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	if(show_tex)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES); 
			glTexCoord2f( 1, 0);
			glVertex2f( 1, 1);
			glTexCoord2f( 0, 0);
			glVertex2f( -1, 1);
			glTexCoord2f( 0, 1);
			glVertex2f( -1, -1);
		
			glTexCoord2f( 1, 0);
			glVertex2f( 1, 1);
			glTexCoord2f( 0, 1);
			glVertex2f( -1, -1);
			glTexCoord2f( 1, 1);
			glVertex2f( 1, -1);
		glEnd();
	}

	if(show_map)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		for(int j=0;j<ft.size();j++)
		{
			glVertex2f( vt[ft[j][0]][0]*2-1, vt[ft[j][0]][1]*2-1);
			glVertex2f( vt[ft[j][1]][0]*2-1, vt[ft[j][1]][1]*2-1);
			glVertex2f( vt[ft[j][2]][0]*2-1, vt[ft[j][2]][1]*2-1);
		}
		glEnd();
	}

	if(show_fmesh)
	{
		glColor3f(0.0f, 1.0f, 0.0f); // Green
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		for(int j=0;j<tri.size();j++)
		{
			int p1=feature[tri[j][0]],p2=feature[tri[j][1]],p3=feature[tri[j][2]];
			glVertex2f( vt[p1][0]*2-1, vt[p1][1]*2-1);
			glVertex2f( vt[p2][0]*2-1, vt[p2][1]*2-1);
			glVertex2f( vt[p3][0]*2-1, vt[p3][1]*2-1);
		}
		glEnd();
	}
	if(show_feature)
	{
		glColor3f(1.0f, 0.0f, 0.0f); // Red
		glPointSize(8);
		glBegin(GL_POINTS);     
		for(int j=0;j<feature.size();j++)
		{	    
			glVertex2f( vt[feature[j]][0]*2-1, vt[feature[j]][1]*2-1);
		}
		glEnd();
	}
	glColor3f(1.0f, 1.0f, 1.0f); // White

	SwapBuffers(wglGetCurrentDC());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void CCG_ProjectDlg::OnBnClickedLoadfile()
{
	AllocConsole();
	freopen("CONOUT$","w",stdout);

	CFileDialog FileDlg(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"",this);
	if(FileDlg.DoModal()==IDOK)
	{
		
		CString PathName=FileDlg.GetPathName();		

		string spath((LPCTSTR)PathName);
		ifstream fin ( spath , ifstream::in );

		v.clear();
		vt.clear();
		fv.clear();
		ft.clear();
		d.clear();
		vn.clear();
		vnn.clear();
		vector<float> tmpn;
		tmpn.push_back(0);
		tmpn.push_back(0);
		tmpn.push_back(0);
		
		string mark;
		char s;
		float tmpx,tmpy,tmpz;
		int v1,v2,v3,t1,t2,t3;
		
		fin >> mark;
		while(mark != "v")
			fin >> mark;
		while(mark == "v")
		{
			fin >> tmpx >> tmpy >> tmpz;
			vector<float> tmpv;
			tmpv.push_back(tmpx);
			tmpv.push_back(tmpy);
			tmpv.push_back(tmpz);
			v.push_back(tmpv);
			vn.push_back(tmpn);
			vnn.push_back(0);

			fin >> mark;
		}

		while(mark != "vt")
			fin >> mark;
		while(mark == "vt")
		{
			fin >> tmpx >> tmpy;
			vector<float> tmpvt;
			tmpvt.push_back(tmpx);
			tmpvt.push_back(tmpy);
			vt.push_back(tmpvt);
			d.push_back(0);

			fin >> mark;
		}

		while(mark != "f")
			fin >> mark;
		while(fin >> v1 >> s >> t1 >> v2 >> s >> t2 >> v3 >> s >> t3)
		{			
			vector<int> tmpfv;
			vector<int> tmpft;
			tmpfv.push_back(v1-1);
			tmpfv.push_back(v2-1);
			tmpfv.push_back(v3-1);
			tmpft.push_back(t1-1);
			tmpft.push_back(t2-1);
			tmpft.push_back(t3-1);
			fv.push_back(tmpfv);
			ft.push_back(tmpft);

			fin >> mark;
		}
		for(unsigned int j=0;j<fv.size();j++)
		{	
			int p1=fv[j][0];
			int p2=fv[j][1];
			int p3=fv[j][2];
			float nx = (v[p2][1]-v[p1][1])*(v[p3][2]-v[p1][2]) - (v[p2][2]-v[p1][2])*(v[p3][1]-v[p1][1]);
			float ny = (v[p2][2]-v[p1][2])*(v[p3][0]-v[p1][0]) - (v[p2][0]-v[p1][0])*(v[p3][2]-v[p1][2]);
			float nz = (v[p2][0]-v[p1][0])*(v[p3][1]-v[p1][1]) - (v[p2][1]-v[p1][1])*(v[p3][0]-v[p1][0]);
			for(unsigned int i=0;i<3;i++)
			{
				vn[fv[j][i]][0]+=nx;
				vn[fv[j][i]][1]+=ny;
				vn[fv[j][i]][2]+=nz;
				vnn[fv[j][i]]+=sqrt(nx*nx+ny*ny+nz*nz);
			}
		}
		for(unsigned int j=0;j<vn.size();j++)
		{
			for(unsigned int i=0;i<3;i++)
			{
				vn[j][i] /= vnn[j]; 
				//cout << vn[j][i] << " ";
			}
			//cout << endl;
		}
		////////////////////////////////////////////////////////////////////////////////
		feature.clear();
		tri.clear();
		vector<int> tmp;
		int points,tri1,tri2,tri3;
		char ch;
		ifstream tin(spath+".txt", ifstream::in );		
	
		while(tin >> ch)
		{
			if(ch == 'p')
			{
				tin >> points;
				feature.push_back(points);
			}
			if(ch == 't')
			{
				tin >> tri1 >> tri2 >> tri3;
				tmp.clear();
				tmp.push_back(tri1-1);
				tmp.push_back(tri2-1);
				tmp.push_back(tri3-1);
				tri.push_back(tmp);
			}
		}
		////////////////////////////////////////////////////////////////////////////////
	}

	xTrans=0.0f;
	yTrans=0.0f;
	zTrans=0.0f;
	xRot=0.0f;
	yRot=0.0f;
	
	show_tex = 1;
	show_map = 1;
	show_feature = 1;
	show_fmesh = 1;
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(TRUE);

	GLDisplay();
	GLDisplay2();
}

void CCG_ProjectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_OPENGL_SHOW);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CRect rect2;
	CWnd *pWnd2 = GetDlgItem(IDC_TEXTURE);
	pWnd2->GetWindowRect(&rect2);
	ScreenToClient(&rect2);

	if(point.x>=rect.left && point.x<=rect.right && point.y>=rect.top && point.y<=rect.bottom)
	{
		int diffX = (int)(point.x - m_fLastX);
		int diffY = (int)(point.y - m_fLastY);
		m_fLastX  = (float)point.x;
		m_fLastY  = (float)point.y;

		// Left mouse button
		if (nFlags & MK_LBUTTON)
		{
			xRot += (float)0.5f * diffY;

			if ((xRot > 360.0f) || (xRot < -360.0f))
			{
				xRot = 0.0f;
			}

			yRot += (float)0.5f * diffX;

			if ((yRot > 360.0f) || (yRot < -360.0f))
			{
				yRot = 0.0f;
			}
			GLDisplay();
		}

		// Right mouse button
		else if (nFlags & MK_RBUTTON)
		{
			zTrans -= (float)0.1f * diffY;
			GLDisplay();
		}

		// Middle mouse button
		else if (nFlags & MK_MBUTTON)
		{
			xTrans += (float)0.05f * diffX;
			yTrans -= (float)0.05f * diffY;
			GLDisplay();
		}		
	}

	if(point.x>=rect2.left && point.x<=rect2.right && point.y>=rect2.top && point.y<=rect2.bottom)
	{
		int diffX = (int)(point.x - m_fLastX);
		int diffY = (int)(point.y - m_fLastY);
		m_fLastX  = (float)point.x;
		m_fLastY  = (float)point.y;

		// Left mouse button
		if (nFlags & MK_LBUTTON)
		{
			if(whichvt==-1)
			{
				for(int j=0;j<feature.size();j++)
				{
					float tmpx = vt[feature[j]][0]*2-1;
					float tmpy = vt[feature[j]][1]*2-1;
					float nowx = ((m_fLastX-rect2.left) / width2)*2-1 ;
					float nowy = -(((m_fLastY-rect2.top) / height2)*2-1) ;
					if( sqrt((nowx-tmpx)*(nowx-tmpx)+(nowy-tmpy)*(nowy-tmpy)) < 0.1 )
					{						
						whichvt = j;
						cout << whichvt;
						break;
					}
				}
			}
			else
			{
				vt[feature[whichvt]][0] += (float) diffX / width2;
				vt[feature[whichvt]][1] -= (float) diffY / height2;
				for(int b=0;b<tri.size();b++)
				{
					if(tri[b][0]==whichvt)
					{
						int p1 = feature[tri[b][0]], p2 = feature[tri[b][1]], p3 = feature[tri[b][2]];
						for(int a=0;a<vt.size();a++)
						{							
							if(inTri(vt[a][0],vt[a][1], vt[p1][0],vt[p1][1], vt[p2][0],vt[p2][1], vt[p3][0],vt[p3][1]) && a!=p1 && a!=p2 && a!=p3)
							{
								MovePoint( vt[a][0],vt[a][1], vt[p1][0],vt[p1][1], vt[p2][0],vt[p2][1], vt[p3][0],vt[p3][1], diffX/width2, diffY/height2, a);
							}
						}
					}
					if(tri[b][1]==whichvt)
					{
						int p1 = feature[tri[b][1]], p2 = feature[tri[b][0]], p3 = feature[tri[b][2]];
						for(int a=0;a<vt.size();a++)
						{							
							if(inTri(vt[a][0],vt[a][1], vt[p1][0],vt[p1][1], vt[p2][0],vt[p2][1], vt[p3][0],vt[p3][1]) && a!=p1 && a!=p2 && a!=p3)
							{
								MovePoint( vt[a][0],vt[a][1], vt[p1][0],vt[p1][1], vt[p2][0],vt[p2][1], vt[p3][0],vt[p3][1], diffX/width2, diffY/height2, a);
							}
						}
					}
					if(tri[b][2]==whichvt)
					{
						int p1 = feature[tri[b][2]], p2 = feature[tri[b][1]], p3 = feature[tri[b][0]];
						for(int a=0;a<vt.size();a++)
						{							
							if(inTri(vt[a][0],vt[a][1], vt[p1][0],vt[p1][1], vt[p2][0],vt[p2][1], vt[p3][0],vt[p3][1]) && a!=p1 && a!=p2 && a!=p3)
							{
								MovePoint( vt[a][0],vt[a][1], vt[p1][0],vt[p1][1], vt[p2][0],vt[p2][1], vt[p3][0],vt[p3][1], diffX/width2, diffY/height2, a);
							}
						}
					}
				}
			}
			GLDisplay();
			GLDisplay2();
		}
		else
		{
			whichvt = -1;
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}




void CCG_ProjectDlg::OnBnClickedButton1()
{
	CFileDialog FileDlg(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"",this);
	if(FileDlg.DoModal()==IDOK)
	{		
		CString PathName=FileDlg.GetPathName();	
		string spath((LPCTSTR)PathName);
		image.clear();
		unsigned image_w, image_h;
		unsigned error = lodepng::decode(image, image_w, image_h, spath);
	}

	GLDisplay();
	GLDisplay2();
}


void CCG_ProjectDlg::OnBnClickedCheck1()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK1);
	show_tex = pBtn -> GetCheck();
	GLDisplay2();
}


void CCG_ProjectDlg::OnBnClickedCheck2()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK2);
	show_map = pBtn -> GetCheck();
	GLDisplay2();
}


void CCG_ProjectDlg::OnBnClickedCheck3()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK3);
	show_feature = pBtn -> GetCheck();
	GLDisplay2();
}


void CCG_ProjectDlg::OnBnClickedCheck4()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK4);
	show_fmesh = pBtn -> GetCheck();
	GLDisplay2();
}


void CCG_ProjectDlg::OnBnClickedCheck5()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK5);
	tex = pBtn -> GetCheck();
	GLDisplay();
}


void CCG_ProjectDlg::OnBnClickedButton2()
{
	int w = 1280;
	int h = 960;
	bool open = true;
    cv::Mat frame;
    cv::VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);
	CvRect rect = cvRect(160,0,1120,960);
    if(!cap.isOpened())
    //如果沒有開啟成功，則離開程式
    {
        cout << "No Camera!" << endl;
    }
    while(open)
    {   
        if(!cap.read(frame))
            break;
        Mat src = frame(Rect(160,0,1120,960));
		resize(src, src, Size(1024,1024));
        imshow( "Camera",  src );
		char key = (char) waitKey(30);
		//delay N millis, usually long enough to display and capture input
		switch (key){
			case 'q':
			case 'Q':
			case 27: //escape key
			  break;
			case ' ': //Save an image
			  imwrite("texture.png", src);
			  cout << "Saved." << endl;			 
			  open=false;
			  break;
			default:
			  break;
		}
    }
	destroyWindow("Camera");
	image.clear();
	unsigned image_w, image_h;
	unsigned error = lodepng::decode(image, image_w, image_h, "texture.png");
	GLDisplay();
	GLDisplay2();
}

