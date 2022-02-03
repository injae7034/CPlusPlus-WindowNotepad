#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandCreator.h"
#include "KeyActionCreator.h"
#include "Command.h"
#include "Glyph.h"
#include "CaretController.h"
#include "TextExtent.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog�������
#include "KeyAction.h"

#include "ScrollActionCreator.h"
#include "ScrollAction.h"
#include "ScrollController.h"
#include "HorizontalScroll.h"
#include "VerticalScroll.h"
#include "DummyRow.h"

HHOOK hSaveMessageBoxHook;//�������� ����

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_IME_COMPOSITION, OnComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnStartCompostion)
	//�ش����(IDM_FILE_OPEN ~ IDM_FONT_CHANGE)�� id���� Ŭ���ϸ� OnCommand�Լ�����
	ON_COMMAND_RANGE(IDM_FILE_OPEN, IDM_ROW_AUTOCHANGE, OnCommand)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//NotepadForm������
NotepadForm::NotepadForm()
	:CFrameWnd()//���⼭ �ݷ��ʱ�ȭ�� �����������ϸ� Font()�� Caret()�� �⺻�����ڰ� ȣ���
	//�ֳ��ϸ� NotepadForm�� ����� font�� caret�� ������ �ֱ⶧���� notepadForm�� �����Ǹ鼭
	//font�� caret�� �⺻�����ڰ� ȣ��Ǿ� ������. �׷��� ������ Font�� Caret��
	//�⺻������ Font()�� Caret()�� �ʿ���.
{
	this->note = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->current = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->IsComposing = false;//false�� �ʱ�ȭ��Ŵ
	this->IsDirty = false;//false�� �ʱ�ȭ��Ŵ
	this->fileName = "���� ����";
	this->filePath = "";
	//CFont���� ����ϰ��� �ϴ� ����ũ��� ����ü�� �ʱ�ȭ��Ŵ.
	//�⺻�����ڷ� ������ this->font�� �Ű����� 5�������ڷ� ġȯ(=)��Ŵ
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfHeight = 100;
	wsprintf(logFont.lfFaceName, "�ü�ü");
	this->font = Font(logFont, RGB(0, 0, 0));
	this->textExtent = NULL;
}

//�޸��� �����찡 ������ ��
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//this->ModifyStyle(0, WS_OVERLAPPEDWINDOW, SWP_DRAWFRAME);
	//1. glyphCreator�� �����.
	GlyphCreator glyphCreator;
	//2. ��Ʈ�� �����.
	this->note = glyphCreator.Create((char*)"\0");
	//3. ���� �����.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//4. ���� �޸��忡 �߰��Ѵ�.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//5. ���� ���� ��ġ�� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	//6. CMenu�� notepadForm�� �����Ѵ�.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//7. textExtent�� ���� �Ҵ��Ѵ�.
	this->textExtent = new TextExtent(this);
	//8. ������ �޸����� ��Ʈ(����)�� �ҷ��´�.
	File file;
	string path = "test.txt";
	file.Load(this, path);
	//9. ó�� ��������� �޸��� �̸��� ���Ѵ�.
	string name = this->fileName;
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//10. ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
	rowIndex = this->note->First();
	//11. ���� ���� ��ġ�� �ٽ� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	//12. ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
	this->current->First();
	//13. scrollController�� �����Ѵ�.
	this->scrollController = new ScrollController(this);

	return 0;
}

//Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//2. glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long caretIndex;
	Long rowIndex;
	//3. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (nChar != '\n' && nChar != '\r')
	{
		//3.1 ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
		caretIndex = this->current->GetCurrent();
		//3.2 FileSaveCommand�� ���� ���� length�� ������
		if (caretIndex == this->current->GetLength())
		{
			//3.2.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
			caretIndex = this->current->Add(glyph);
		}
		//3.3 index�� ���� ���� length�� �ٸ���
		else
		{
			//3.3.1 ���� ���� index��°�� ���ο� ���ڸ� ���� ����.
			caretIndex = this->current->Add(caretIndex, glyph);
		}
		
	}
	//4. �Է¹��� ���ڰ� ���๮���̸�
	else if (nChar == '\n' || nChar == '\r')
	{
		//4.1 ���� ���� ��ġ�� ���Ѵ�.
		rowIndex = this->note->GetCurrent();
		//4.2 ���� ���� ĳ���� ��ġ�� ���Ѵ�.
		caretIndex = this->current->GetCurrent();
		//4.3. ���� �ٿ��� ���� ĳ�� ���� ��ġ�� �ִ� ���ڵ��� �����.
		glyph = this->current->Split(caretIndex);
		//4.4 rowIndex�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (rowIndex == this->note->GetLength() - 1)
		{
			//4.4.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			rowIndex = this->note->Add(glyph);
		}
		//4.5 �װ� �ƴϸ�
		else
		{
			//4.5.1 ���ο� ���� ���� ���� ���� ��ġ�� �����ִ´�.
			rowIndex = this->note->Add(rowIndex + 1, glyph);
		}
		//4.4 ���� ���� ��ġ�� ���� �����Ѵ�.
		this->current = this->note->GetAt(rowIndex);
		//4.5 ���� ���� ĳ���� ��ġ�� ó������ �̵���Ų��.
		this->current->First();
	}

	Long distance;
	//9. ĳ���� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->note->GetCurrent();
	//10. caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
	Long beginCaretYPos = this->textExtent->GetHeight() * (currentRowIndex);
	Long endCaretYPos = this->textExtent->GetHeight() * (currentRowIndex + 1);
	//11. ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
	Long VScrollPos = this->GetScrollPos(SB_VERT);
	//12. ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long VerticalSum = VScrollPos + this->
		scrollController->scroll[1]->GetPageSize();
	//13. ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
	//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
	if (beginCaretYPos < VScrollPos)
	{
		//13.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
		distance = beginCaretYPos;
		//13.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[1]->Move(distance);
		//13.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
		this->SetScrollPos(SB_VERT,
			this->scrollController->scroll[1]->GetCurrentPos());
	}
	//14. ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
	//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
	else if (endCaretYPos > VerticalSum)
	{
		//14.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
		//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
		distance = endCaretYPos - this->
			scrollController->scroll[1]->GetPageSize();
		//14.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[1]->Move(distance);
		//14.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_VERT,
			this->scrollController->scroll[1]->GetCurrentPos());
	}
	//15. ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
	Long caretXPos = this->textExtent->GetTextWidth(this->
		current->GetPartOfContent(this->current->GetCurrent()));
	//16 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
	Long HScrollPos = this->GetScrollPos(SB_HORZ);
	//17. ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long HorizontalSum = HScrollPos + this->
		scrollController->scroll[0]->GetPageSize();
	//18. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
		//�޸��忡�� �տ� ������
	if (caretXPos < HScrollPos)
	{
		//18.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->
			scrollController->scroll[0]->GetPageSize() / 5);
		//18.2 distance�� �����̸�
		if (distance < 0)
		{
			//18.2.1 distance�� 0���� �ٲ��ش�.
			distance = 0;
		}
		//18.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[0]->Move(distance);
		//18.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_HORZ,
			this->scrollController->scroll[0]->GetCurrentPos());
	}
	//19. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
	//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
	else if (caretXPos > HorizontalSum)
	{
		//19.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//19.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
		Long maxScrollPos = this->scrollController->scroll[0]->GetMax()
			- this->scrollController->scroll[0]->GetPageSize();
		//19.3 distance�� maxScrollPos���� ũ��
		if (distance > maxScrollPos)
		{
			//19.3.1 distance�� maxScrollPos�� �����Ѵ�.
			distance = maxScrollPos;
		}
		//19.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[0]->Move(distance);
		//19.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_HORZ,
			this->scrollController->scroll[0]->GetCurrentPos());
	}

	//5. ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
	this->Notify();
	//6. isComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//7. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//8. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//9. �����Ѵ�.
	Invalidate(TRUE);
}

//�޸��忡 �ؽ�Ʈ�� ����� ��
void NotepadForm::OnPaint()
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//2. �ؽ�Ʈ�� ����� �����ϰ���.
	//3. �ؽ�Ʈ�� ������ ����.
	dc.SetTextColor(this->font.GetColor());
	//4. ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//5. CFont�� �����Ѵ�.
	CFont font;
	//6. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreateFontIndirect(&this->font.GetLogFont());
	//7. ��Ʈ�� dc�� �����Ѵ�.
	HFONT oldFont;
	oldFont = (HFONT)dc.SelectObject(font);
	//8. TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//9. �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//10. note�� ����� ���ڵ��� ����Ѵ�.
	Long i = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;
	//11. �ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (i < this->note->GetLength())
	{
		//11.1 ���� ���� ���ڵ��� ���Ѵ�.
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		//11.2 ��ũ���� ��ġ�� ���Ѵ�.
		currentXPos = this->GetScrollPos(SB_HORZ);
		currentYPos = this->GetScrollPos(SB_VERT);
		//11.3 �ؽ�Ʈ ���� ��ġ���� ó������ (0,0)���� �����ϰ� �ι�°���� (0, ������ճ���)���� ������.
		//11.3 �ؽ�Ʈ ������ġ�� �����Ǿ�� ȭ�鸸 �̵��ϹǷ� �̵��Ѹ�ŭ �������!
		dc.TextOut(0 - currentXPos, i * text.tmHeight - currentYPos, content);
		//dc.TextOut(0, i * text.tmHeight, content);
		//this->Notify(); ���⼭ Notify ���ָ� �ȵ�! ĳ���� ��� ���Եǰ� �Ȼ����.
		i++;
	}
	dc.SelectObject(oldFont);
	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����. 
	font.DeleteObject();

}

