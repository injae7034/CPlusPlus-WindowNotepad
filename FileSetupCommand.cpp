#include "FileSetupCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "PageSetUpDialog.h"
#include "PageSetUpInformation.h"

//생성자
FileSetupCommand::FileSetupCommand(NotepadForm* notepadForm)
{
    this->notepadForm = notepadForm;
}

//Execute
void FileSetupCommand::Execute()
{
    //1. pageSetUpDialog를 생성한다.
    PageSetupDialog pageSetupDialog(this->notepadForm);
    //2. pageSetUpDialog를 화면에 출력한다.
    LONG id;
    id = pageSetupDialog.DoModal();
    //3. OK버튼을 눌렀으면
    if (id == IDOK)
    {
        //3.1 페이지 설정 대화상자에서 devmode구조체를 구한다.
        DEVMODE* devmode = pageSetupDialog.GetDevMode();
        //3.2 페이지 설정 대화상자에서 hdc를 구한다.
        HDC hdc = pageSetupDialog.CreatePrinterDC();
        ASSERT(hdc);
        //3.2 hdc에서 cdc를 구한다.
        CDC* cdc = CDC::FromHandle(hdc);
        //3.3 프린트가 가능한 영역을 구한다.
        CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
        //3.4 페이지설정 대화상자에서 여백을 구한다.
        CRect marginRect;
        pageSetupDialog.GetMargins(&marginRect, NULL);
        //3.5 구한 여백을 페이지 화면 비율에 맞게 다시 설정한다.
        Long changedMarginRectLeft = marginRect.left * 600 / 2540;
        Long changedMarginRectTop = marginRect.top * 600 / 2540;
        Long changedMarginRectRight = marginRect.right * 600 / 2540;
        Long changedMarginRectBottom = marginRect.bottom * 600 / 2540;
        CRect changedMarginRect(changedMarginRectLeft, changedMarginRectTop,
            changedMarginRectRight, changedMarginRectBottom);
        //3.6 여백을 제외하고 프린트 가능한 영역을 구한다.
        CRect printableRect(changedMarginRect.left, changedMarginRect.top,
            rect.Width() - changedMarginRect.right, rect.Height() - changedMarginRect.bottom);
        //3.7 기존 페이지설정 정보가 있으면
        if (this->notepadForm->pageSetUpInformation != NULL)
        {
            //3.7.1 기존 페이지 설정 정보를 할당해제한다.
            delete this->notepadForm->pageSetUpInformation;
            this->notepadForm->pageSetUpInformation = 0;
        }
        //3.8 여백을 제외한 프린트 가능한 영역, 용지크기, 머릿글, 바닥글, 용지방향의 정보를 바탕으로
        //새로운 페이지 설정 정보를 만든다.
        this->notepadForm->pageSetUpInformation = new PageSetUpInformation(printableRect,
            devmode->dmPaperSize, pageSetupDialog.GetHeader(), 
            pageSetupDialog.GetFooter(), devmode->dmOrientation, devmode->dmDeviceName);
    }
}

//소멸자
FileSetupCommand::~FileSetupCommand()
{

}