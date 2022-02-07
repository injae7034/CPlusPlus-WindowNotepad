#include "FilePreviewCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "afxext.h"//CPrintInfo헤더파일
#include "afxwin.h"//CView헤더파일
#include "Glyph.h"
#include "PrintingVisitor.h"
#include "PrintInformation.h"
#include "PreviewForm.h"
#include "PageSetUpInformation.h"

//생성자
FilePreviewCommand::FilePreviewCommand(NotepadForm* notepadForm)
{
    this->notepadForm = notepadForm;
}

//Execute
void FilePreviewCommand::Execute()
{
	//1. 미리보기 폼을 띄우기 전에 기존의 미리보기 폼이 있으면
	if (this->notepadForm->previewForm != 0)
	{
		//1.1 미리보기 폼을 할당해제한다.
		this->notepadForm->previewForm->SendMessage(WM_CLOSE);//내용(미리보기폼)할당해제
		delete this->notepadForm->previewForm;//내용의 주소를 저장한 공간을 할당해제
		//1.2 댕글링 포인터를 0으로 안바꿔주면 할당해제가 됬는데 다시 할당해제를 하러 들어와서 에러가난다.
		this->notepadForm->previewForm = 0;
	}
	//2. 기존에 printInformation이 있으면
	if (this->notepadForm->printInformation != 0)
	{
		//2.1 printInformation을 할당해제한다.
		delete this->notepadForm->printInformation;
		this->notepadForm->printInformation = 0;
	}
	//3. CPrintDialog를 생성한다.
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_RETURNDEFAULT | PD_COLLATE
		| PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	//4. CPrintDialog를 실행시킨다.
	dlg.DoModal();
	//5. 프린트 다이얼로그에서 hdc를 구한다.
	HDC hdc = dlg.CreatePrinterDC();
	ASSERT(hdc);
	//6. 프린트 다이얼로그의 hdc에서 cdc를 구한다.
	CDC* cdc = CDC::FromHandle(hdc);
	//7. CPrintDialog의 정보를 가지고 있는 devmode구조체를 구한다.
	DEVMODE* devmode = dlg.GetDevMode();
	//8. 페이지 설정정보가 있으면
	if (this->notepadForm->pageSetUpInformation != 0)
	{
		//8.1 페이지 설정에서 설정한 용지방향 정보를 devmode에 저장한다.
		devmode->dmOrientation = this->notepadForm->pageSetUpInformation->GetOrientation();
		//8.2 페이지 설정에서 설정한 용지크기 정보를 devmode에 저장한다.
		devmode->dmPaperSize = this->notepadForm->pageSetUpInformation->GetPaperSize();
	}
	//9. 페이지 설정정보가 없으면
	else
	{
		//9.1 세로 방향을 디폴트로 설정한다.
		devmode->dmOrientation = 1;
		//9.2 A4용지 크기를 devmode에 저장한다.
		devmode->dmPaperSize = 9;
	}
	//10. devmode의 정보를 반영해서 cdc를 reset(update)한다.
	cdc->ResetDCA(devmode);
	//11. 프린트 비율로 글꼴의 비율을 맞춰준다.
	LOGFONT printLogFont = this->notepadForm->font.GetLogFont();
	printLogFont.lfHeight = -MulDiv(this->notepadForm->font.GetSize() / 10, 600, 72);
	//LOGFONT printLogFont = this->notepadForm->font.FindPrintingLogFont(cdc);
	CFont font;
	HFONT oldFont;
	font.CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)cdc->SelectObject(font);
	//12. 페이지 설정 정보가 있으면
	if (this->notepadForm->pageSetUpInformation != 0)
	{
		//12.1 페이지 설정에서 설정한 프린트가 가능한 크기를 반영해 프린트정보를 저장할 클래스를 생성한다.
		this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
			printLogFont, hdc, this->notepadForm->pageSetUpInformation->GetPrintableRect());
	}
	//13. 페이지 설정 정보가 없으면
	else
	{
		//13.1 기본적으로 프린트가 가능한 크기를 반영해 프린트정보를 저장할 클래스를 생성한다.
		this->notepadForm->printInformation = new PrintInformation(this->notepadForm,
			printLogFont, hdc,
			CRect(0, 0, cdc->GetDeviceCaps(HORZRES), cdc->GetDeviceCaps(VERTRES)));
	}
	//14. NotepadForm의 멤버인 previewForm에 미리보기 폼을 생성한다.
	this->notepadForm->previewForm = new PreviewForm(this->notepadForm);
	CRect rect(0, 0, 4958 / 3, 7016 / 9);//미리보기폼의 초기 생성시 출력되는 화면 고정
	this->notepadForm->previewForm->Create(NULL, "미리보기", WS_OVERLAPPEDWINDOW,
		rect, NULL, NULL, 0, NULL);
	this->notepadForm->previewForm->ShowWindow(SW_SHOW);
	this->notepadForm->previewForm->UpdateWindow();
}

//소멸자
FilePreviewCommand::~FilePreviewCommand()
{

}