//�ѱ��� �Է¹��� ��
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//1. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	//2. Ű����κ��� �Է¹��� ������ �������� �ѱ��� �����Ѵ�.
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//3. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long index = this->current->GetCurrent();
	//4. IsComposing���� '��'�̸�(�ѱ��� �������� �����̸�)
	if (this->IsComposing == true)
	{
		//4.1. ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
		//�׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
		this->current->Remove(index - 1);
		//4.2 ���ŵ� current�� ��ġ�� index�� �����Ѵ�.
		index = this->current->GetCurrent();

	}
	//5. ������ġ�� �ѱ��� ������ ������ �ѱ��� �������� �ƴ����� ���¸� �����Ѵ�.
	this->IsComposing = false;
	//6. �ѱ��� �ԷµǾ�����(�ѱ� �����߿� ���ڸ� �� ���������� '\0'���ڷ� OnComposition�� �Էµȴ�.)
	if (koreanLetter[0] != '\0')
	{
		//6.1 doubleByteLetter�� �����Ѵ�.
		Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
		//6.2 index�� ���� ���� length�� ������
		if (index == this->current->GetLength())
		{
			//6.2.1 ���� ���� ������ ���� �ڿ� ���ο� �ѱ��� �߰��Ѵ�.
			index = this->current->Add(doubleByteLetter);
		}
		//6.3. index�� ���� ���� length�� �ٸ���
		else
		{
			//6.3.1 ���� ���� index��°�� ���ο� �ѱ��� ���� ����.
			index = this->current->Add(index, doubleByteLetter);
		}
		//6.4 �ѱ��� ���� ��ġ�� �߰��߱⶧���� �ѱ��� �������� ���·� �����Ѵ�.
		this->IsComposing = true;
	}

	Long distance;
	//9. ĳ���� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->note->GetCurrent();
	//10. caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
	Long beginCaretYPos = this->textExtent->GetHeight() * (currentRowIndex);
	Long endCaretYPos = this->textExtent->GetHeight() * (currentRowIndex + 1);
	//11. ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
	Long VScrollPos = this->GetScrollPos(SB_VERT);
	//12. ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long VerticalSum = VScrollPos + this->
		scrollController->scroll[1]->GetPageSize();
	//13. ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
	//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
	if (beginCaretYPos < VScrollPos)
	{
		//13.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
		distance = beginCaretYPos;
		//13.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[1]->Move(distance);
		//13.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
		this->SetScrollPos(SB_VERT,
			this->scrollController->scroll[1]->GetCurrentPos());
	}
	//14. ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
	//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
	else if (endCaretYPos > VerticalSum)
	{
		//14.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
		//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
		distance = endCaretYPos - this->
			scrollController->scroll[1]->GetPageSize();
		//14.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[1]->Move(distance);
		//14.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_VERT,
			this->scrollController->scroll[1]->GetCurrentPos());
	}
	//15. ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
	Long caretXPos = this->textExtent->GetTextWidth(this->
		current->GetPartOfContent(this->current->GetCurrent()));
	//16 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
	Long HScrollPos = this->GetScrollPos(SB_HORZ);
	//17. ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long HorizontalSum = HScrollPos + this->
		scrollController->scroll[0]->GetPageSize();
	//18. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
		//�޸��忡�� �տ� ������
	if (caretXPos < HScrollPos)
	{
		//18.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->
			scrollController->scroll[0]->GetPageSize() / 5);
		//18.2 distance�� �����̸�
		if (distance < 0)
		{
			//18.2.1 distance�� 0���� �ٲ��ش�.
			distance = 0;
		}
		//18.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[0]->Move(distance);
		//18.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_HORZ,
			this->scrollController->scroll[0]->GetCurrentPos());
	}
	//19. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
	//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
	else if (caretXPos > HorizontalSum)
	{
		//19.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//19.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
		Long maxScrollPos = this->scrollController->scroll[0]->GetMax()
			- this->scrollController->scroll[0]->GetPageSize();
		//19.3 distance�� maxScrollPos���� ũ��
		if (distance > maxScrollPos)
		{
			//19.3.1 distance�� maxScrollPos�� �����Ѵ�.
			distance = maxScrollPos;
		}
		//19.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[0]->Move(distance);
		//19.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_HORZ,
			this->scrollController->scroll[0]->GetCurrentPos());
	}

	//7. ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
	this->Notify();
	//8. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//9. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//10. �����Ѵ�.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//�ϼ��� �ѱ��� �Է¹޾��� ��
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//1. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//2. doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//3. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long index = this->current->GetCurrent();
	//4. ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
	// �׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
	this->current->Remove(index - 1);
	//5. ������ �ִ� ���ڰ� �������� ������ ĳ���� ���� ���� ��ġ�� 1�����߱⶧����
	//ĳ���� ���� ���� ��ġ���� ��Ÿ���� index���� 1 ���ҽ����ش�.
	//index--;
	//���ŵ� current�� ��ġ�� index�� �����Ѵ�.
	index = this->current->GetCurrent();
	//5. index�� ���� ���� length�� ������
	if (index == this->current->GetLength())
	{
		//5.1 ���� ���� ������ ���� �ڿ� ���ο� �ѱ��� �߰��Ѵ�.
		index = this->current->Add(doubleByteLetter);
	}
	//6. index�� ���� ���� length�� �ٸ���
	else
	{
		//6.1 ���� ���� index��°�� ���ο� �ѱ��� ���� ����.
		index = this->current->Add(index, doubleByteLetter);
	}

	Long distance;
	//9. ĳ���� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->note->GetCurrent();
	//10. caret �̵� �Ŀ� ĳ���� ���� ����(���۰� ��)�� ���Ѵ�.(beginCaretYPos, endCaretYPos)
	Long beginCaretYPos = this->textExtent->GetHeight() * (currentRowIndex);
	Long endCaretYPos = this->textExtent->GetHeight() * (currentRowIndex + 1);
	//11. ������ũ���� �̵��ϱ� ���� ���� ������ũ���� VScrollPos(��ũ���� ���� ��ġ)�� ���Ѵ�.
	Long VScrollPos = this->GetScrollPos(SB_VERT);
	//12. ���ν�ũ���� ������ġ(VScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long VerticalSum = VScrollPos + this->
		scrollController->scroll[1]->GetPageSize();
	//13. ĳ���� ���� ��ġ ������(beginCaretYPos)�� 
	//������ũ���� ������ġ(VScrollPos)���� �޸��忡�� ���� ������
	if (beginCaretYPos < VScrollPos)
	{
		//13.1 ������ũ���� �̵��� ��ġ(distance)�� ĳ���� ���� ��ġ ���������� ���Ѵ�.
		distance = beginCaretYPos;
		//13.2 ������ũ�� ���� ��ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[1]->Move(distance);
		//13.3 ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
		this->SetScrollPos(SB_VERT,
			this->scrollController->scroll[1]->GetCurrentPos());
	}
	//14. ĳ���� ������ġ ������(endCaretYPos)�� ������ũ���� ���� ��ġ(VScrollPos)�� 
	//����ȭ���� ���α����� ��(VerticalSum)���� �޸��忡�� �Ʒ��� ������
	else if (endCaretYPos > VerticalSum)
	{
		//14.1 ������ũ���� �̵��� ����(distance)�� ���Ѵ�.
		//ĳ���� ���� ��ġ ������ �������� ȭ���� ���α����� ���� ���Ѵ�.
		distance = endCaretYPos - this->
			scrollController->scroll[1]->GetPageSize();
		//14.2 ������ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[1]->Move(distance);
		//14.3 ������ũ�ѹ��� Thumb�� ������ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_VERT,
			this->scrollController->scroll[1]->GetCurrentPos());
	}
	//15. ĳ���� �̵��� �Ŀ� ĳ���� ���� ������ġ(caretXPos)�� ���Ѵ�.
	Long caretXPos = this->textExtent->GetTextWidth(this->
		current->GetPartOfContent(this->current->GetCurrent()));
	//16 ����ũ���� �̵��ϱ����� ����ũ���� ������ġ(HScrollPos)�� ���Ѵ�.
	Long HScrollPos = this->GetScrollPos(SB_HORZ);
	//17. ����ũ���� ������ġ(HScrollPos)�� ����ȭ���� ���α����� ���� ���Ѵ�.
	Long HorizontalSum = HScrollPos + this->
		scrollController->scroll[0]->GetPageSize();
	//18. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)����
		//�޸��忡�� �տ� ������
	if (caretXPos < HScrollPos)
	{
		//18.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->
			scrollController->scroll[0]->GetPageSize() / 5);
		//18.2 distance�� �����̸�
		if (distance < 0)
		{
			//18.2.1 distance�� 0���� �ٲ��ش�.
			distance = 0;
		}
		//18.3 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[0]->Move(distance);
		//18.4 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_HORZ,
			this->scrollController->scroll[0]->GetCurrentPos());
	}
	//19. ĳ���� ���� ������ġ(caretXPos)�� ����ũ���� ������ġ(HScrollPos)��
	//����ȭ���� ���α����� ��(HorizontalSum)���� �޸��忡�� �ڿ� ������
	else if (caretXPos > HorizontalSum)
	{
		//19.1 ����ũ���� �̵��� ��ġ(distance)�� ���Ѵ�.
		distance = caretXPos - (this->
			scrollController->scroll[0]->GetPageSize() / 5 * 4);
		//19.2 ����ũ�ѹ��� Thumb�� �ִ� �̵���(maxScrollPos)�� ���Ѵ�.
		Long maxScrollPos = this->scrollController->scroll[0]->GetMax()
			- this->scrollController->scroll[0]->GetPageSize();
		//19.3 distance�� maxScrollPos���� ũ��
		if (distance > maxScrollPos)
		{
			//19.3.1 distance�� maxScrollPos�� �����Ѵ�.
			distance = maxScrollPos;
		}
		//19.4 ����ũ���� ������ġ�� distance�� �̵���Ų��.
		this->scrollController->scroll[0]->Move(distance);
		//19.5 ����ũ�ѹ��� Thumb�� ����ũ���� ���� ��ġ�� �̵���Ų��.
		this->SetScrollPos(SB_HORZ,
			this->scrollController->scroll[0]->GetCurrentPos());
	}

	//7. IsComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//8. ĳ���� ����Ǿ����� �˸���.
	this->Notify();
	//9. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//10. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//11. �����Ѵ�.
	Invalidate(TRUE);

	return 0;
}

