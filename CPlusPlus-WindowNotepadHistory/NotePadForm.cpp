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
#include "PageMoveController.h"
#include "RowAutoChange.h"

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
	//this->IsOnScroll = false;//ó�������ɶ��� ��ũ���� �̿��� �̵��� ���� ������ false�� �ʱ�ȭ��.
	this->fileName = "���� ����";
	this->filePath = "";
	this->previousPageWidth = 0;
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
	//6. ���� ȭ���� ���� ���̸� �����Ѵ�.
	CRect rect;
	this->GetClientRect(&rect);
	this->previousPageWidth = rect.Width();
	//7. CMenu�� notepadForm�� �����Ѵ�.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//8. textExtent�� ���� �Ҵ��Ѵ�.
	this->textExtent = new TextExtent(this);
	//9. ������ �޸����� ��Ʈ(����)�� �ҷ��´�.
	File file;
	string path = "test.txt";
	file.Load(this, path);
	//10. ó�� ��������� �޸��� �̸��� ���Ѵ�.
	string name = this->fileName;
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//11. ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
	rowIndex = this->note->First();
	//12. ���� ���� ��ġ�� �ٽ� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	//13. ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
	Long letterIndex = this->current->First();
	//14. scrollController�� �����Ѵ�.
	this->scrollController = new ScrollController(this);
	//15. pageMoveController�� �����Ѵ�.
	this->pageMoveController = new PageMoveController(this);

	return 0;
}

//Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//�齺���̽��� �ƴϸ�(�齺����Ű�� OnKeyDown�� ���� �����ϰ� OnChar�� ���´�)
	//�齺���̽��ΰ�� OnChar���� �ƹ� ó���� �����ְ� �ٷ� ������ �ȴ�!
	if (nChar != VK_BACK)
	{
		//1. CtrlŰ�� ���������� �ȴ��������� ���¸� üũ�ؼ� �����Ѵ�.
		Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
		//2. CtrlŰ�� �ȴ����� ������
		if (ctrlPressedCheck >= 0)
		{
			//2.1 glyphCreator�� �����Ѵ�.
			GlyphCreator glyphCreator;
			//2.2 glyph�� �����Ѵ�.
			Glyph* glyph = glyphCreator.Create((char*)&nChar);
			Long letterIndex;
			Long rowIndex;
			//2.3 �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
			if (nChar != '\n' && nChar != '\r')
			{
				//2.3.1 ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
				letterIndex = this->current->GetCurrent();
				//2.3.2 FileSaveCommand�� ���� ���� length�� ������
				if (letterIndex == this->current->GetLength())
				{
					//2.3.2.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
					letterIndex = this->current->Add(glyph);
				}
				//2.3.3 index�� ���� ���� length�� �ٸ���
				else
				{
					//2.3.3.1 ���� ���� index��°�� ���ο� ���ڸ� ���� ����.
					letterIndex = this->current->Add(letterIndex, glyph);
				}

			}
			//2.4 �Է¹��� ���ڰ� ���๮���̸�
			else if (nChar == '\n' || nChar == '\r')
			{
				//2.4.1 ���� ���� ��ġ�� ���Ѵ�.
				rowIndex = this->note->GetCurrent();
				//2.4.2 ���� ���� ĳ���� ��ġ�� ���Ѵ�.
				letterIndex = this->current->GetCurrent();
				//2.4.3. ���� �ٿ��� ���� ĳ�� ���� ��ġ�� �ִ� ���ڵ��� �����.
				glyph = this->current->Split(letterIndex);
				//2.4.4 rowIndex�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
				if (rowIndex == this->note->GetLength() - 1)
				{
					//2.4.4.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
					rowIndex = this->note->Add(glyph);
				}
				//2.4.5 �װ� �ƴϸ�
				else
				{
					//2.4.5.1 ���ο� ���� ���� ���� ���� ��ġ�� �����ִ´�.
					rowIndex = this->note->Add(rowIndex + 1, glyph);
				}
				//2.4.4 ���� ���� ��ġ�� ���� �����Ѵ�.
				this->current = this->note->GetAt(rowIndex);
				//2.4.5 ���� ���� ĳ���� ��ġ�� ó������ �̵���Ų��.
				this->current->First();
				//2.4.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
				UINT state = this->GetMenu()->
					GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
				//2.4.7 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
				if (state == MF_CHECKED)
				{
					//2.4.7.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
					//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
					this->SendMessage(WM_SIZE);
				}
			}
			//2.5 ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
			this->Notify();
			//2.6 isComposing�� false�� �ٲ۴�.
			this->IsComposing = false;
			//2.7 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			SetWindowText(CString(name.c_str()));
			//2.8 �޸��忡 ��������� ������ �����Ѵ�.
			this->IsDirty = true;
			//2.9 �����Ѵ�.
			Invalidate(TRUE);
		}
	}
}

