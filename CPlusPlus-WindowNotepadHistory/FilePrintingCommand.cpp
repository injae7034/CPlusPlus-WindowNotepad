#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "afxext.h"//CPrintInfo�������
#include "afxwin.h"//CView�������
#include "Glyph.h"
#include "TextingOutVisitor.h"
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
        CFont font;
        HFONT oldFont;
        //2.1 ����Ʈ ���̾�α׿��� hdc�� ���Ѵ�.
        HDC hdc = dlg.CreatePrinterDC();
        ASSERT(hdc);
        //2.2 ����Ʈ ���̾�α��� hdc���� cdc�� ���Ѵ�.
        CDC* cdc = CDC::FromHandle(hdc);
        //2.3 ����Ʈ�� ���� ������ ���� ������ �����Ѵ�.
        DOCINFO docinfo;
        //2.4 ����Ʈ�� ���� ������ ���� ������ �ʱ�ȭ�����ش�.
        ZeroMemory(&docinfo, sizeof(DOCINFO));
        //2.5 ����Ʈ�� ���� ������ �̸��� ���Ѵ�.
        docinfo.lpszDocName = "�޸���";
        //2.6 ����Ʈ�� �����Ѵ�.
        cdc->StartDoc(&docinfo);
        //2.7 cdc�� ����Ʈ�������� ǥ���Ѵ�.
        cdc->m_bPrinting = 1;
        //2.8 ����Ʈ ���� textingOutVisitor�� �����Ѵ�.
        TextingOutVisitor textingOutVisitor = TextingOutVisitor(this->notepadForm, cdc, 0, 0);

        CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
        CDC dcTemp;
        dcTemp.CreateCompatibleDC(cdc);
        HBITMAP hbmp = ::CreateCompatibleBitmap(dcTemp, rect.right, rect.bottom);
        HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);
        dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

        //2.9 ����Ʈ ������ ������ �����ش�.
        LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
        printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
        font.CreateFontIndirect(&printLogFont);
        oldFont = (HFONT)dcTemp.SelectObject(font);
        //2.10 ����Ʈ������ ������ Ŭ������ �����Ѵ�.
        this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
            printLogFont, &dcTemp);
        //2.12 ����Ʈ ����� �Ѵ�.
        this->notepadForm->printInformation->GetPrintNote()->Accept(&textingOutVisitor);
        
        cdc->BitBlt(0, 0, rect.right, rect.bottom, &dcTemp, 0, 0, SRCCOPY);
        dcTemp.SelectObject(oldBMP);
        ::DeleteObject(hbmp);
        dcTemp.DeleteDC();

        //2.14 ����Ʈ�� ������.
        cdc->EndDoc();
    }
}

//�Ҹ���
FilePrintingCommand::~FilePrintingCommand()
{

}