//�ѱ� ���� ������ �˸�
LRESULT NotepadForm::OnStartCompostion(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//�޸����� Focus�� ���� ��
void NotepadForm::OnSetFocus(CWnd* pOldWnd)
{
	//1. ĳ�� �Ŵ����� �����Ѵ�.
	CaretController* caretController = new CaretController(this);
	//this->caretController = new CaretController(this);
	//2. ĳ���� ����Ǿ����� �������鿡�� �˸���.
	this->Notify();
}

//�޸����� Focus�� ���� ��
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//observer�ּҹ迭���� CaretManager�� ã�� ������ �ݺ��Ѵ�.
	Long i = 0;
	//1. ������ ����Ʈ���� �������� ���Ѵ�.
	Observer* observer = this->observers.GetAt(i);
	//2. i�� length���� ���� ���� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i <this->length && dynamic_cast<CaretController*>(observer) != observer)
	{
		//2.1 i�� ������Ų��.
		i++;
		//2.2 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->observers.GetAt(i);
	}
	//3. �������� CaretManager�̸�(i < this->length�� �־��ִ� ������ ĳ���� ���µ� 
	//ų��Ŀ���� �ϴ°�찡, ų��Ŀ���� ���� 2���ϴ� ��찡 ���� �� �ֱ⶧����, �׷� ���� ���� ������)
	if (i < this->length && dynamic_cast<CaretController*>(observer))
	{
		//3.1 ���� �Ҵ�� �������� ������ �Ҵ������Ѵ�.
		delete observer;//������ ���� �Ҵ�����
		//3.2 ����������Ʈ�� �� ������ ������ �Ҵ������� ������ �ּҸ� ������ �ִ�
		//����� �Ҵ������Ѵ�.
		this->observers.Delete(i);//������ ������ ������ �ִ� �ּ� �Ҵ�����
		//3.3 �迭���(�ּҸ� ����)�� �Ѱ� �Ҵ����������� �Ҵ緮�� ���ҽ�Ų��.
		this->capacity--;
		//3.4 ��뷮�� ���ҽ�Ų��.
		this->length--;
	}
}