//�޸��忡 �ؽ�Ʈ�� ����� ��//��½� Visitor��������
void NotepadForm::OnPaint()
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//2. �ؽ�Ʈ�� ����� �����ϰ���.
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
	Long rowIndex = 0;
	Long currentXPos;
	Long currentYPos;
	CString content;

	Long currentWidth = 0;
	Long letterWidth = 0;
	Long letterCount = 0;
	Long letterIndex = 0;
	Glyph* row = 0;
	Glyph* letter = 0;

	//11. �ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (rowIndex < this->note->GetLength())
	{
		//11.1 ���� ���� ���Ѵ�.
		row = this->note->GetAt(rowIndex);
		//11.2 ��ũ���� ��ġ�� ���Ѵ�.
		currentXPos = this->GetScrollPos(SB_HORZ);
		currentYPos = this->GetScrollPos(SB_VERT);
		//11.3 �������� ù ���ڸ� ���Ѵ�.
		letterIndex = 0;
		currentWidth = 0;
		letterWidth = 0;
		//11.4 �������� ������ ���Ѵ�.
		letterCount = row->GetLength();
		//11.5 ������� ��ġ�� ���� ���ڰ������� �������� �ݺ��Ѵ�.
		while (letterIndex < letterCount)
		{
			//11.5.1 ���� ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//11.5.2 ���� ������ ������ ���Ѵ�.
			content = CString(letter->GetContent().c_str());
			//11.5.3 ������ �ʺ� ���Ѵ�.
			//ó���� 0���� �����ؾ��ϱ� ������ currentWidth += letterWidth�� 
			//letterWidth = this->textExtent->GetTextWidth((string)content) ���� �տ� �־����.
			//�׷��� ���� ���� �ʺ� ���ؼ� ���� ������ ���������� ����ָ� ��
			//ù������ �������� 0�̰� ���� ������ �������� ù������ �ʺ��̴�!!!
			currentWidth += letterWidth;
			letterWidth = this->textExtent->GetTextWidth((string)content);
			//���࿡ ���ڰ� �ǹ����̸� ������ ���� 8���� �ٲ��ش�.
			if (content == "\t")
			{
				content = "        ";
			}
			//11.5.3. ���� ���ڰ� ������ �ȵǾ�������
			if (letter->IsSelected() == false)
			{
				//11.5.3.1 ���ڸ� ȭ�鿡 ����Ѵ�.
				dc.SetBkColor(RGB(255, 255, 255));
				//�ؽ�Ʈ�� ������ ����. �̷����ؾ� ���߿� �۲û��ڿ��� ������ �ٲٸ� �ݿ��� �� ����.
				dc.SetTextColor(this->font.GetColor());
				//dc.SetTextColor(RGB(0, 0, 0));//�̷����ϸ� ���߿� �۲ÿ��� ���ڻ��� �ٲܼ�����
				dc.TextOut(currentWidth -currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			//11.5.4 ���� ���ڰ� ������ �Ǿ�������
			else
			{
				//11.5.4.1
				dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));//red, green, blue ���� ���� 
				dc.SetTextColor(RGB(255, 255, 255));//�̷����ϸ� ���߿� �۲ÿ��� ���ڻ��� �ٲܼ� ����
				dc.TextOut(currentWidth -currentXPos, rowIndex * text.tmHeight
					- currentYPos, content);
			}
			letterIndex++;
		}
		//11.3 �ؽ�Ʈ ������ġ�� �����Ǿ�� ȭ�鸸 �̵��ϹǷ� �̵��Ѹ�ŭ �������!
		//�׷� ���� ȭ���� ó�� �������� �����Ǿ� �ִµ� -�������ν� ȭ���� �����̴� ��ó�� ����.
		//dc.TextOut(0 - currentXPos, rowIndex * text.tmHeight - currentYPos, content);
		rowIndex++;
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
	//2. Ű����κ��� �Է¹��� ������ �������� ���ο� �ѱ��� �����Ѵ�.
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
		//���� �ѱ��� ������ ���� �Է� ���� �ѱ��� ��ü�� �� �ִ�.
		this->current->Remove(index - 1);
		//4.2 ���ŵ� current�� ��ġ�� index�� �����Ѵ�.
		index = this->current->GetCurrent();
	}
	//5. ������ġ�� �ѱ��� ������ ������ �ѱ��� �������� �ƴ����� ���¸� �����Ѵ�.
	this->IsComposing = false;
	//6. ���ο� �ѱ��� �ԷµǾ�����(�ѱ� �����߿� ���ڸ� �� ���������� '\0'���ڷ�
	//OnComposition�� �Էµȴ� �齺���̽�Ű�� �ԷµǸ� ���� �ѱ��� �������� '\0'�� ��� ���� �ȴ�.)
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
	//7. �ѱ� �����߿� �齺���̽�Ű�� ������ ���� ���� �ѱ��� ����������
	else
	{
		//BackSpace�� DeleteŰ�� ������ �ѱ������߿� ����� ��쵵 OnSize�� ������� �Ѵ�.
		//7.1 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//7.2 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//7.2.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->SendMessage(WM_SIZE);
		}
	}
	//8. ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
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
	this->caretController = new CaretController(this);
	//2. ĳ���� ����Ǿ����� �������鿡�� �˸���.
	this->Notify();
}

