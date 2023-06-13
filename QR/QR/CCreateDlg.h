#pragma once


// CCreateDlg 대화 상자

class CCreateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateDlg)

public:
	CCreateDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCreateDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BITMAPINFO* m_pBitmap;
	CEdit m_edit1;
	afx_msg void OnClickedButtonMake();
	int Bitmap(int w, int h, int bpp);
	CEdit m_edit3;
	afx_msg void OnBnClickedButton1();
};