//Command����
void NotepadForm::OnCommand(UINT nId)
{
	//1. CommandCreator�� �����Ѵ�.
	CommandCreator commandCreator(this);
	//2. ConcreteCommand�� �����Ѵ�.
	Command* command = commandCreator.Create(nId);
	//3. command�� NULL�� �ƴϸ�
	if (command != NULL)
	{
		//3.1 ConcreteCommand�� execute �Լ��� �����Ѵ�.
		command->Execute();
		//3.2 command�� �Ҵ������Ѵ�.
		delete command;
	}
}

//�޸��忡�� Ű����� �̵��ϱ�
void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. KeyActionCreator�� �����Ѵ�.
	KeyActionCreator keyActionCreator(this);
	//2. ConcreteKeyAction�� �����Ѵ�.
	KeyAction* keyAction = keyActionCreator.Create(nChar);
	//3. keyAction�� NULL�� �ƴϸ�
	if (keyAction != NULL)
	{
		//3.1 ConcreteKeyAction�� OnKeyDown �Լ��� �����Ѵ�.
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		//3.2 keyAction�� �Ҵ������Ѵ�.
		delete keyAction;
	}
	//4. ��������� ����������Ʈ���� �˸���.
	this->Notify();
	this->Invalidate();
}

//�޸��忡�� ���� ��ũ���� Ŭ���� ��
void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. scrollActionCreator�� �����Ѵ�.
	ScrollActionCreator scrollActionCreator(this);
	//2. concreteScrollAction�� �����Ѵ�.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//3. scrollAction�� NULL�� �ƴϸ�
	if (scrollAction != NULL)
	{
		//3.1 ConcreteScrollAction�� OnVScroll �Լ��� �����Ѵ�.
		scrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		//3.2 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//4. ��������� �������鿡�� �˸���.
	this->Notify();
	this->Invalidate();	
}

