// CQRRead.cpp: 구현 파일
//

#include "pch.h"
#include "QR.h"
#include "CQRRead.h"
#include "afxdialogex.h"

#include "CCreateDlg.h"
#include "QRDlg.h"
// CQRRead 대화 상자

IMPLEMENT_DYNAMIC(CQRRead, CDialogEx)

CQRRead::CQRRead(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_QR, pParent)
{

}

CQRRead::~CQRRead()
{
}

void CQRRead::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}


BEGIN_MESSAGE_MAP(CQRRead, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CQRRead::OnBnClickedOk)
END_MESSAGE_MAP()


// CQRRead 메시지 처리기


void CQRRead::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CQRRead::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ConnectCamera();
	//m_image = imread("apple.jpg", IMREAD_UNCHANGED);
	//Bitmap(m_image.cols, m_image.rows, m_image.channels() * 8);
	//Drawimage();
	CDialogEx::OnPaint();				   // 
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}


HCURSOR CQRRead::OnQueryDragIcon()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return static_cast<HCURSOR>(m_hIcon);
	//return CDialogEx::OnQueryDragIcon();
}

int CQRRead::Bitmap(int w, int h, int bpp)
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

int CQRRead::Drawimage()
{
	// TODO: 여기에 구현 코드 추가.
	CClientDC dc(GetDlgItem(IDC_CAMERA));
	CRect rect;

	GetDlgItem(IDC_CAMERA)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_image1.cols, m_image1.rows, m_image1.data, m_pBitmap, DIB_RGB_COLORS, SRCCOPY);

	ImageScanner scanner;

	// configure the reader
	//scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	Mat grey;
	CString a;
	cvtColor(m_image1, grey, COLOR_BGR2GRAY);
	uchar* raw = (uchar*)grey.data;
	int width = m_image1.cols;
	int height = m_image1.rows;

	// wrap image data
	Image image(width, height, "Y800", raw, width * height);

	// scan the image for barcodes
	int n = scanner.scan(image);
	dc.SetTextColor(RGB(255, 25, 2));
	a.Format(_T("인식중..."));
	// extract results
	for (Image::SymbolIterator symbol = image.symbol_begin();
		symbol != image.symbol_end();
		++symbol)
	{
		dc.SetTextColor(RGB(2, 25, 255));
		string str = symbol->get_data();
		m_edit1.SetWindowTextW(CString(str.c_str()));
		a.Format(_T("코드: %s"), CString(str.c_str()));
	}

	// clean up
	image.set_data(NULL, 0);
	dc.DrawText(a, -1, &rect, DT_LEFT | DT_WORDBREAK);


	return 0;
}

int CQRRead::ConnectCamera()
{
	// TODO: 여기에 구현 코드 추가.
	capture = new VideoCapture(0);

	if (!capture->isOpened())
	{
		printf("캠을 열수 없습니다. \n");
		return FALSE;
	}

	SetTimer(1000, 100, NULL);
	return 0;
}

void CQRRead::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!capture->isOpened())
	{
		return;
	}

	if (nIDEvent == 1000)
	{
		if (!capture->read(m_image1))
		{
			return;
		}

		Bitmap(m_image1.cols, m_image1.rows, m_image1.channels() * 8);
		Drawimage();
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CQRRead::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CQRRead::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
	CString edtStr;
	m_edit1.GetWindowTextW(edtStr);
	if (edtStr == _T(""))
	{
		AfxMessageBox(_T("QR이 인식되지 않았습니다."), MB_ICONWARNING);
		return;
	}
	ShellExecute(NULL, TEXT("open"), edtStr, NULL, NULL, SW_SHOW);
}
