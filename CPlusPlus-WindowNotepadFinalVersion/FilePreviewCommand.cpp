#include "FilePreviewCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "afxext.h"//CPrintInfo�������
#include "afxwin.h"//CView�������
#include "Glyph.h"
#include "PrintingVisitor.h"
#include "PrintInformation.h"
#include "PreviewForm.h"
#include "PageSetUpInformation.h"

//������
FilePreviewCommand::FilePreviewCommand(NotepadForm* notepadForm)
{
    this->notepadForm = notepadForm;
}

//Execute
void FilePreviewCommand::Execute()
{
	//1. �̸����� ���� ���� ���� ������ �̸����� ���� ������
	if (this->notepadForm->previewForm != 0)
	{
		//1.1 �̸����� ���� �Ҵ������Ѵ�.
		this->notepadForm->previewForm->SendMessage(WM_CLOSE);//����(�̸�������)�Ҵ�����
		delete this->notepadForm->previewForm;//������ �ּҸ� ������ ������ �Ҵ�����
		//1.2 ��۸� �����͸� 0���� �ȹٲ��ָ� �Ҵ������� ��µ� �ٽ� �Ҵ������� �Ϸ� ���ͼ� ����������.
		this->notepadForm->previewForm = 0;
	}
	//2. ������ printInformation�� ������
	if (this->notepadForm->printInformation != 0)
	{
		//2.1 printInformation�� �Ҵ������Ѵ�.
		delete this->notepadForm->printInformation;
		this->notepadForm->printInformation = 0;
	}
	//3. CPrintDialog�� �����Ѵ�.
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_RETURNDEFAULT | PD_COLLATE
		| PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	//4. CPrintDialog�� �����Ų��.
	dlg.DoModal();
	//5. ����Ʈ ���̾�α׿��� hdc�� ���Ѵ�.
	HDC hdc = dlg.CreatePrinterDC();
	ASSERT(hdc);
	//6. ����Ʈ ���̾�α��� hdc���� cdc�� ���Ѵ�.
	CDC* cdc = CDC::FromHandle(hdc);
	//7. CPrintDialog�� ������ ������ �ִ� devmode����ü�� ���Ѵ�.
	DEVMODE* devmode = dlg.GetDevMode();
	//2.5 ������ ���� ������ ������
	if (this->notepadForm->pageSetUpInformation != 0)
	{
		//2.5.1 ������ �������� ������ �������� ������ devmode�� �����Ѵ�.
		devmode->dmOrientation = this->notepadForm->pageSetUpInformation->GetOrientation();
		//2.5.2 ����̽� �̸��� ������
		if (this->notepadForm->pageSetUpInformation->GetDevName().
			Compare((LPCTSTR)devmode->dmDeviceName) == 0)
		{
			//2.5.2.1 ������ ���� �������� ���� ����ũ�� ������ devmode�� �����Ѵ�.
			devmode->dmPaperSize = this->notepadForm->pageSetUpInformation->GetPaperSize();
		}
		//2.5.3 �̸��� �ٸ���
		else
		{
			//2.5.3.1 ������ ���������� ���ش�.
			delete this->notepadForm->pageSetUpInformation;
			this->notepadForm->pageSetUpInformation = 0;
		}
	}
	//9. ������ ���������� ������
	else
	{
		//9.1 ���� ������ ����Ʈ�� �����Ѵ�.
		devmode->dmOrientation = AfxGetApp()->GetProfileInt("NotepadSection",
			"PaperOrientation", 1);
		//9.3.1 A4���� ũ�⸦ devmode�� �����Ѵ�.
		devmode->dmPaperSize = AfxGetApp()->GetProfileInt("NotepadSection", "PaperSize",
			DMPAPER_A4);
	}
	//10. devmode�� ������ �ݿ��ؼ� cdc�� reset(update)�Ѵ�.
	cdc->ResetDCA(devmode);
	//11. ����Ʈ ������ �۲��� ������ �����ش�.
	LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
	printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)cdc->SelectObject(font);
	//12. ������ ���� ������ ������
	if (this->notepadForm->pageSetUpInformation != 0)
	{
		//12.1 ������ �������� ������ ����Ʈ�� ������ ũ�⸦ �ݿ��� ����Ʈ������ ������ Ŭ������ �����Ѵ�.
		this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
			printLogFont, hdc, this->notepadForm->pageSetUpInformation->GetPrintableRect());
	}
	//13. ������ ���� ������ ������
	else
	{
		//13.1 �⺻������ ����Ʈ�� ������ ũ�⸦ �ݿ��� ����Ʈ������ ������ Ŭ������ �����Ѵ�.
		this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
			printLogFont, hdc,
			CRect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES)));
	}
	//14. NotepadForm�� ����� previewForm�� �̸����� ���� �����Ѵ�.
	this->notepadForm->previewForm = new PreviewForm(this->notepadForm);
	CRect rect(0, 0, 4958 / 3, 7016 / 9);//�̸��������� �ʱ� ������ ��µǴ� ȭ�� ����
	this->notepadForm->previewForm->Create(NULL, "�̸�����", WS_OVERLAPPEDWINDOW,
		rect, NULL, NULL, 0, NULL);
	this->notepadForm->previewForm->ShowWindow(SW_SHOW);
	this->notepadForm->previewForm->UpdateWindow();
}

//�Ҹ���
FilePreviewCommand::~FilePreviewCommand()
{

}