//�޸��忡�� ���� ��ũ���� Ŭ���� ��
void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. scrollActionCreator�� �����Ѵ�.
	ScrollActionCreator scrollActionCreator(this);
	//2. concreteScrollAction�� �����Ѵ�.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//3. scrollAction�� NULL�� �ƴϸ�
	if (scrollAction != NULL)
	{
		//3.1 ConcreteScrollAction�� OnHScroll �Լ��� �����Ѵ�.
		scrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		//3.2 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//4. ��������� �������鿡�� �˸���.
	this->Notify();
	this->Invalidate();
}

//�޸��忡�� ȭ���� ũ�Ⱑ ����� ��
void NotepadForm::OnSize(UINT nType, int cx, int cy)
{
	//1. ���� �޸����� â�� ����(�ּ�ȭ, �ִ�ȭ, ���� ũ��� ����)�� ���� ���̿� ���� ���̸� �Է¹޴´�.
	CFrameWnd::OnSize(nType, cx, cy);
	//2. ���� �޸����� ���°� �ּ�ȭ�� �ƴϸ�(�ּ�ȭ�̸� cx�� cy ���� �Ѵ� 0�� �ǰ�, 
	//cx�� 0�̸� �Ʒ����� cxũ��� �ݺ��� �����µ� ���ѹݺ��� �߻��ؼ� �ּ�ȭ��ư�� ������ ���̳�!) 
	if (nType != SIZE_MINIMIZED)
	{
		Long caretIndex = 0;
		Long rowTextWidth = 0;
		Glyph* glyph = 0;
		Long rowIndex = 0;
		Glyph* row = 0;
		Glyph* previousRow = 0;
		//2.1. �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//2.2 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//����ȭ���� ũ�⿡ ������ ������ �ڵ������� ����ϰ� �ϴ� �ٽ� �����·� ������.
			//DummyRow�� ������ �ٽ� Row�� �ű�� DummyRow�� ���� �Ҵ������Ѵ�.
			//2.2.1 Note�� �� ���� �������� �������� �ݺ��Ѵ�.
			while (rowIndex < this->note->GetLength())
			{
				//2.2.1.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
				row = this->note->GetAt(rowIndex);
				//2.2.1.2 DummyRow�̸�
				if (dynamic_cast<DummyRow*>(row))
				{
					//2.2.1.2.1 DummyRow ���� ��(Row)�� ���Ѵ�.
					previousRow = this->note->GetAt(rowIndex - 1);
					//2.2.1.2.2 DummyRow�� ���� ��(Row)�� ��ģ��.
					row->Join(previousRow);
					//2.2.1.2.3 Note���� DummyRow�� �ּҸ� �����.
					this->note->Remove(rowIndex);
				}
				//2.2.1.3 DummyRow�� �ƴϸ�
				else
				{
					//2.2.1.3.1 ���� �ٷ� �̵��Ѵ�.
					rowIndex++;
				}
			}
			//ȭ���� ũ�� ���濡 ���� �ٽ� �ڵ������� ���ش�.
			//2.2.2 rowIndex�� ����ġ��Ų��.
			rowIndex = 0;
			//2.2.3 Note�� �� ���� �������� �������� �ݺ��Ѵ�.
			while (rowIndex < this->note->GetLength())
			{
				//2.2.3.1 Note�� rowIndex��° ���� ���Ѵ�.
				row = this->note->GetAt(rowIndex);
				//2.2.3.2 caretIndex�� ����ġ��Ų��.
				caretIndex = 0;
				//2.2.3.3 caretIndex�� ������Ų��.
				caretIndex++;
				//2.2.3.4 rowIndex��° �ٿ��� caretIndex���� �ؽ�Ʈ�� ���α��̸� �����Ѵ�.
				rowTextWidth = this->textExtent->GetTextWidth
				(row->GetPartOfContent(caretIndex).c_str());
				//2.2.3.5 caretIndex�� rowIndex��° ���� �ѱ��� �������� �������� 
				//�׸��� rowIndex��° ���� ���α��̰� ����ȭ���� ���α���(cx)���� �������� �ݺ��Ѵ�.
				while (caretIndex < row->GetLength() && rowTextWidth <= cx)
				{
					//2.2.3.5.1 caretIndex�� ������Ų��.
					caretIndex++;
					//2.2.3.5.2 ������ caretIndex������ ���� ���̸� �����Ѵ�.
					rowTextWidth = this->textExtent->GetTextWidth
					(row->GetPartOfContent(caretIndex));
				}
				//2.2.3.6 rowIndex��° ���� ���� ���̰� ���� ȭ���� ���� ����(cx)���� ũ��
				if (rowTextWidth > cx)
				{
					//2.2.3.6.1 caretIndex������ ���̰� ����ȭ���� ���� ����(cx)���� ũ�� ������ 
					//�� ���ù��� ���Դ�. �׷��� ĳ���� �������� �� ĭ �̵��� �ؼ� ���̸� ���
					//����ȭ���� ���� ����(cx)���� �۴�. ĳ��(caretIndex)�� ���� ���ڸ� ���� ��ġ��
					//�ݿ��ϱ� ������ �׻� ���� ���ں��� ��ĭ �ռ� �ִ�
					//�׷��� caretIndex-1���� split�� �ؾ� ȭ���� �Ѵ� ���ڸ� ���� �ٷ� ���� �� �ִ�.
					caretIndex--;
					//2.2.3.6.2 rowIndex��° ���� ���� ���̰� ����ȭ���� ���� ���̺��� Ŀ�� ������
					//���ں��� rowIndex��° �ٿ��� caretIndex ���� ��ġ�� �ִ� ���ڵ��� ������.
					//(DummyRow����)
					glyph = row->Split(caretIndex, true);
					//2.2.3.6.3 ���ο� ���� rowIndex��° ���� ���� ��ġ�� �����ִ´�.
					rowIndex = this->note->Add(rowIndex + 1, glyph);
				}
				//2.2.3.7 caretIndex�� rowIndex��° ���� �ѱ��� �������� ũ�ų� ������
				else if (caretIndex >= row->GetLength())
				{
					//2.2.3.7.1 ���� �ٷ� �̵��Ѵ�.
					rowIndex++;
				}
			}
		}
		
	}
	this->Notify();
	this->Invalidate();
}

