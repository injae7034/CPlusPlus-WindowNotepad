#include "Font.h"

//기본생성자
Font::Font()
	:logFont()
{
	this->color = RGB(0, 0, 0);
	this->size = 0;
}


//매개변수 2개 생성자
Font::Font(LOGFONT logFont, Long size, COLORREF color)
	:logFont(logFont)
{
	this->size = size;
	this->color = color;
}

//복사생성자
Font::Font(const Font& source)
	:logFont(source.logFont)
{
	this->size = source.size;
	this->color = source.color;
}

//치환연산자
Font& Font::operator=(const Font& source)
{
	this->logFont = source.logFont;
	this->size = source.size;
	this->color = source.color;

	return *this;
}

LOGFONT Font::FindPreviewLogFont(CDC* priviewDC)
{
    LOGFONT logFont;
    bool isSame = false;

    // 1. 공식에 의한 로그폰트를 구한다. -MulDiv(pointSize, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
    logFont = this->logFont;
    logFont.lfHeight = -MulDiv(this->size / 10, priviewDC->GetDeviceCaps(LOGPIXELSY), 72);

    // 2. 인쇄할 때 불일치하는지 확인한다.
    CRect rRect;
    CRect printRect(0, 0, priviewDC->GetDeviceCaps(HORZRES), priviewDC->GetDeviceCaps(VERTRES));

    priviewDC->SetMapMode(MM_ANISOTROPIC);
    priviewDC->SetWindowExt(12, 12);
    priviewDC->SetViewportExt(5, 5);

    HFONT hFont = CreateFontIndirect(&logFont);
    HFONT oldFont = (HFONT)priviewDC->SelectObject(hFont);

    CSize iSize = priviewDC->GetTextExtent("i");

    int iCount = printRect.Width() / iSize.cx;
    int compareICount = 0;

    char characterI[9999];
    int nHeight = iSize.cy;
    int i = 0;
    while (rRect.right < printRect.Width() - iSize.cy && nHeight == iSize.cy)
    { // 인쇄의 가로 길이 - 한 글자의 너비(딱 맞게 세면 한 글자를 더 세게 되므로 빼야됨)
        rRect.right = printRect.right;
        characterI[i] = 'i';
        characterI[i + 1] = '\0';
        i++;

        compareICount++;

        // 검사할 문자열, 문자 개수, 구할 Rect, Flag => 반환값 높이
        nHeight = priviewDC->DrawText(characterI, i, rRect, DT_CALCRECT);
    }
    if (compareICount == iCount)
    {
        isSame = true;
    }

    // 3. 불일치하면
    if (isSame != true)
    {
        // 3.1. 한 치수 작게 로그폰트를 구한다.
        logFont.lfHeight = -MulDiv(this->size / 10 - 1, priviewDC->GetDeviceCaps(LOGPIXELSY), 72);
    }

    // 4. 일치하면
    else
    {
        // 4.1. 한 치수 크게 로그폰트를 구한다.
        logFont.lfHeight = -MulDiv(this->size / 10 + 1, priviewDC->GetDeviceCaps(LOGPIXELSY), 72);
    }

    // 5. 일치하면 인쇄할 때 불일치하는지 확인한다.
    priviewDC->SelectObject(oldFont);
    DeleteObject(hFont);

    hFont = CreateFontIndirect(&logFont);
    oldFont = (HFONT)priviewDC->SelectObject(hFont);

    iSize = priviewDC->GetTextExtent("i");
    iCount = printRect.Width() / iSize.cx;
    rRect.right = 0;
    compareICount = 0;
    i = 0;
    while (rRect.right < printRect.Width() - iSize.cy && nHeight == iSize.cy && isSame == true)
    { // 인쇄의 가로 길이 - 한 글자의 너비(딱 맞게 세면 한 글자를 더 세게 되므로 빼야됨)
        rRect.right = printRect.right;
        characterI[i] = 'i';
        characterI[i + 1] = '\0';
        i++;

        compareICount++;

        // 검사할 문자열, 문자 개수, 구할 Rect, Flag => 반환값 높이
        nHeight = priviewDC->DrawText(characterI, i, rRect, DT_CALCRECT);
    }
    if (compareICount == iCount)
    {
        isSame = true;
    }

    // 6. 불일치하면
    if (isSame != true)
    {
        // 6.1. 한 치수 작게 로그폰트를 구한다.
        logFont.lfHeight = -MulDiv(this->size / 10 - 1, priviewDC->GetDeviceCaps(LOGPIXELSY), 72);
    }

    Long test = iSize.cx * iCount;

    priviewDC->SelectObject(oldFont);
    DeleteObject(hFont);

    return logFont;
}

