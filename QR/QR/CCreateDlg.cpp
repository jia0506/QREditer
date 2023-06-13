// CCreateDlg.cpp: 구현 파일
//

#include "pch.h"
#include "QR.h"
#include "CCreateDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "opencv2/opencv.hpp"

#ifdef _DEBUG
#pragma comment(lib, "opencv_world451d.lib")
#else
#pragma comment(lib, "opencv_world451.lib")
#endif

#include "qrcodegen.h"

#include "Windows.h"

using namespace std;
using namespace cv;
using qrcodegen::QrCode;
using namespace qrcodegen;

// CCreateDlg 대화 상자

IMPLEMENT_DYNAMIC(CCreateDlg, CDialogEx)

CCreateDlg::CCreateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CREATE, pParent)
{

}

CCreateDlg::~CCreateDlg()
{
}

void CCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
}


BEGIN_MESSAGE_MAP(CCreateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, &CCreateDlg::OnClickedButtonMake)
	ON_BN_CLICKED(IDC_BUTTON1, &CCreateDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCreateDlg 메시지 처리기

int CCreateDlg::Bitmap(int w, int h, int bpp)
{
	// todo: 여기에 구현 코드 추가.
	if (m_pBitmap != NULL)
	{
		delete[] m_pBitmap;
		m_pBitmap = NULL;
	}

	if (bpp == 8)
		m_pBitmap = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmap = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFO)];

	m_pBitmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmap->bmiHeader.biPlanes = 1;
	m_pBitmap->bmiHeader.biBitCount = bpp;
	m_pBitmap->bmiHeader.biCompression = BI_RGB;
	m_pBitmap->bmiHeader.biSizeImage = 0;
	m_pBitmap->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmap->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmap->bmiHeader.biClrUsed = 0;
	m_pBitmap->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmap->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmap->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmap->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmap->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmap->bmiHeader.biWidth = w;
	m_pBitmap->bmiHeader.biHeight = -h;
	return 0;
}

Mat MakeQrCodeMat(const QrCode& qr, int iBorder/* = 0*/)
{
	int iQrSize = qr.getSize();
	int iNewSize = iQrSize + iBorder * 2;
	Mat img(iNewSize, iNewSize, CV_8UC1, Scalar(255));
	for (int iQrPosX = 0; iQrPosX < iQrSize; iQrPosX++)
	{
		for (int iQrPosY = 0; iQrPosY < iQrSize; iQrPosY++)
		{
			if (qr.getModule(iQrPosX, iQrPosY))
			{
				int iNewX = iQrPosX + iBorder;
				int iNewY = iQrPosY + iBorder;
				img.at<uchar>(iNewY, iNewX) = 0;
			}
		}
	}

	return img;
}

void CCreateDlg::OnClickedButtonMake()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_edit1->SetLimitText(0x999);
	CString strText = _T(""); // edit에 입력되는 문자열을 저장하기 위한 변수 선언
	m_edit1.GetWindowText(strText);
	
	if (strText == _T(""))
	{
		AfxMessageBox(_T("QR로 변환할 주소를 입력하세요."), MB_ICONWARNING);
		return;
	}
	/*char* valueTemp; //메모리 동적할당이 필요. malloc? calloc?
	sprintf(valueTemp, "%S",strText); //S는 대문자 S라는점이 특이사항.
	*/
	const char* cText;
	cText = (CStringA)strText;
	//const char* cText = (char*)(LPCTSTR)strText;
	const QrCode qr = QrCode::encodeText(cText, QrCode::Ecc::LOW); // 텍스트, 오류복원 레벨
	Mat img = MakeQrCodeMat(qr, 2); // QrCode => Mat

	Mat img_resize;
	resize(img, img_resize, Size(300, 300), 0, 0, INTER_NEAREST); // 사이즈 변환
	
	CString strText2 = _T(""); // edit에 입력되는 문자열을 저장하기 위한 변수 선언
	m_edit3.GetWindowText(strText2);
	const char* cText2;
	cText2 = (CStringA)strText2;
	//cv::String strFile = strText2 + ".jpg";
	//imshow("img", img);
	imshow(cText2, img_resize);
	//imwrite(strFile, img_resize);
	waitKey();
	destroyAllWindows();
	//m_edit1->SetSel(0, -1);
	//m_edit1->Clear();
}


void CCreateDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_edit1->SetLimitText(0x999);
	CString strText = _T(""); // edit에 입력되는 문자열을 저장하기 위한 변수 선언
	m_edit1.GetWindowText(strText);

	if (strText == _T(""))
	{
		AfxMessageBox(_T("QR로 변환할 주소를 입력하세요."), MB_ICONWARNING);
		return;
	}
	/*char* valueTemp; //메모리 동적할당이 필요. malloc? calloc?
	sprintf(valueTemp, "%S",strText); //S는 대문자 S라는점이 특이사항.
	*/
	const char* cText;
	cText = (CStringA)strText;
	//const char* cText = (char*)(LPCTSTR)strText;
	const QrCode qr = QrCode::encodeText(cText, QrCode::Ecc::LOW); // 텍스트, 오류복원 레벨
	Mat img = MakeQrCodeMat(qr, 2); // QrCode => Mat

	Mat img_resize;
	resize(img, img_resize, Size(300, 300), 0, 0, INTER_NEAREST); // 사이즈 변환

	CString strText2 = _T(""); // edit에 입력되는 문자열을 저장하기 위한 변수 선언
	m_edit3.GetWindowText(strText2);
	const char* cText2;
	cText2 = (CStringA)strText2;
	cv::String strFile = strText2 + ".jpg";
	//imshow("img", img);
	imshow(cText2, img_resize);
	imwrite(strFile, img_resize);
	waitKey();
	destroyAllWindows();
	//m_edit1->SetSel(0, -1);
	//m_edit1->Clear();
}