//�޸��忡�� �ݱ��ư�� Ŭ������ ��
void NotepadForm::OnClose()
{
	//1. file�� �����Ѵ�.
	File file;
	int messageBoxButton = IDCANCEL;
	//2. �޸��忡 ���� ������ ������
	if (this->IsDirty == true)
	{
		//2.1 �޼����ڽ��� �޼����� �����Ѵ�.
		string message = "���� ����";
		//2.2 ���ϰ�ΰ� ������ ������
		if (this->filePath != "")
		{
			//2.2.1 �޼����� �����Ѵ�.
			message = this->filePath;
		}
		//2.3 SaveBox �޼��� ������ �����.
		message.insert(0, "���� ������ ");
		message += "�� �����Ͻðڽ��ϱ�?";
		//2.4 Save�޼����ڽ��� ����Ѵ�.
		messageBoxButton = SaveMessageBox(this->GetSafeHwnd(), message.c_str(), "�޸���", MB_YESNOCANCEL);
		//2.5 Save�޼����ڽ����� Yes�� ����������
		if (messageBoxButton == IDYES)
		{
			//2.5.1 ��Ʈ�� �̹� �ִ� �����̸�
			if (this->filePath != "")
			{
				//2.5.1.1 �޸����� �����Ѵ�.
				file.Save(this, this->filePath);
				//2.5.1.2 �޸��忡 ��������� ������ �����Ѵ�.
				this->IsDirty = false;
				//2.5.1.3 �޸��� ������ �ٲ۴�.
				string name = this->fileName;
				name += " - �޸���";
				SetWindowText(CString(name.c_str()));
			}
			//2.5.2 ��Ʈ�� ó�� ������ �����̸�
			else
			{
				//2.5.2.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
				LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
				CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
				//2.5.2.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
				if (fileDialog.DoModal() == IDOK)
				{
					//2.5.2.2.1 ������ ������ �̸��� ���Ѵ�.
					this->fileName = string(fileDialog.GetFileTitle());
					//2.5.2.2.2 ������ ������ ��θ� ���Ѵ�.
					this->filePath = string(fileDialog.GetPathName());
				}
				//2.5.2.3 ������ �޸����� �����Ѵ�.
				file.Save(this, this->filePath);
				//2.5.2.4 �޸��忡 ��������� ������ �����Ѵ�.
				this->IsDirty = false;
				//2.5.2.5 �޸��� ������ �ٲ۴�.
				string name = this->fileName;
				name += " - �޸���";
				SetWindowText(CString(name.c_str()));
			}
		}
	}
	//3. �޼����ڽ����� CANCEL�� �������� �ʾҰų� ����� ������ ������
	if (messageBoxButton != IDCANCEL || this->IsDirty == false)
	{
		//3.1 �޸����� �����.
		if (this->note != NULL)
		{
			delete this->note;
			//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
			//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
			//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
			//delete this->current;
		}
		//3.2 TextExtent�� �Ҵ������Ѵ�.
		if (this->textExtent != NULL)
		{
			delete this->textExtent;
		}
		//NotepadForm�� Subject�� ����� �޾ұ� ������ NotepadForm�� �Ҹ�� ��
		//Subject�� �Ҹ��ڰ� ȣ��Ǹ� ScrollController�� �˾Ƽ� �Ҵ����������ش�.
		//ScrollController�� NotepadForm�� ������ �� ���� �ѹ� �Ҵ�ǰ� NotepadForm�� �Ҹ�� ��
		//���� �Ҹ�Ǳ� ������ ���� ScrollController�� �Ҵ����������� �ʿ䰡 ����.
		//CaretController���� ���� OnSetFoucs�ɶ����� �����ǰ� OnKillFoculs�ɶ����� �Ҵ������Ǵµ�
		//�� �� NotepadForm�� �Ҹ�����ʱ� ������ �ݵ�� OnKillFoucs���� ������ �Ҵ������� ������Ѵ�.
		/*
		//observer�ּҹ迭���� ScrollController�� ã�� ������ �ݺ��Ѵ�.
		Long i = this->length - 1;
		//1. ������ ����Ʈ���� �������� ���Ѵ�.
		Observer* observer = this->observers.GetAt(i);
		//2. i�� 0���� ũ�ų� ���� ���� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
		while (i >= 0 && dynamic_cast<ScrollController*>(observer) != 0)
		{
			//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
			observer = this->observers.GetAt(i);
			//2.2 i�� ���ҽ�Ų��.
			i--;
		}
		//3. �������� CaretManager�̸�
		if (dynamic_cast<ScrollController*>(observer))
		{
			//3.1 ���� �Ҵ�� �������� ������ �Ҵ������Ѵ�.
			delete observer;//������ ���� �Ҵ�����
			i++;//�ݺ������� i�� �ѹ� �� -1����� ������ �����·� ������ ���� +1�� ����.
			//3.2 ����������Ʈ�� �� ������ ������ �Ҵ������� ������ �ּҸ� ������ �ִ�
			//����� �Ҵ������Ѵ�.
			this->observers.Delete(i);//������ ������ ������ �ִ� �ּ� �Ҵ�����
			//3.3 �迭���(�ּҸ� ����)�� �Ѱ� �Ҵ����������� �Ҵ緮�� ���ҽ�Ų��.
			this->capacity--;
			//3.4 ��뷮�� ���ҽ�Ų��.
			this->length--;
		}
		*/
		//3.3 �޸����� �ݴ´�.
		CFrameWnd::OnClose();
	}
}

//Save�޼����ڽ�
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam) {
	HWND hChildWnd;

	CString msg;// = TEXT("");

	if (nCode == HCBT_ACTIVATE) {
		hChildWnd = (HWND)wParam;
		if (GetDlgItem(hChildWnd, IDYES) != NULL) {
			msg = "����(&S)";
			SetWindowText(GetDlgItem(hChildWnd, IDYES), msg);
		}

		if (GetDlgItem(hChildWnd, IDNO) != NULL) {
			msg = "���� �� ��(&N)";
			SetWindowText(GetDlgItem(hChildWnd, IDNO), msg);
		}
		UnhookWindowsHookEx(hSaveMessageBoxHook);
}

	return 0;
}

int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType) {
	hSaveMessageBoxHook = SetWindowsHookEx(WH_CBT, &SaveMessageBoxProc, 0, GetCurrentThreadId());

	return MessageBox(hWnd, lpText, lpCaption, nType);
}