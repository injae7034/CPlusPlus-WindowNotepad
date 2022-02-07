#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "Glyph.h"
#include "PrintingVisitor.h"
#include "PrintInformation.h"
#include "PageSetUpInformation.h"

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
        //2.4 프린트 다이얼로그에서 devmode를 구한다.
        DEVMODE* devmode = dlg.GetDevMode();
        //2.5 페이지 설정 정보가 있으면
        if (this->notepadForm->pageSetUpInformation != 0)
        {
            //2.5.1 페이지 설정에서 설정한 용지방향 정보를 devmode에 저장한다.
            devmode->dmOrientation = this->notepadForm->pageSetUpInformation->GetOrientation();
            //2.5.2 페이지 설정 정보에서 정한 용지크기 정보를 devmode에 저장한다.
            devmode->dmPaperSize = this->notepadForm->pageSetUpInformation->GetPaperSize();
        }
        //2.6 페이지 설정 정보가 없으면
        else
        {
            //2.6.1 세로 방향을 디폴트로 설정한다.
            devmode->dmOrientation = 1;
            //2.6.2 A4용지크기를 devmode에 저장한다.
            devmode->dmPaperSize = 9;
        }
        //2.7 devmode에 정보를 반영해서 cdc를 reste(upadate)해준다.
        cdc->ResetDCA(devmode);
        //2.8 프린트할 문서 정보를 담을 공간을 선언한다.
        DOCINFO docinfo;
        //2.9 프린트할 문서 정보를 담을 공간을 초기화시켜준다.
        ZeroMemory(&docinfo, sizeof(DOCINFO));
        //2.10 프린트할 문서 정보의 이름을 정한다.
        docinfo.lpszDocName = "메모장";
        //2.11 프린트를 시작한다.
        cdc->StartDoc(&docinfo);
        //2.12 PrintingVisitor를 생성한다.
        PrintingVisitor printingVisitor = PrintingVisitor(this->notepadForm, cdc, 0, 0);
        //2.13 프린트 비율로 글꼴의 비율을 맞춰준다.
        LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
        printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
        CFont font;
        HFONT oldFont;
        font.CreateFontIndirect(&printLogFont);
        oldFont = (HFONT)cdc->SelectObject(font);
        //2.14 페이지 설정 정보가 있으면
        if (this->notepadForm->pageSetUpInformation != 0)
        {
            //2.14.1 페이지 설정에서 설정한 프린트가 가능한 영역을 반영해 프린트정보를 저장할 클래스를 생성한다.
            this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
                printLogFont, hdc, this->notepadForm->pageSetUpInformation->GetPrintableRect());
        }
        //2.15 페이지 설정 정보가 없으면
        else
        {
            //2.15.1 기본적인 프린트가 가능한 영역을 반영해 프린트정보를 저장할 클래스를 생성한다.
            this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
                printLogFont, hdc,
                CRect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES)));
        }
        //2.16 프린트 출력을 한다.
        this->notepadForm->printInformation->GetPrintNote()->Accept(&printingVisitor);
        //2.17 프린트를 끝낸다.
        cdc->EndDoc();
    }
}

//소멸자
FilePrintingCommand::~FilePrintingCommand()
{

}