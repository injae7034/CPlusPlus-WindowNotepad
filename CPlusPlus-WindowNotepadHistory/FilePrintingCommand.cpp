#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "afxext.h"//CPrintInfo헤더파일
#include "afxwin.h"//CView헤더파일
#include "Glyph.h"
#include "PrintingVisitor.h"
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
        //2.1 기존에 printInformation이 있으면
        if (this->notepadForm->printInformation != 0)
        {
            //2.1.1 기존에 있는 printInformation을 할당해제한다.
            delete this->notepadForm->printInformation;
        }
        //2.2 프린트 다이얼로그에서 hdc를 구한다.
        HDC hdc = dlg.CreatePrinterDC();
        ASSERT(hdc);
        //2.3 프린트 다이얼로그의 hdc에서 cdc를 구한다.
        CDC* cdc = CDC::FromHandle(hdc);
        //2.4 프린트할 문서 정보를 담을 공간을 선언한다.
        DOCINFO docinfo;
        //2.5 프린트할 문서 정보를 담을 공간을 초기화시켜준다.
        ZeroMemory(&docinfo, sizeof(DOCINFO));
        //2.6 프린트할 문서 정보의 이름을 정한다.
        docinfo.lpszDocName = "메모장";
        //2.7 프린트를 시작한다.
        cdc->StartDoc(&docinfo);
        //2.8 PrintingVisitor를 생성한다.
        PrintingVisitor printingVisitor = PrintingVisitor(this->notepadForm, cdc, 0, 0);
        //2.9 프린트 비율로 글꼴의 비율을 맞춰준다.
        LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
        printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
        CFont font;
        HFONT oldFont;
        font.CreateFontIndirect(&printLogFont);
        oldFont = (HFONT)cdc->SelectObject(font);
        //2.10 프린트정보를 저장할 클래스를 생성한다.
        this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
            printLogFont, hdc);
        //2.11 프린트 출력을 한다.
        this->notepadForm->printInformation->GetPrintNote()->Accept(&printingVisitor);
        //2.12 프린트를 끝낸다.
        cdc->EndDoc();
    }
}

//소멸자
FilePrintingCommand::~FilePrintingCommand()
{

}