LOGFONT Font::FindPrintingLogFont(CDC* printerDC)
{
    LOGFONT logFont;
    bool isSame = false;

    // 1. 공식에 의한 로그폰트를 구한다. -MulDiv(pointSize, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
    logFont = this->logFont;
    logFont.lfHeight = -MulDiv(this->size/10, printerDC->GetDeviceCaps(LOGPIXELSY), 72);

    // 2. 인쇄할 때 불일치하는지 확인한다.
    CRect rRect;
    CRect printRect(0, 0, printerDC->GetDeviceCaps(HORZRES), printerDC->GetDeviceCaps(VERTRES));

    HFONT hFont = CreateFontIndirect(&logFont);
    HFONT oldFont = (HFONT)printerDC->SelectObject(hFont);

    CSize iSize = printerDC->GetTextExtent("i");

    int iCount = printRect.Width() / iSize.cx;
    int compareICount = 0;

    char characterI[9999];
    int nHeight = iSize.cy;
    int i = 0;
    while (rRect.right < printRect.Width() - iSize.cy && nHeight == iSize.cy)
    { // 인쇄의 가로 길이 - 한 글자의 너비(딱 맞게 세면 한 글자를 더 세게 되므로 빼야됨)
        rRect.right = printRect.right;
        characterI[i] = 'i';
        characterI[i + 1] = '\0';
        i++;

        compareICount++;

        // 검사할 문자열, 문자 개수, 구할 Rect, Flag => 반환값 높이
        nHeight = printerDC->DrawText(characterI, i, rRect, DT_CALCRECT);
    }
    if (compareICount == iCount)
    {
        isSame = true;
    }

    // 3. 불일치하면
    if (isSame != true) 
    {
        // 3.1. 한 치수 작게 로그폰트를 구한다.
        logFont.lfHeight = -MulDiv(this->size / 10 - 1, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
    }

    // 4. 일치하면
    else
    {
        // 4.1. 한 치수 크게 로그폰트를 구한다.
        logFont.lfHeight = -MulDiv(this->size / 10 + 1, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
    }

    // 5. 일치하면 인쇄할 때 불일치하는지 확인한다.
    printerDC->SelectObject(oldFont);
    DeleteObject(hFont);

    hFont = CreateFontIndirect(&logFont);
    oldFont = (HFONT)printerDC->SelectObject(hFont);

    iSize = printerDC->GetTextExtent("i");
    iCount = printRect.Width() / iSize.cx;

    compareICount = 0;
    i = 0;
    while (rRect.right < printRect.Width() - iSize.cy && nHeight == iSize.cy && isSame == true)
    { // 인쇄의 가로 길이 - 한 글자의 너비(딱 맞게 세면 한 글자를 더 세게 되므로 빼야됨)
        rRect.right = printRect.right;
        characterI[i] = 'i';
        characterI[i + 1] = '\0';
        i++;

        compareICount++;

        // 검사할 문자열, 문자 개수, 구할 Rect, Flag => 반환값 높이
        nHeight = printerDC->DrawText(characterI, i, rRect, DT_CALCRECT);
    }
    if (compareICount == iCount)
    {
        isSame = true;
    }

    // 6. 불일치하면
    if (isSame != true)
    {
        // 6.1. 한 치수 작게 로그폰트를 구한다.
        logFont.lfHeight = -MulDiv(this->size / 10 - 1, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
    }

    printerDC->SelectObject(oldFont);
    DeleteObject(hFont);

    return logFont;
}

//소멸자
Font::~Font()
{

}