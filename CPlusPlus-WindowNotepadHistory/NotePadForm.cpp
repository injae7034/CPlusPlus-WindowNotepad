#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//NotepadForm생성자
NotepadForm::NotepadForm()
	:CFrameWnd()
{
	this->note = NULL;//NULL로 초기화시킴.
}

//NotepadForm소멸자
NotepadForm::~NotepadForm()
{

}

//1. 윈도우가 생성될 때
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//1.1 glyphCreator를 만든다.
	GlyphCreator glyphCreator;
	//1.2 메모장을 만든다.
	this->note = glyphCreator.Create((char*)"\0");
	//1.3 첫번째 줄을 만든다.
	Glyph* row = glyphCreator.Create((char*)"\n");
	Long rowIndex;
	Long letterIndex;
	rowIndex = this->note->Add(row);
	//1.4 첫번째 줄에 'A'를 입력한다.
	Glyph* letter = glyphCreator.Create((char*)"A");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.5 첫번째 줄에 'p'를 입력한다.
	letter = glyphCreator.Create((char*)"p");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.6 첫번째 줄에 'p'를 입력한다.
	letter = glyphCreator.Create((char*)"p");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.7 첫번째 줄에 'l'를 입력한다.
	letter = glyphCreator.Create((char*)"l");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.8 첫번째 줄에 'e를 입력한다.
	letter = glyphCreator.Create((char*)"e");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.9 첫번째 줄에 "은"을 입력한다.
	letter = glyphCreator.Create((char*)"은");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.10 두번째 줄을 만든다.
	row = glyphCreator.Create((char*)"\n");
	rowIndex = this->note->Add(row);
	//1.11 두번째 줄에 "사"를 입력한다.
	letter = glyphCreator.Create((char*)"사");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.12 두번째 줄에 "과"를 입력한다.
	letter = glyphCreator.Create((char*)"과");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.13 두번째 줄에 "다"를 입력한다.
	letter = glyphCreator.Create((char*)"다");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.14 두번째 줄에 "."를 입력한다.
	letter = glyphCreator.Create((char*)".");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.15 세번째 줄을 만든다.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//1.16 세번째 줄에 "사"를 입력한다.
	letter = glyphCreator.Create((char*)"사");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.17 세번째 줄에 "과"를 입력한다.
	letter = glyphCreator.Create((char*)"과");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.18 세번째 줄에 "는"을 입력한다.
	letter = glyphCreator.Create((char*)"는");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.19 네번째 줄을 만든다.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//1.20 네번째 줄에 "맛"을 입력한다.
	letter = glyphCreator.Create((char*)"맛");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.21 네번째 줄에 "있"을 입력한다.
	letter = glyphCreator.Create((char*)"있");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.22 네번째 줄에 "다"을 입력한다.
	letter = glyphCreator.Create((char*)"다");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);
	//1.23 네번째 줄에 "."을 입력한다.
	letter = glyphCreator.Create((char*)".");
	letterIndex = this->note->GetAt(rowIndex)->Add(letter);

	return 0;
}

//2. 메모장에 텍스트를 출력할 떄
void NotepadForm::OnPaint()
{
	//2.1 CPaintDC를 생성한다.
	CPaintDC dc(this);
	//2.2 텍스트의 배경을 투명하게함.
	dc.SetBkMode(TRANSPARENT);
	//2.3 텍스트의 색깔을 정함.
	dc.SetTextColor(RGB(0, 0, 0));
	//2.4 왼쪽을 기준선으로 정함.
	dc.SetTextAlign(TA_LEFT);
	//2.5 CFont를 생성한다.
	CFont font;
	//2.6 글씨크기와 글씨체를 정하다.
	font.CreatePointFont(500, _T("나눔바른펜"));
	//2.7 폰트를 dc에 지정한다.
	dc.SelectObject(font);
	//2.8 note에 저장된 글자들을 출력한다.
	Long i = 0;
	CString content;
	//줄단위의 반복구조를 통해서 줄을 나눠서 줄개수만큼 출력하도록 함.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		dc.TextOut(0, i * 150, content);
		i++;
		
	}
	//노트를 그대로 출력하면 줄이 구분이 안되고 한줄에 내용이 다나옴.
	content = CString(this->note->GetContent().c_str());
	dc.TextOut(0, i * 150, content);

#if 0
	//2.8 Note에 저장된 Content를 불러온다.
	CString content = CString(this->note->GetContent().c_str());
	//2.9 Content를 출력한다.
	dc.TextOut(0, 0, _T(this->note->GetContent().c_str()));
#endif
}