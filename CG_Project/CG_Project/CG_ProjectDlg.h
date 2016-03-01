
// CG_ProjectDlg.h : 標頭檔
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "afxwin.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// CCG_ProjectDlg 對話方塊
class CCG_ProjectDlg : public CDialogEx
{
// 建構
public:
	CCG_ProjectDlg(CWnd* pParent = NULL);	// 標準建構函式

	//////////////////////////////////////////////////////////////////////
	CRect AreaRect,WinRect;
	HDC m_hDC;           // DC 緩衝??
	HGLRC m_hRC;         // GLRC 框框??
	CWnd *qwnd;          // MFC的東西??

	CRect AreaRect2,WinRect2;
	HDC m_hDC2;           // DC 緩衝??
	HGLRC m_hRC2;         // GLRC 框框??
	CWnd *qwnd2;          // MFC的東西??

	RECT SHOW_RECT;
	RECT SHOW_RECT2;
	CStatic	m_Show;
	CStatic m_TEXTURE;

	void GLReshapeWindow(GLint w, GLint h);
	bool SetupPixelFormat();
	void GLInit(void *pData);
	void GLDisplay();
	void GLDisplay2();

	GLfloat width, height;
	GLfloat width2, height2;

	vector<vector<float>> v;
	vector<vector<float>> vt;
	vector<vector<int>> fv;
	vector<vector<int>> ft;

	float m_fLastX;
	float m_fLastY;
	float zRot;
	float yRot;
	float xRot;
	float Zoom;
	float xTrans;
	float yTrans;
	float zTrans;

	vector<unsigned char> image;
	

	int whichvt;

	bool show_tex;
	bool show_map;
	bool show_feature;
	bool show_fmesh;
	bool tex;
	//////////////////////////////////////////////////////////////////////

// 對話方塊資料
	enum { IDD = IDD_CG_PROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadfile();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck5();
};
