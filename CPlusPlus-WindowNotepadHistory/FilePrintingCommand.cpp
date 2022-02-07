#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "afxext.h"//CPrintInfo헤더파일
#include "afxwin.h"//CView헤더파일
#include "Glyph.h"
#include "TextingOutVisitor.h"
#include "PrintInformation.h"

//생성자
FilePrintingCommand::FilePrintingCommand(NotepadForm* notepadForm)
{
    this->notepadForm = notepadForm;
}

//Execute
void FilePrintingCommand::Execute()
{
    //1. 프린트 다이얼로그를 생성한다.
    CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
    //2. 프린트 다이얼로그에서 OK버튼을 눌렀으면
    if (dlg.DoModal() == IDOK)
    {
        CFont font;
        HFONT oldFont;
        //2.1 프린트 다이얼로그에서 hdc응 구한다.
        HDC hdc = dlg.CreatePrinterDC();
        ASSERT(hdc);
        //2.2 프린트 다이얼로그의 hdc에서 cdc를 구한다.
        CDC* cdc = CDC::FromHandle(hdc);
        //2.3 프린트할 문서 정보를 담을 공간을 선언한다.
        DOCINFO docinfo;
        //2.4 프린트할 문서 정보를 담을 공간을 초기화시켜준다.
        ZeroMemory(&docinfo, sizeof(DOCINFO));
        //2.5 프린트할 문서 정보의 이름을 정한다.
        docinfo.lpszDocName = "메모장";
        //2.6 프린트를 시작한다.
        cdc->StartDoc(&docinfo);
        //2.7 cdc가 프린트전용임을 표시한다.
        cdc->m_bPrinting = 1;
        //2.8 프린트 전용 textingOutVisitor를 생성한다.
        TextingOutVisitor textingOutVisitor = TextingOutVisitor(this->notepadForm, cdc, 0, 0);

        CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
        CDC dcTemp;
        dcTemp.CreateCompatibleDC(cdc);
        HBITMAP hbmp = ::CreateCompatibleBitmap(dcTemp, rect.right, rect.bottom);
        HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);
        dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

        //2.9 프린트 비율로 비율을 맞춰준다.
        LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
        printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
        font.CreateFontIndirect(&printLogFont);
        oldFont = (HFONT)dcTemp.SelectObject(font);
        //2.10 프린트정보를 저장할 클래스를 생성한다.
        this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
            printLogFont, &dcTemp);
        //2.12 프린트 출력을 한다.
        this->notepadForm->printInformation->GetPrintNote()->Accept(&textingOutVisitor);
        
        cdc->BitBlt(0, 0, rect.right, rect.bottom, &dcTemp, 0, 0, SRCCOPY);
        dcTemp.SelectObject(oldBMP);
        ::DeleteObject(hbmp);
        dcTemp.DeleteDC();

        //2.14 프린트를 끝낸다.
        cdc->EndDoc();
    }
}

//소멸자
FilePrintingCommand::~FilePrintingCommand()
{

}