//�޸����� Focus�� ���� ��
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//1. CaretController�� ���������Ѵ�.(���⼭ �Ҵ������� ���̵�)
	Subject::Detach(this->caretController);
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
		//3.3 ��ȭ�� �޸��忡 �����Ѵ�.
		//if �����ȿ��� Notify�� ����� Ctrl�̳� Shift, Alt Capslock�� ���� Ư�����Ű�� ������ ��
		//Notify�� ȣ���� ĳ���� �ִ� ������ ��ũ���� �̵����� �ʴ´�. OnKeyDown�� Ű����Ű �� ���
		//Ű�� �������� ȣ��Ǳ� ������ ���ϴ� keyAction�� �ƴѰ�� Notify�� ������� �ʰ� �ؾ��Ѵ�!
		this->Notify();
		this->Invalidate();
	}
}

//�޸��忡�� ���� ��ũ���� Ŭ���� ��
void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. pageMoveController�� �Ҵ�Ǿ� ������
	if (this->pageMoveController != 0)
	{
		//1.1 pageMoveController�� ��������(�Ҵ�����)�Ѵ�.
		Subject::Detach(this->pageMoveController);
		//1.2 ��۸� �����͸� ���ֱ� ���� 0�� �����Ѵ�.
		this->pageMoveController = 0;
	}
	//2. scrollActionCreator�� �����Ѵ�.
	ScrollActionCreator scrollActionCreator(this);
	//3. concreteScrollAction�� �����Ѵ�.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//4. scrollAction�� NULL�� �ƴϸ�
	if (scrollAction != NULL)
	{
		//4.1 ConcreteScrollAction�� OnVScroll �Լ��� �����Ѵ�.
		scrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		//4.2 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//5. ��������� �������鿡�� �˸���.
	this->Notify();
	this->Invalidate();	
	//6. pageMoveController�� �Ҵ������Ǿ�������
	if (this->pageMoveController == 0)
	{
		//6.1 pageMoveController�� �ٽ� �Ҵ����ش�.
		this->pageMoveController = new PageMoveController(this);
	}
}

