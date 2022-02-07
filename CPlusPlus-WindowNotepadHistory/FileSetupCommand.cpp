#include "FileSetupCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "PageSetUpDialog.h"
#include "PageSetUpInformation.h"

//������
FileSetupCommand::FileSetupCommand(NotepadForm* notepadForm)
{
    this->notepadForm = notepadForm;
}

//Execute
void FileSetupCommand::Execute()
{
    //1. pageSetUpDialog�� �����Ѵ�.
    PageSetupDialog pageSetupDialog(this->notepadForm);
    //2. pageSetUpDialog�� ȭ�鿡 ����Ѵ�.
    LONG id;
    id = pageSetupDialog.DoModal();
    //3. OK��ư�� ��������
    if (id == IDOK)
    {
        //3.1 ������ ���� ��ȭ���ڿ��� devmode����ü�� ���Ѵ�.
        DEVMODE* devmode = pageSetupDialog.GetDevMode();
        //3.2 ������ ���� ��ȭ���ڿ��� hdc�� ���Ѵ�.
        HDC hdc = pageSetupDialog.CreatePrinterDC();
        ASSERT(hdc);
        //3.2 hdc���� cdc�� ���Ѵ�.
        CDC* cdc = CDC::FromHandle(hdc);
        //3.3 ����Ʈ�� ������ ������ ���Ѵ�.
        CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
        //3.4 ���������� ��ȭ���ڿ��� ������ ���Ѵ�.
        CRect marginRect;
        pageSetupDialog.GetMargins(&marginRect, NULL);
        //3.5 ���� ������ ������ ȭ�� ������ �°� �ٽ� �����Ѵ�.
        Long changedMarginRectLeft = marginRect.left * 600 / 2540;
        Long changedMarginRectTop = marginRect.top * 600 / 2540;
        Long changedMarginRectRight = marginRect.right * 600 / 2540;
        Long changedMarginRectBottom = marginRect.bottom * 600 / 2540;
        CRect changedMarginRect(changedMarginRectLeft, changedMarginRectTop,
            changedMarginRectRight, changedMarginRectBottom);
        //3.6 ������ �����ϰ� ����Ʈ ������ ������ ���Ѵ�.
        CRect printableRect(changedMarginRect.left, changedMarginRect.top,
            rect.Width() - changedMarginRect.right, rect.Height() - changedMarginRect.bottom);
        //3.7 ���� ���������� ������ ������
        if (this->notepadForm->pageSetUpInformation != NULL)
        {
            //3.7.1 ���� ������ ���� ������ �Ҵ������Ѵ�.
            delete this->notepadForm->pageSetUpInformation;
            this->notepadForm->pageSetUpInformation = 0;
        }
        //3.8 ������ ������ ����Ʈ ������ ����, ����ũ��, �Ӹ���, �ٴڱ�, ���������� ������ ��������
        //���ο� ������ ���� ������ �����.
        this->notepadForm->pageSetUpInformation = new PageSetUpInformation(printableRect,
            devmode->dmPaperSize, pageSetupDialog.GetHeader(), 
            pageSetupDialog.GetFooter(), devmode->dmOrientation, devmode->dmDeviceName);
    }
}

//�Ҹ���
FileSetupCommand::~FileSetupCommand()
{

}