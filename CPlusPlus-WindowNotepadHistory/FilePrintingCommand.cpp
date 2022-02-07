#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "afxext.h"//CPrintInfo�������
#include "afxwin.h"//CView�������
#include "Glyph.h"
#include "PrintingVisitor.h"
#include "PrintInformation.h"

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
        //2.4 ����Ʈ�� ���� ������ ���� ������ �����Ѵ�.
        DOCINFO docinfo;
        //2.5 ����Ʈ�� ���� ������ ���� ������ �ʱ�ȭ�����ش�.
        ZeroMemory(&docinfo, sizeof(DOCINFO));
        //2.6 ����Ʈ�� ���� ������ �̸��� ���Ѵ�.
        docinfo.lpszDocName = "�޸���";
        //2.7 ����Ʈ�� �����Ѵ�.
        cdc->StartDoc(&docinfo);
        //2.8 PrintingVisitor�� �����Ѵ�.
        PrintingVisitor printingVisitor = PrintingVisitor(this->notepadForm, cdc, 0, 0);
        //2.9 ����Ʈ ������ �۲��� ������ �����ش�.
        LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
        printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
        CFont font;
        HFONT oldFont;
        font.CreateFontIndirect(&printLogFont);
        oldFont = (HFONT)cdc->SelectObject(font);
        //2.10 ����Ʈ������ ������ Ŭ������ �����Ѵ�.
        this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
            printLogFont, hdc);
        //2.11 ����Ʈ ����� �Ѵ�.
        this->notepadForm->printInformation->GetPrintNote()->Accept(&printingVisitor);
        //2.12 ����Ʈ�� ������.
        cdc->EndDoc();
    }
}

//�Ҹ���
FilePrintingCommand::~FilePrintingCommand()
{

}