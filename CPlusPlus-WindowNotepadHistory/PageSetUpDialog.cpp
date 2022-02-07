#include "PageSetupDialog.h"
#include "NotepadForm.h"
#include "PreviewForm.h"
#include <dlgs.h>
#include <afxdisp.h>
#include "resource.h"
#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(PageSetupDialog, CPageSetupDialog)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

PageSetupDialog::PageSetupDialog(CWnd* pParentWnd)
	: CPageSetupDialog(PSD_ENABLEPAGESETUPTEMPLATE | PSD_MARGINS, pParentWnd)
{
	this->notepadForm = pParentWnd;
	this->m_psd.Flags |= PSD_ENABLEPAGESETUPTEMPLATE;
	this->m_psd.hInstance = AfxGetResourceHandle();
	this->m_psd.lpPageSetupTemplateName = MAKEINTRESOURCE(PageSetupDialog::IDD);
	//�迭�� ���� �Ҵ��ϰ� �ּҸ� ����� ������.
	this->header = new char[512];
	this->footer = new char[512];
}


BOOL PageSetupDialog::OnInitDialog() 
{
	CPageSetupDialog::OnInitDialog();

	return TRUE;
}

INT_PTR PageSetupDialog::DoModal()
{
	return CPageSetupDialog::DoModal();
}

//������ �������� OK��ư�� Ŭ������ ��
void PageSetupDialog::OnOK() 
{
	//1. �Ӹ��۰� �ٴڱ� ����Ʈ ��Ʈ�ѿ� ���� �ִ� ���ڸ� �д´�.
	CString header;
	CString footer;
	this->GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(header);
	this->GetDlgItem(IDC_EDIT_FOOTER)->GetWindowText(footer);
	strcpy(this->header, (LPCTSTR)header);
	strcpy(this->footer, (LPCTSTR)footer);
	//2. ������ ���� ��ȭ���ڿ��� devmode����ü�� ���Ѵ�.
	DEVMODE* devmode = this->GetDevMode();
	//3. ������ ���� ��ȭ���ڿ��� hdc�� ���Ѵ�.
	HDC hdc = this->CreatePrinterDC();
	//4. hdc���� cdc�� ���Ѵ�.
	CDC* cdc = CDC::FromHandle(hdc);
	//5. ����Ʈ�� ������ ������ ���Ѵ�.
	CRect rect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES));
	//6. ���������� ��ȭ���ڿ��� ������ ���Ѵ�.
	CRect marginRect;
	this->GetMargins(&marginRect, NULL);
	//7. ���� ������ ������ ȭ�� ������ �°� �ٽ� �����Ѵ�.
	Long changedMarginRectLeft = marginRect.left * 600 / 2540;
	Long changedMarginRectTop = marginRect.top * 600 / 2540;
	Long changedMarginRectRight = marginRect.right * 600 / 2540;
	Long changedMarginRectBottom = marginRect.bottom * 600 / 2540;
	CRect changedMarginRect(changedMarginRectLeft, changedMarginRectTop,
		changedMarginRectRight, changedMarginRectBottom);
	//8. ������ �����ϰ� ����Ʈ ������ ������ ���Ѵ�.
	CRect printableRect(changedMarginRect.left, changedMarginRect.top,
		rect.Width() - changedMarginRect.right, rect.Height() - changedMarginRect.bottom);
	//9. ���� ȭ���� ���� ���̸� ���Ѵ�.
	Long pageWidth = printableRect.right - printableRect.left;
	Long pageHeight = printableRect.bottom - printableRect.top;
	LOGFONT printLogFont;
	//10. PageSetUpDialog�� �θ� �����찡 NotepadForm�̸�
	if (dynamic_cast<NotepadForm*>(this->notepadForm))
	{
		//10.1 ����Ʈ ������ �۲��� ������ �����ش�.
		printLogFont = dynamic_cast<NotepadForm*>(this->notepadForm)->font.GetLogFont();
		printLogFont.lfHeight = -MulDiv(dynamic_cast<NotepadForm*>(this->notepadForm)->
			font.GetSize() / 10, 600, 72);
	}
	//11. PageSetUpDialog�� �θ� �����찡 PreviewForm�̸�
	else if (dynamic_cast<PreviewForm*>(this->notepadForm))
	{
		//11.1 ����Ʈ ������ �۲��� ������ �����ش�.
		printLogFont = dynamic_cast<PreviewForm*>(this->notepadForm)
			->notepadForm->font.GetLogFont();
		printLogFont.lfHeight = -MulDiv(dynamic_cast<PreviewForm*>(this->notepadForm)
			->notepadForm->font.GetSize() / 10, 600, 72);
	}
	
	
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)cdc->SelectObject(font);
	//11. �۲��� ���̸� ���� ���������� ���� ������ ����.
	TEXTMETRIC text;
	cdc->GetTextMetrics(&text);
	Long pageRowCount = pageHeight / text.tmHeight;
	//12. �Ӹ����� ������
	Long headerWidth = 0;
	if (strcmp(this->header, "") != 0)
	{
		//12.1 �� �������� ���� ������ ���ҽ�Ų��.
		pageRowCount--;
		//12.2 �Ӹ����� ���̸� ���Ѵ�.
		headerWidth = cdc->GetTextExtent(this->header).cx;
	}
	//13. �ٴڱ��� ������
	Long footerWidth = 0;
	if (strcmp(this->footer, "") != 0)
	{
		//13.1 �� �������� ���� ������ ���ҽ�Ų��.
		pageRowCount--;
		//13.2 �ٴڵ��� ���̸� ���Ѵ�.
		footerWidth = cdc->GetTextExtent(this->footer).cx;
	}
	//14. �ѱ����� �ִ� ���̸� ���Ѵ�.
	Long letterMaxWidth = text.tmMaxCharWidth;
	//13. ������ ������ ������ ���̰� �ѱ����� �ִ� ���̺��� ũ�� ������ ���� ������ 0���� ũ��,
	//������ ������ ������ ���̰� �Ӹ��۰� �ٴڱ��� ���̺��� ũ��
	if (pageWidth > letterMaxWidth && pageRowCount > 0
		&& pageWidth > headerWidth && pageWidth > footerWidth)
	{
		//���� �Ҵ��� ���ڹ迭�� �Ҵ���������.
		if (this->header != 0)
		{
			delete[] this->header;
		}
		if (this->footer != 0)
		{
			delete[] this->footer;
		}
		//13.1 ������ ���� ��ȭ���ڿ��� ������ ���� ������ �ݿ��ϰ� ������ ��ȭ���ڸ� �ݴ´�.
		CPageSetupDialog::OnOK();
	}
	//14. �׷��� ������
	else
	{
		//14.1 ������ �ٽ� �����϶�� �޼����ڽ��� ����Ѵ�.
		int messageBoxButton = MessageBox("���� �Ǵ� �Ӹ����̳� �ٴڱ��� ���� ũ�⸦ �Ѿ����ϴ�.\n"
			"���� ũ�� �Ǵ� �Ӹ����̳� �ٴڱ��� ���� ũ�⸦ ���� �ʰ� �ٽ� �Է��Ͻʽÿ�.", 
			"������ ����", MB_OK);
	}
}