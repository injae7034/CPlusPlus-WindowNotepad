#include "FilePreviewCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "afxext.h"//CPrintInfo�������
#include "afxwin.h"//CView�������
#include "Glyph.h"
#include "PrintingVisitor.h"
#include "PrintInformation.h"
#include "PreviewForm.h"

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
	//7. ����Ʈ ������ �۲��� ������ �����ش�.
	LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
	printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)cdc->SelectObject(font);
	//8. ����Ʈ������ ������ Ŭ������ �����Ѵ�.
	this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
		printLogFont, hdc);
	//9. NotepadForm�� ����� previewForm�� �̸����� ���� �����Ѵ�.
	this->notepadForm->previewForm = new PreviewForm(this->notepadForm);
	CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES) / 3, cdc->GetDeviceCaps(VERTRES) / 9);
	this->notepadForm->previewForm->Create(NULL, "�̸�����", WS_OVERLAPPEDWINDOW,
		rect, NULL, NULL, 0, NULL);
	this->notepadForm->previewForm->ShowWindow(SW_SHOW);
	this->notepadForm->previewForm->UpdateWindow();
}

//�Ҹ���
FilePreviewCommand::~FilePreviewCommand()
{

}