//�޸��忡�� ���� ��ũ���� Ŭ���� ��
void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//1. pageMoveController�� �Ҵ�Ǿ� ������
	if (this->pageMoveController != 0)
	{
		//1.1 pageMoveController�� ��������(�Ҵ�����)�Ѵ�.
		Subject::Detach(this->pageMoveController);
		//1.2 ��۸� �����͸� ���ֱ� ���� 0�� �����Ѵ�.
		this->pageMoveController = 0;
	}
	//2. scrollActionCreator�� �����Ѵ�.
	ScrollActionCreator scrollActionCreator(this);
	//3. concreteScrollAction�� �����Ѵ�.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//4. scrollAction�� NULL�� �ƴϸ�
	if (scrollAction != NULL)
	{
		//4.1 ConcreteScrollAction�� OnHScroll �Լ��� �����Ѵ�.
		scrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		//4.2 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//5. ��������� �������鿡�� �˸���.
	this->Notify();
	this->Invalidate();
	//6. pageMoveController�� �Ҵ������Ǿ�������
	if (this->pageMoveController == 0)
	{
		//6.1 pageMoveController�� �ٽ� �Ҵ����ش�.
		this->pageMoveController = new PageMoveController(this);
	}
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
		//2.1 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//2.2 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//2.2.1 RowAutoChange�� �����Ѵ�.(���� �Ҵ��ϸ� ���߿� ���� �Ҵ������� �������
			//�׷��� �ּҾ��� ���ÿ� �Ҵ��ϸ� �� �Լ� ������ ����Ǹ� �ڵ����� ���� �����.)
			//���⼭�� ���ÿ����� RowAutoChange�� ������ ���� ���Ѱ��̱� ������ ���ÿ� �Ҵ��ϴ°� ȿ������!
			RowAutoChange rowAutoChange(this);
			//2.2.2 �޸����� ���� ȭ�� ũ�Ⱑ �ٲ������
			if (this->previousPageWidth != cx)
			{
				//2.2.2.3 �ڵ����� ���� ���� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
				Long changedRowPos = this->note->GetCurrent();
				Long changedLetterPos = this->current->GetCurrent();
				Long originRowPos = 0;
				Long originLetterPos = 0;
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//2.2.2.4 �ڵ������� ����Ѵ�.
				rowAutoChange.UndoAllRows();
				//2.2.2.5 ȭ��ũ�� ���濡 ���� �ٽ� �ڵ������� ���ش�.
				rowAutoChange.DoAllRows();
				//2.2.2.6 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.2.2.7 ���� ���� ��ġ�� ������ ��ġ�� �����Ѵ�.
				this->note->Move(changedRowPos);
				this->current = this->note->
					GetAt(this->note->GetCurrent());
				this->current->Move(changedLetterPos);
				//2.2.2.8 ĳ���� ��ġ�� ����Ǿ����� �˸���.
				this->Notify();
				//2.2.2.9 ��������� �����Ѵ�.
				this->Invalidate(TRUE);
				//2.2.2.10 �޸����� ���� ȭ���� ���� ���̰� �ٲ���� ������ �̸� �������ش�.
				this->previousPageWidth = cx;
			}
			//2.2.3 �޸����� ���� ȭ�� ũ�Ⱑ �ٲ��� �ʾ�����
			else
			{
				//2.2.3.1 �ڵ����� ���� ���� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
				Long changedRowPos = this->note->GetCurrent();
				Long changedLetterPos = this->current->GetCurrent();
				Long originRowPos = 0;
				Long originLetterPos = 0;
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//2.2.3.2 �ڵ������� ����Ѵ�.
				rowAutoChange.UndoRow();
				//2.2.3.3 ȭ��ũ�� ���濡 ���� �ٽ� �ڵ������� ���ش�.
				rowAutoChange.DoRow();
				//2.2.3.4 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.2.3.5 ���� ���� ��ġ�� ������ ��ġ�� �����Ѵ�.
				this->note->Move(changedRowPos);
				this->current = this->note->
					GetAt(this->note->GetCurrent());
				this->current->Move(changedLetterPos);
				//2.2.3.6 ĳ���� ��ġ�� ����Ǿ����� �˸���.
				this->Notify();
				//2.2.3.7 ��������� �����Ѵ�.
				this->Invalidate(TRUE);
			}
		}
	}
	
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
		messageBoxButton = SaveMessageBox(this->GetSafeHwnd(), message.c_str(), 
			"�޸���", MB_YESNOCANCEL);
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

	if (nCode == HCBT_ACTIVATE)
	{
		hChildWnd = (HWND)wParam;
		if (GetDlgItem(hChildWnd, IDYES) != NULL)
		{
			msg = "����(&S)";
			SetWindowText(GetDlgItem(hChildWnd, IDYES), msg);
		}

		if (GetDlgItem(hChildWnd, IDNO) != NULL)
		{
			msg = "���� �� ��(&N)";
			SetWindowText(GetDlgItem(hChildWnd, IDNO), msg);
		}
		UnhookWindowsHookEx(hSaveMessageBoxHook);
	}
	return 0;
}

int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType)
{
	hSaveMessageBoxHook = SetWindowsHookEx(WH_CBT, &SaveMessageBoxProc, 0,
		GetCurrentThreadId());

	return MessageBox(hWnd, lpText, lpCaption, nType);
}