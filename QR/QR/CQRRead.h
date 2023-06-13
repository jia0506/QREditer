#pragma once

#include "opencv2/opencv.hpp"
using namespace cv;
// CQRRead 대화 상자

class CQRRead : public CDialogEx
{
	DECLARE_DYNAMIC(CQRRead)

public:
	CQRRead(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CQRRead();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int Bitmap(int w, int h, int bpp);		// Bitmap으로 변환
	int Drawimage();		// 화면 출력
	int ConnectCamera();		// 카메라 연결 및 연결여부 확인	
private:
	HICON m_hIcon;
	BITMAPINFO* m_pBitmap;
	VideoCapture* capture;
	Mat m_image1;
	CEdit m_edit1;
};
