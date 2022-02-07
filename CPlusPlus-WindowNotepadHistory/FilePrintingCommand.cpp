#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "Glyph.h"
#include "PrintingVisitor.h"
#include "PrintInformation.h"
#include "PageSetUpInformation.h"

//������
FilePrintingCommand::FilePrintingCommand(NotepadForm* notepadForm)
{
    this->notepadForm = notepadForm;
}

//Execute
void FilePrintingCommand::Execute()
{
    //1. ����Ʈ ���̾�α׸� �����Ѵ�.
    CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
    //2. ����Ʈ ���̾�α׿��� OK��ư�� ��������
    if (dlg.DoModal() == IDOK)
    {
        //2.1 ������ printInformation�� ������
        if (this->notepadForm->printInformation != 0)
        {
            //2.1.1 ������ �ִ� printInformation�� �Ҵ������Ѵ�.
            delete this->notepadForm->printInformation;
        }
        //2.2 ����Ʈ ���̾�α׿��� hdc�� ���Ѵ�.
        HDC hdc = dlg.CreatePrinterDC();
        ASSERT(hdc);
        //2.3 ����Ʈ ���̾�α��� hdc���� cdc�� ���Ѵ�.
        CDC* cdc = CDC::FromHandle(hdc);
        //2.4 ����Ʈ ���̾�α׿��� devmode�� ���Ѵ�.
        DEVMODE* devmode = dlg.GetDevMode();
        //2.5 ������ ���� ������ ������
        if (this->notepadForm->pageSetUpInformation != 0)
        {
            //2.5.1 ������ �������� ������ �������� ������ devmode�� �����Ѵ�.
            devmode->dmOrientation = this->notepadForm->pageSetUpInformation->GetOrientation();
            //2.5.2 ������ ���� �������� ���� ����ũ�� ������ devmode�� �����Ѵ�.
            devmode->dmPaperSize = this->notepadForm->pageSetUpInformation->GetPaperSize();
        }
        //2.6 ������ ���� ������ ������
        else
        {
            //2.6.1 ���� ������ ����Ʈ�� �����Ѵ�.
            devmode->dmOrientation = 1;
            //2.6.2 A4����ũ�⸦ devmode�� �����Ѵ�.
            devmode->dmPaperSize = 9;
        }
        //2.7 devmode�� ������ �ݿ��ؼ� cdc�� reste(upadate)���ش�.
        cdc->ResetDCA(devmode);
        //2.8 ����Ʈ�� ���� ������ ���� ������ �����Ѵ�.
        DOCINFO docinfo;
        //2.9 ����Ʈ�� ���� ������ ���� ������ �ʱ�ȭ�����ش�.
        ZeroMemory(&docinfo, sizeof(DOCINFO));
        //2.10 ����Ʈ�� ���� ������ �̸��� ���Ѵ�.
        docinfo.lpszDocName = "�޸���";
        //2.11 ����Ʈ�� �����Ѵ�.
        cdc->StartDoc(&docinfo);
        //2.12 PrintingVisitor�� �����Ѵ�.
        PrintingVisitor printingVisitor = PrintingVisitor(this->notepadForm, cdc, 0, 0);
        //2.13 ����Ʈ ������ �۲��� ������ �����ش�.
        LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
        printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
        CFont font;
        HFONT oldFont;
        font.CreateFontIndirect(&printLogFont);
        oldFont = (HFONT)cdc->SelectObject(font);
        //2.14 ������ ���� ������ ������
        if (this->notepadForm->pageSetUpInformation != 0)
        {
            //2.14.1 ������ �������� ������ ����Ʈ�� ������ ������ �ݿ��� ����Ʈ������ ������ Ŭ������ �����Ѵ�.
            this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
                printLogFont, hdc, this->notepadForm->pageSetUpInformation->GetPrintableRect());
        }
        //2.15 ������ ���� ������ ������
        else
        {
            //2.15.1 �⺻���� ����Ʈ�� ������ ������ �ݿ��� ����Ʈ������ ������ Ŭ������ �����Ѵ�.
            this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
                printLogFont, hdc,
                CRect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES)));
        }
        //2.16 ����Ʈ ����� �Ѵ�.
        this->notepadForm->printInformation->GetPrintNote()->Accept(&printingVisitor);
        //2.17 ����Ʈ�� ������.
        cdc->EndDoc();
    }
}

//�Ҹ���
FilePrintingCommand::~FilePrintingCommand()
{

}