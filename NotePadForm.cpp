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

#include "PrintInformation.h"
#include "CommandHistory.h"
#include "TextingOutVisitor.h"
#include "SelectingTexts.h"
#include "PreviewForm.h"
#include "GlyphFinder.h"

//#include <htmlhelp.h>
//#pragma comment(lib, "htmlhelp")

HHOOK hSaveMessageBoxHook;//�������� ����

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()//������۸��� ���� ���� ��ġ
	ON_MESSAGE(WM_IME_COMPOSITION, OnComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnStartCompostion)
	ON_COMMAND_RANGE(IDM_FILE_OPEN, ID_ONCHARCOMMAND, OnCommand)
	ON_WM_MENUSELECT(IDR_MENU1, OnMenuSelect)
	//ON_REGISTERED_MESSAGE() //ã������ȭ���ڿ��� �θ�������� �޼����� �����ϱ� ���� �ʿ���
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//NotepadForm������
NotepadForm::NotepadForm()
	//���⼭ �ݷ��ʱ�ȭ�� �����������ϸ� Font()�� Caret()�� �⺻�����ڰ� ȣ���
	//�ֳ��ϸ� NotepadForm�� ����� font�� caret�� ������ �ֱ⶧���� notepadForm�� �����Ǹ鼭
	//font�� caret�� �⺻�����ڰ� ȣ��Ǿ� ������. �׷��� ������ Font�� Caret��
	//�⺻������ Font()�� Caret()�� �ʿ���.
{
	this->note = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->current = NULL;//NULL�� �ʱ�ȭ��Ŵ.
	this->isComposing = false;//false�� �ʱ�ȭ��Ŵ
	this->isDirty = false;//false�� �ʱ�ȭ��Ŵ
	this->isSelecting = false;//false�� �ʱ�ȭ��Ŵ
	this->isRowAutoChanging = false;//false�� �ʱ�ȭ ��Ŵ.
	this->isMouseLButtonClicked = false;//false�� �ʱ�ȭ ��Ŵ.
	this->fileName = "���� ����";
	this->filePath = "";
	this->previousPageWidth = 0;//ó�������ɶ��� ���� ȭ�� �ʺ� 0���� �ʱ�ȭ����
	this->selectedStartXPos = 0;//ó�������ɶ��� ���õ� texts�� ���� ������ 0���� �ʱ�ȭ����
	this->selectedStartYPos = 0;//ó�������ɶ��� ���õ� texts�� ���� ������ 0���� �ʱ�ȭ����
	this->glyph = 0;//�޸����� ó�� �����Ǹ� ���ڰ� ���� ������ 0���� �ʱ�ȭ����
	this->removedSelectedTexts = 0;//�޸����� ó�� �����Ǹ� ������ ���õ� ���ڰ� ���� ������ 0���� �ʱ�ȭ��
	this->textExtent = NULL;
	this->selectingTexts = NULL;
	this->printInformation = NULL;
	this->previewForm = NULL;
	this->pageSetUpInformation = NULL;
	this->caretController = 0;
}

//�޸��� �����찡 ������ ��
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);
	this->icon = AfxGetApp()->LoadIcon(IDI_ICON1); ///< ����� ������
	this->SetIcon(this->icon, ICON_SMALL);
	//HWND hwnd = (HWND)HtmlHelpA(GetDesktopWindow(), "notepad.chm", HH_DISPLAY_TOPIC, NULL);
	//HWND hwnd = (HWND)HtmlHelpA(this->GetSafeHwnd(), "notepad.chm", HH_DISPLAY_TOPIC, NULL);

	//1. glyphCreator�� �����.
	GlyphCreator glyphCreator;
	//2. Ŭ�����带 �����.
	this->clipboard = glyphCreator.Create((char*)"clipboard");
	//3. ��Ʈ�� �����.
	this->note = glyphCreator.Create((char*)"\0");
	//4. ���� �����.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//5. ���� �޸��忡 �߰��Ѵ�.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//6. ���� ���� ��ġ�� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	//7. ���� ȭ���� ���� ���̸� �����Ѵ�.
	CRect rect;
	this->GetClientRect(&rect);
	this->previousPageWidth = rect.Width();
	//8. CMenu�� notepadForm�� �����Ѵ�.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//���콺 ������ ��ư�� Ŭ������ �� �޴��� notepadForm�� �����Ѵ�.
	this->mouseRButtonMenu.LoadMenu(IDR_MENU3);
	//9. CMenu�� �޴����� �ڵ����� Enable�Ǵ� ���� ���� ���� FALSE ó���� �������
	//�ƴϸ� �ٸ� ������ Unenable���ѵ� ����ؼ� �ڵ����� Enable���ѹ���!
	this->m_bAutoMenuEnable = FALSE;
	//10. �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ��Ų��. ��Ȱ��ȭ�� ����Ʈ�� ���ÿ����� ����� Ȱ��ȭ������!
	this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//�������, �ٽý��� �޴��� ��Ȱ��ȭ ��Ų��.
	this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//���콺Ŭ�� �޴��� �����ϱ�, �߶󳻱�, ����, �������, �ٽ� ���� �޴��� ��Ȱ��ȭ��Ų��.
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//11. �ܺ�Ŭ�����忡 ���� ���ڿ��� ����Ǿ�������
	unsigned int priority_list = CF_TEXT;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//11.1 �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	}
	//12. �ܺ�Ŭ�����忡 ���� ���ڿ��� ����Ǿ� ����������
	else if (GetPriorityClipboardFormat(&priority_list, 1) == NULL)
	{
		//12.1 �ٿ��ֱ� �޴��� ��Ȱ��ȭ��Ų��.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//CFont���� ����ϰ��� �ϴ� ����ũ��� ����ü�� �ʱ�ȭ��Ŵ.
	//�⺻�����ڷ� ������ this->font�� �Ű����� 5�������ڷ� ġȯ(=)��Ŵ
	CFont font;
	CDC* dc = this->GetDC();
	Long size = AfxGetApp()->GetProfileInt("NotepadSection", "FontSize", 200);
	CString style = AfxGetApp()->GetProfileString("NotepadSection", "FontStyle", "���� ���");
	font.CreatePointFont(size, style, dc);
	LOGFONT logFont;
	font.GetLogFont(&logFont);
	this->font = Font(logFont, size, RGB(0, 0, 0));
	//13. textExtent�� ���� �Ҵ��Ѵ�.
	this->textExtent = new TextExtent(this);
	//14. ó�� ��������� �޸��� �̸��� ���Ѵ�.
	string name = this->fileName;
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//19. scrollController�� �����Ѵ�.
	this->scrollController = new ScrollController(this);
	//21. pageMoveController�� �����Ѵ�.
	this->pageMoveController = new PageMoveController(this);
	//22. selectingTexts�� �����Ѵ�.
	this->selectingTexts = new SelectingTexts(this);
	//23. CFindReplaceDialog�� �ʱ�ȭ���ش�.
	this->findReplaceDialog = 0;
	//24. CommandHistory�� �����Ѵ�.
	this->commandHistory = new CommandHistory(this);
	//25. �ڵ����࿩�θ� ���α׷��� ������ �����ϱ� ���ؼ� ������Ʈ���� NotepadSection������ �����
	//IsRowAutoChanging ���� ������.
	this->isRowAutoChanging = (bool)!AfxGetApp()->GetProfileInt(
		"NotepadSection", "IsRowAutoChanging", 1);
	//26. ������Ʈ���� NotepadSection���Ͽ� ����� IsRowAutoChanging ������ RowAutoChange��
	//�޼����� ���� �� ���� ������.
	if (this->isRowAutoChanging == true)
	{
		this->SendMessage(WM_COMMAND, IDM_ROW_AUTOCHANGE);
	}

	return 0;
}

//Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. CtrlŰ�� ���������� �ȴ��������� ���¸� üũ�ؼ� �����Ѵ�.
	Long ctrlPressedCheck = GetKeyState(VK_CONTROL);
	//�齺���̽��� �ƴϸ�(�齺����Ű�� OnKeyDown�� ���� �����ϰ� OnChar�� ���´�)
	//�齺���̽��ΰ�� OnChar���� �ƹ� ó���� �����ְ� �ٷ� ������ �ȴ�!
	//�ܾ������ �������� ���� ��, OnKeyDown�� SendMessage�� ���� ��, ���������� CtrlŰ�� ���� ä��
	//VK_BACK�� ���±� ������, ���⼭ OnCharCommand�� ������� �ʵ��� ���� ���ؼ�
	//ctrlPressedCheck & 0x8000 && nChar != VK_BACK ���ǹ��� �ʿ��ϴ�.
	if (nChar != VK_BACK && nChar != 27)
	{
		if (!(ctrlPressedCheck & 0x80))
		{
			//1.1.1 glyphCreator�� �����Ѵ�.
			GlyphCreator glyphCreator;
			//1.1.2 glyph�� �����ؼ� �����Ѵ�.
			this->glyph = glyphCreator.Create((char*)&nChar);
			//1.1.3 OnCommand�� �޼����� ������.
			this->SendMessage(WM_COMMAND, ID_ONCHARCOMMAND);
		}

	}
}

//�޸��忡 �ؽ�Ʈ�� ����� ��//��½� Visitor��������
void NotepadForm::OnPaint()
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);

	// ������۸��� ���� ���� ��ġ
	CRect rect;
	this->GetClientRect(&rect);
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	
	//2. �޸��忡 �ؽ�Ʈ�� ����� Ŭ������ �����Ѵ�.
	TextingOutVisitor textingOutVisitor = TextingOutVisitor(this, &dcTemp, 0, 0);
	//3. �޸��忡 �ؽ�Ʈ�� ����Ѵ�.
	this->note->Accept(&textingOutVisitor);
	
	// ������۸��� ���� ���� ��ġ
	dc.BitBlt(0, 0, rect.right, rect.bottom, &dcTemp, 0, 0, SRCCOPY);
	dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcTemp.DeleteDC();
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
	if (this->isComposing == true)
	{
		//4.1 ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
		//�׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
		//���� �ѱ��� ������ ���� �Է� ���� �ѱ��� ��ü�� �� �ִ�.
		this->current->Remove(index - 1);
		//4.2 ���ŵ� current�� ��ġ�� index�� �����Ѵ�.
		index = this->current->GetCurrent();
	}
	//5. ������ġ�� �ѱ��� ������ ������ �ѱ��� �������� �ƴ����� ���¸� �����Ѵ�.
	this->isComposing = false;
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
		//6.3 index�� ���� ���� length�� �ٸ���
		else
		{
			//6.3.1 ���� ���� index��°�� ���ο� �ѱ��� ���� ����.
			index = this->current->Add(index, doubleByteLetter);
		}
		//6.4 �ѱ��� ���� ��ġ�� �߰��߱⶧���� �ѱ��� �������� ���·� �����Ѵ�.
		this->isComposing = true;
		//BackSpace�� DeleteŰ�� ������ �ѱ������߿� ����� ��쵵 OnSize�� ������� �Ѵ�.
		//7.1 �ڵ� �� �ٲ��� �������̸�
		if (this->isRowAutoChanging == true)
		{
			//7.1.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->SendMessage(WM_SIZE);
		}
	}
	//7. �ѱ� �����߿� �齺���̽�Ű�� ������ ���� ���� �ѱ��� ����������
	else
	{
		//BackSpace�� DeleteŰ�� ������ �ѱ������߿� ����� ��쵵 OnSize�� ������� �Ѵ�.
		//7.1 �ڵ� �� �ٲ��� �������̸�
		if (this->isRowAutoChanging == true)
		{
			//7.1.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
	this->isDirty = true;
	//11. �����Ѵ�.
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//�ϼ��� �ѱ��� �Է¹޾��� ��
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//1. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//2. �Ű������� �Է¹��� wParam�� ���� �ѱ��� ���ۿ� �ű��.
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//3. doubleByteLetter�� �����Ѵ�.
	this->glyph = glyphCreator.Create((char*)koreanLetter);
	//4. OnCommand�� �޼����� ������.
	this->SendMessage(WM_COMMAND, ID_ONIMECHARCOMMAND);
	//5. ���� ���õ� ������ OnImeCharCommand�� ���±� ������ �ʱ�ȭ�Ѵ�.
	this->removedSelectedTexts = 0;

	return 0;
}

//�ѱ� ���� ������ �˸�
LRESULT NotepadForm::OnStartCompostion(WPARAM wParam, LPARAM lParam)
{
	//1. �޸��忡�� ���õ� texts�� ������
	if (this->isSelecting == true)
	{
		//1.1 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
		Long selectedStartRowPos = this->selectedStartYPos;//������ ���۵Ǵ� �� ��ġ
		Long selectedStartLetterPos = this->selectedStartXPos;//������ ���۵Ǵ� ���� ��ġ
		Long selectedEndRowPos = this->note->GetCurrent();//������ ������ �� ��ġ
		Long selectedEndLetterPos = this->current->GetCurrent();//������ ������ ���� ��ġ
		//1.2 content�� �����ϰ� �����.
		this->removedSelectedTexts = this->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//1.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->SetWindowText(CString(name.c_str()));
		//1.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->isDirty = true;
		//1.5 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
		//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		this->current = this->note->GetAt(this->note->GetCurrent());
		//1.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->isRowAutoChanging == true)
		{
			//1.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->SendMessage(WM_SIZE);
		}
		//1.7 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->isSelecting = false;
		//1.8 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->selectedStartXPos = 0;
		//1.9 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->selectedStartYPos = 0;
		//1.10 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

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
	this->caretController = 0;
}

//Command����
void NotepadForm::OnCommand(UINT nId)
{
	//1. CommandCreator�� �����Ѵ�.
	CommandCreator commandCreator(this);
	//2. ConcreteCommand�� �����Ѵ�.
	Command* command = commandCreator.Create(nId);
	bool isStop = false;
	//3. command�� NULL�� �ƴϸ�
	if (command != NULL)
	{
		//3.1 ConcreteCommand�� execute �Լ��� �����Ѵ�.
		command->Execute();
		//3.2 ���ڸ� �Է��ϴ� command�̸� 
		if (nId == ID_ONCHARCOMMAND || nId == ID_ONIMECHARCOMMAND)
		{
			//3.2.1 text�� �Է��ϴ� command�� ������ �ʴ´ٴ� ǥ�ø� �Ѵ�.
			isStop = false;
			//3.2.2 UndoList�� �߰��Ѵ�.
			this->commandHistory->PushUndoList(command, isStop);
			//3.2.3 ������Ҹ� Ȱ��ȭ ��Ų��.
			this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			//3.2.4 redoList�� �ʱ�ȭ��Ų��.
			this->commandHistory->MakeRedoListEmpty();
			//3.2.5 �ٽ� ������ ��Ȱ��ȭ��Ų��.
			this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//3.3 ���ڸ� ����� command�̰ų� �ٿ��ֱ� command�̸�
		else if (nId == ID_BACKSPACEKEYACTIONCOMMAND || nId == ID_DELETEKEYACTIONCOMMAND
			|| nId == ID_CTRLBACKSPACEKEYACTIONCOMMAND || nId == ID_CTRLDELETEKEYACTIONCOMMAND
			|| nId == ID_SHIFTCTRLBACKSPACEKEYACTIONCOMMAND ||
			nId == ID_SHIFTCTRLDELETEKEYACTIONCOMMAND || nId == IDM_NOTE_PASTE)
		{
			//3.3.1 ����ٴ� ǥ�ø� �Ѵ�.
			isStop = true;
			//3.3.2 Command�� ��������� ������
			if (command->IsDirty() == true)
			{
				//3.3.2.1 UndoList�� �߰��Ѵ�.
				this->commandHistory->PushUndoList(command, isStop);
				//3.3.2.2 ������Ҹ� Ȱ��ȭ ��Ų��.
				this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
				//3.3.2.3 redoList�� �ʱ�ȭ��Ų��.
				this->commandHistory->MakeRedoListEmpty();
				//3.3.2.4 �ٽ� ������ ��Ȱ��ȭ��Ų��.
				this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			//3.3.3 Command�� ��������� ������
			else
			{
				//3.3.3.1 command�� �Ҵ������Ѵ�.
				if (command != 0)
				{
					delete command;
				}
			}
		}
		//3.4 ���ÿ����� ����ų� ���ÿ����� �߶󳻰ų� �ٲٴ� command�̸�
		else if (nId == IDM_NOTE_REMOVE || nId == IDM_NOTE_CUT
			|| nId == ID_ONREPLACEBUTTONCLICKEDCOMMAND
			|| nId == ID_ONREPLACEALLBUTTONCLICKEDCOMMAND)
		{
			//3.4.1 ����ٴ� ǥ�ø� �Ѵ�.
			isStop = true;
			//3.4.2 UndoList�� �߰��Ѵ�.
			this->commandHistory->PushUndoList(command, isStop);
			//3.4.3 ������Ҹ� Ȱ��ȭ ��Ų��.
			this->menu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_ENABLED);
			//3.4.4 redoList�� �ʱ�ȭ��Ų��.
			this->commandHistory->MakeRedoListEmpty();
			//3.4.5 �ٽ� ������ ��Ȱ��ȭ��Ų��.
			this->menu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//3.5 ���ڸ� �Է��ϴ� command�� ���ڸ� ����� command�� �ƴϸ�
		//undoList�� �� command�� �ƴϸ� ���� �Ҵ������� ����� �޸� ������ �Ȼ����.
		//undoList�� �� command�� commandHistory�� �Ҹ�� ��, �Ҹ��ڿ��� ���� �Ҵ������ȴ�.
		else
		{
			//3.5.1 command�� �Ҵ������Ѵ�.
			if (command != 0)
			{
				delete command;
			}
		}
	}
	//4. ��ȭ�� �޸��忡 �����Ѵ�.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//�޸��忡�� Ű����� �̵��ϱ�
void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//3.1 KeyActionCreator�� �����Ѵ�.
	KeyActionCreator keyActionCreator(this);
	//3.2 ConcreteKeyAction�� �����Ѵ�.
	KeyAction* keyAction = keyActionCreator.Create(nChar);
	//3.3 keyAction�� NULL�� �ƴϸ�
	if (keyAction != NULL)
	{
		//3.3.1 ConcreteKeyAction�� OnKeyDown �Լ��� �����Ѵ�.
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		//3.3.2 keyAction�� �Ҵ������Ѵ�.
		delete keyAction;
		//3.3.3 �޸��忡 ������ �ȵǾ� ������
		if (this->isSelecting == false)
		{
			this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//3.3.4 �޸��忡 ������ �Ǿ� ������
		else
		{
			this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
			this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
			this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
			this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		}
		//3.3.5 ��ȭ�� �޸��忡 �����Ѵ�.
		//if �����ȿ��� Notify�� ����� Ctrl�̳� Shift, Alt Capslock�� ���� Ư�����Ű�� ������ ��
		//Notify�� ȣ���� ĳ���� �ִ� ������ ��ũ���� �̵����� �ʴ´�. OnKeyDown�� Ű����Ű �� ���
		//Ű�� �������� ȣ��Ǳ� ������ ���ϴ� keyAction�� �ƴѰ�� Notify�� ������� �ʰ� �ؾ��Ѵ�!
		this->Notify();
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
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
		//4.1 ����scroll�� ������ ���Ѵ�.
		SCROLLINFO scrollInfo;
		this->GetScrollInfo(SB_VERT, &scrollInfo);
		//4.2 ConcreteScrollAction�� OnVScroll �Լ��� �����Ѵ�.
		//���⼭ nPos�� �Ű������� ������ ���� Ŀ���� �� ������ ����� ThumbPositionScrollAction����
		//nPos�� �̿��ϸ� �̵��� �Ͼ�� �ʴ´�. scrollInfo.nTrackPos�� �̿��ؾ� ���� Ŀ����
		//����� �� �̵��� �Ͼ��.
		scrollAction->OnVScroll(nSBCode, scrollInfo.nTrackPos, pScrollBar);
		//4.3 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//5. ��������� �������鿡�� �˸���.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
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
		//4.1 ����scroll�� ������ ���Ѵ�.
		SCROLLINFO scrollInfo;
		this->GetScrollInfo(SB_HORZ, &scrollInfo);
		//4.2 ConcreteScrollAction�� OnHScroll �Լ��� �����Ѵ�.
		//���⼭ nPos�� �Ű������� ������ ���� Ŀ���� �� ������ ����� ThumbPositionScrollAction����
		//nPos�� �̿��ϸ� �̵��� �Ͼ�� �ʴ´�. scrollInfo.nTrackPos�� �̿��ؾ� ���� Ŀ����
		//����� �� �̵��� �Ͼ��.
		scrollAction->OnHScroll(nSBCode, scrollInfo.nTrackPos, pScrollBar);
		//4.3 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//5. ��������� �������鿡�� �˸���.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	//6. pageMoveController�� �Ҵ������Ǿ�������
	if (this->pageMoveController == 0)
	{
		//6.1 pageMoveController�� �ٽ� �Ҵ����ش�.
		this->pageMoveController = new PageMoveController(this);
	}
}

//�޸��忡�� ���콺 �ٷ� ��ũ�� �̵��� ��
BOOL NotepadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
	//3. ���� �Ʒ��� ����������
	UINT nSBCode;
	if (zDelta < 0)
	{
		nSBCode = 1;
	}
	//4. ���� ���� ����������
	else
	{
		nSBCode = 0;
	}
	//5. concreteScrollAction�� �����Ѵ�.
	ScrollAction* scrollAction = scrollActionCreator.Create(nSBCode);
	//6. scrollAction�� NULL�� �ƴϸ�
	if (scrollAction != NULL)
	{
		//6.1 ������ũ���� ������
		Long i = 0;
		if (GetStyle() & WS_VSCROLL)
		{
			//6.1.1 3�� �ݺ��Ѵ�.
			while (i < 3)
			{
				scrollAction->OnVScroll(nSBCode, 0, NULL);
				i++;
			}
		}
		//6.2 ���� ��ũ���� ���� ����ũ���� ������
		else if (GetStyle() & WS_HSCROLL)
		{
			//6.2.1 3�� �ݺ��Ѵ�.
			while (i < 3)
			{
				scrollAction->OnHScroll(nSBCode, 0, NULL);
				i++;
			}
		}
		//6.3 scrollAction�� �Ҵ������Ѵ�.
		delete scrollAction;
	}
	//7. ��������� �������鿡�� �˸���.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	//8. pageMoveController�� �Ҵ������Ǿ�������
	if (this->pageMoveController == 0)
	{
		//8.1 pageMoveController�� �ٽ� �Ҵ����ش�.
		this->pageMoveController = new PageMoveController(this);
	}

	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//���� ���콺 ��ư�� Ŭ������ ��
void NotepadForm::OnLButtonDown(UINT nFlags, CPoint point)
{
	//���콺 ���� ��ư�� Ŭ���ߴٰ� ǥ���Ѵ�.
	this->isMouseLButtonClicked = true;
	//1. shiftŰ�� ������ �ִ��� Ȯ���Ѵ�.
	Long shiftPressedCheck = GetKeyState(VK_SHIFT);
	//2. shiftŰ�� �ȴ�����, ������ �������̸�
	if (!(shiftPressedCheck & 0x80) && this->isSelecting == true)
	{
		//2.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->selectingTexts->Undo();
		//2.2 ������ ���� ���·� �ٲ۴�.
		this->isSelecting = false;
		//2.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->selectedStartXPos = 0;
		//2.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->selectedStartYPos = 0;
		this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//3. ���콺�� �̵��ϱ� ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->note->GetCurrent();
	Long previousLetterIndex = this->current->GetCurrent();
	//3. ������ũ���� ���� ��ġ�� ���Ѵ�.
	Long currentScrollYPos = this->GetScrollPos(SB_VERT);
	//4. ���콺�� Ŭ���� ���� y��ǥ�� ���Ѵ�.
	Long mouseClickYPos = point.y + currentScrollYPos;
	//5. ��Ʈ�� ù �ٺ��� ���̸� ���� ���콺�� Ŭ���� ���� y��ǥ���� �۰ų� ��������
	//�׸��� ������ ���� ��ġ���� �������� �ݺ��Ѵ�.
	Long currentRowIndex = 0;
	Long currentRowPos = currentRowIndex * this->textExtent->GetHeight()
		+ this->textExtent->GetHeight();
	Long lastRowIndex = this->note->GetLength() - 1;
	while (currentRowPos < mouseClickYPos && currentRowIndex < lastRowIndex)
	{
		//5.1 ���� ��ġ�� ������Ų��.
		currentRowIndex++;
		//5.2 ���� ���̸� ���Ѵ�.
		currentRowPos = currentRowIndex * this->textExtent->GetHeight()
			+ this->textExtent->GetHeight();
	}
	//6. ���� ��ġ�� �̵���Ų��.
	currentRowIndex = this->note->Move(currentRowIndex);
	this->current = this->note->GetAt(currentRowIndex);
	//7. ����ũ���� ���� ��ġ�� ���Ѵ�.
	Long currentScrollXPos = this->GetScrollPos(SB_HORZ);
	//8. ���콺�� Ŭ���� ���� x��ǥ�� ���Ѵ�.
	Long mouseClickXPos = point.x + currentScrollXPos;
	//9. ���� ù ���ں��� �ʺ� ���� ���콺�� Ŭ���� ���� x��ǥ���� �۰ų� ��������
	//�׸��� ������ġ�� ���� ���ڰ������� �������� �ݺ��Ѵ�.
	Long currentLetterIndex = 0;
	Long currentLetterPos = this->textExtent->GetTextWidth(this->current
		->GetPartOfContent(currentLetterIndex));
	while (currentLetterPos < mouseClickXPos && currentLetterIndex < this->current->GetLength())
	{
		//9.1 ���� ��ġ�� ������Ų��.
		currentLetterIndex++;
		//9.2 ���� �ʺ� ���Ѵ�.
		currentLetterPos = this->textExtent->GetTextWidth(this->current
			->GetPartOfContent(currentLetterIndex));
	}
	//10. ���� ���ڱ��� ���� �ؽ�Ʈ �ʺ� ���Ѵ�.
	Long previousLetterPos = this->textExtent->
		GetTextWidth(this->current->GetPartOfContent(currentLetterIndex - 1));
	//11. ���콺Ŭ�� x��ǥ���� ���� ���ڱ��� ���� �ؽ�Ʈ �ʺ��� ���� ���Ѵ�.
	Long previousDifference = mouseClickXPos - previousLetterPos;
	//12. ���� ���ڱ��� ���� �ؽ�Ʈ �ʺ񿡼� ���콺Ŭ�� x��ǥ�� ���� ���Ѵ�.
	Long currentDifference = currentLetterPos - mouseClickXPos;
	//13. ���� ���� ���� ������ �۰ų� ������
	if (previousDifference <= currentDifference)
	{
		//13.1 ���� ���� ��ġ�� �̵���Ų��.
		currentLetterIndex = this->current->Move(currentLetterIndex - 1);
	}
	//14. ���� ���� ���� ������ ũ��
	else
	{
		//14.1 ���� ��ġ�� �̵���Ų��.
		currentLetterIndex = this->current->Move(currentLetterIndex);
	}
	//15. ShiftŰ�� ���� ä�� ���� ���콺�� Ŭ��������
	if (shiftPressedCheck & 0x80)
	{
		//15.1 ���콺�� ���������� �̵�������
		if (previousRowIndex != currentRowIndex || previousLetterIndex != currentLetterIndex)
		{
			//15.1.1 ������ ���� ���� �ƴϸ�
			if (this->isSelecting == false)
			{
				//15.1.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
				this->isSelecting = true;
				//15.1.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->selectedStartXPos = previousLetterIndex;
				//15.1.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->selectedStartYPos = previousRowIndex;
				//15.1.1.4 ���ÿ����� ����� ������ �޴���ư���� �����ϱ�, �߶󳻱�, �����޴��� Ȱ��ȭ��Ų��.
				this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			}
			//15.1.2 ������ ���� ���̸�
			else
			{
				//15.1.2.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
				this->selectingTexts->Undo();
			}
			//15.1.3 ������ ���۵Ǵ� �ٺ��� ���� ���� ��ġ�� ��ũ��
			if (this->selectedStartYPos < currentRowIndex)
			{
				//15.1.3.1 �ؽ�Ʈ�� �������� �����Ѵ�.
				this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
					currentRowIndex, currentLetterIndex);
			}
			//15.1.4 ������ ���۵Ǵ� ���� ��ġ�� ���� ���� ��ġ�� ������ 
			else if (this->selectedStartYPos == currentRowIndex)
			{
				//15.1.4.1 ������ ���۵Ǵ� ���� ��ġ���� ���� ���� ��ġ�� ��ũ��
				if (this->selectedStartXPos < currentLetterIndex)
				{
					//15.1.4.1.1 �ؽ�Ʈ�� �������� �����Ѵ�.
					this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
						currentRowIndex, currentLetterIndex);
				}
				//15.1.4.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ���� ��ġ���� �� ũ��
				else
				{
					//15.1.4.2.1 �ؽ�Ʈ�� �������� �����Ѵ�.
					this->selectingTexts->DoPrevious(this->selectedStartYPos,
						this->selectedStartXPos, currentRowIndex, currentLetterIndex);
				}
			}
			//15.1.5 ������ ���۵Ǵ� ���� ��ġ�� ���� ���� ��ġ���� �� ũ��
			else if (this->selectedStartYPos > currentRowIndex)
			{
				//15.1.5.1 �ؽ�Ʈ�� �������� �����Ѵ�.
				this->selectingTexts->DoPrevious(this->selectedStartYPos,
					this->selectedStartXPos, currentRowIndex, currentLetterIndex);
			}
		}
	}

	this->SetCapture();

	//16. ��������� �������鿡�� �˸���.
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//���� ���콺 ��ư�� ���� Ŭ������ ��
void NotepadForm::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//1. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->note->GetCurrent();
	//2. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
	Long previousRowIndex = 0;
	Long previousLetterIndex = 0;
	//3. �ڵ������� ���� ���� �ƴϸ�
	if (this->isRowAutoChanging == false)
	{
		//3.1 �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
		previousRowIndex = currentRowIndex;
		//3.2 �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
		previousLetterIndex = currentLetterIndex;
		//3.3 ��Ʈ���� �ܾ������ �������� �̵��Ѵ�.
		currentLetterIndex = this->note->PreviousWord();
		//3.4 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->note->GetCurrent();
		//3.5 ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->current = this->note->GetAt(currentRowIndex);
	}
	//4. �ڵ������� ���� ���̸�
	else
	{
		//4.1 ��Ʈ���� �ڵ����� ���� ���� ���¿��� �ܾ������ ���������� �̵��Ѵ�.
		this->note->
			PreviousWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
		//4.2 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->note->GetCurrent();
		//4.3 �޸����� ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->current = this->note->GetAt(currentRowIndex);
	}
	//5. �ܾ������ ���������� �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	previousRowIndex = this->note->GetCurrent();
	//6. �ܾ������ ���������� �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
	previousLetterIndex = this->current->GetCurrent();
	//7. �ڵ������� ���� ���� �ƴϸ�
	currentRowIndex = previousRowIndex;
	currentLetterIndex = previousLetterIndex;
	if (this->isRowAutoChanging == false)
	{
		//7.1 ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
		currentLetterIndex = this->note->NextWord();
	}
	//8. �ڵ������� ���� ���̸�
	else
	{
		//8.1 ��Ʈ���� �ڵ������� ���� ���� ���¿��� �ܾ������ ���������� �̵��Ѵ�.
		this->note->NextWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
	}
	//9. �ܾ� ������ ���������� �̵��� �Ŀ� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
	currentRowIndex = this->note->GetCurrent();
	this->current = this->note->GetAt(currentRowIndex);
	currentLetterIndex = this->current->GetCurrent();
	//10. �ܾ������ �̵��� �Ŀ� ���� ��ġ�� ���� �ٸ���
	if (previousRowIndex < currentRowIndex)
	{
		//10.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
		this->isSelecting = true;
		//10.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->selectedStartXPos = previousLetterIndex;
		//10.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->selectedStartYPos = previousRowIndex;
	}
	//11. �ܾ������ �̵��� �Ŀ� ���� ��ġ�� ���� ������(�� �� ������ �ܾ���� �̵��� ������)
	else
	{
		//11.1 �ܾ� ������ �̵��� �־�����
		if (previousLetterIndex < currentLetterIndex)
		{
			//11.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->isSelecting = true;
			//11.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
			this->selectedStartXPos = previousLetterIndex;
			//11.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
			this->selectedStartYPos = previousRowIndex;
		}
	}
	//12. ������ ���� ���̸�
	if (this->isSelecting == true)
	{
		//12.1 �ؽ�Ʈ�� �����Ѵ�.
		this->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
		//12.2 ���ÿ����� ����� ������ �޴���ư���� �����ϱ�, �߶󳻱�, �����޴��� Ȱ��ȭ��Ų��.
		this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//12.3 ��������� �������鿡�� �˸���.
		this->Notify();
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}	
}

//������ ���콺 ��ư�� Ŭ������ ��
void NotepadForm::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu* MyMenu;
	CPoint pt;
	GetCursorPos(&pt);
	MyMenu = this->mouseRButtonMenu.GetSubMenu(0);
	MyMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}

//���콺�� ������ ��
void NotepadForm::OnMouseMove(UINT nFlags, CPoint point)
{
	//1. shiftŰ�� ������ �ִ��� Ȯ���Ѵ�.
	//Long shiftPressedCheck = GetKeyState(VK_SHIFT);
	//���콺 ���� ��ư�� ������ �ְų� �Ǵ� shiftŰ�� ���� ä�� ���콺 ���� ��ư�� ��������
	if (this->isMouseLButtonClicked == true
		|| nFlags == MK_SHIFT && this->isMouseLButtonClicked == true)
	{
		//1. ���콺�� �̵��ϱ� ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		Long previousRowIndex = this->note->GetCurrent();
		Long previousLetterIndex = this->current->GetCurrent();
		//2. ������ũ���� ���� ��ġ�� ���Ѵ�.
		Long currentScrollYPos = this->GetScrollPos(SB_VERT);
		//3. ���콺�� �巡���� ���� y��ǥ�� ���Ѵ�.
		Long mouseDragYPos = point.y + currentScrollYPos;
		//4. ��Ʈ�� ù �ٺ��� ���̸� ���� ���콺�� �巡���� ���� y��ǥ���� �۰ų� ��������
		//�׸��� ������ ���� ��ġ���� �������� �ݺ��Ѵ�.
		Long currentRowIndex = 0;
		Long currentRowPos = currentRowIndex * this->textExtent->GetHeight()
			+ this->textExtent->GetHeight();
		Long lastRowIndex = this->note->GetLength() - 1;
		while (currentRowPos < mouseDragYPos && currentRowIndex < lastRowIndex)
		{
			//4.1 ���� ��ġ�� ������Ų��.
			currentRowIndex++;
			//4.2 ���� ���̸� ���Ѵ�.
			currentRowPos = currentRowIndex * this->textExtent->GetHeight()
				+ this->textExtent->GetHeight();
		}
		//5. ���� ��ġ�� �̵���Ų��.
		currentRowIndex = this->note->Move(currentRowIndex);
		this->current = this->note->GetAt(currentRowIndex);
		//6. ����ũ���� ���� ��ġ�� ���Ѵ�.
		Long currentScrollXPos = this->GetScrollPos(SB_HORZ);
		//7. ���콺�� �巡���� ���� x��ǥ�� ���Ѵ�.
		Long mouseDragXPos = point.x + currentScrollXPos;
		//8. ���� ù ���ں��� �ʺ� ���� ���콺�� �巡���� ���� x��ǥ���� �۰ų� ��������
		//�׸��� ������ġ�� ���� ���ڰ������� �������� �ݺ��Ѵ�.
		Long currentLetterIndex = 0;
		Long currentLetterPos = this->textExtent->GetTextWidth(this->current
			->GetPartOfContent(currentLetterIndex));
		while (currentLetterPos < mouseDragXPos
			&& currentLetterIndex < this->current->GetLength())
		{
			//8.1 ���� ��ġ�� ������Ų��.
			currentLetterIndex++;
			//8.2 ���� �ʺ� ���Ѵ�.
			currentLetterPos = this->textExtent->GetTextWidth(this->current
				->GetPartOfContent(currentLetterIndex));
		}
		//10. ���� ���ڱ��� ���� �ؽ�Ʈ �ʺ� ���Ѵ�.
		Long previousLetterPos = this->textExtent->
			GetTextWidth(this->current->GetPartOfContent(currentLetterIndex - 1));
		//11. ���콺Ŭ�� x��ǥ���� ���� ���ڱ��� ���� �ؽ�Ʈ �ʺ��� ���� ���Ѵ�.
		Long previousDifference = mouseDragXPos - previousLetterPos;
		//12. ���� ���ڱ��� ���� �ؽ�Ʈ �ʺ񿡼� ���콺Ŭ�� x��ǥ�� ���� ���Ѵ�.
		Long currentDifference = currentLetterPos - mouseDragXPos;
		//13. ���� ���� ���� ������ �۰ų� ������
		if (previousDifference <= currentDifference)
		{
			//13.1 ���� ���� ��ġ�� �̵���Ų��.
			currentLetterIndex = this->current->Move(currentLetterIndex - 1);
		}
		//14. ���� ���� ���� ������ ũ��
		else
		{
			//14.1 ���� ��ġ�� �̵���Ų��.
			currentLetterIndex = this->current->Move(currentLetterIndex);
		}
		//10. ���콺�� ���������� �̵�������
		if (previousRowIndex != currentRowIndex || previousLetterIndex != currentLetterIndex)
		{
			//10.1 ������ ���� ���� �ƴϸ�
			if (this->isSelecting == false)
			{
				//10.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
				this->isSelecting = true;
				//10.1.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->selectedStartXPos = previousLetterIndex;
				//10.1.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->selectedStartYPos = previousRowIndex;
				//10.1.4 ���ÿ����� ����� ������ �޴���ư���� �����ϱ�, �߶󳻱�, �����޴��� Ȱ��ȭ��Ų��.
				this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			}
			//10.2 ������ ���� ���̸�
			else
			{
				//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
				this->selectingTexts->Undo();
			}
			//10.2 ������ ���۵Ǵ� �ٺ��� ���� ���� ��ġ�� ��ũ��
			if (this->selectedStartYPos < currentRowIndex)
			{
				//10.2.1 �ؽ�Ʈ�� �������� �����Ѵ�.
				this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
					currentRowIndex, currentLetterIndex);
			}
			//10.3 ������ ���۵Ǵ� ���� ��ġ�� ���� ���� ��ġ�� ������ 
			else if (this->selectedStartYPos == currentRowIndex)
			{
				//10.3.1 ������ ���۵Ǵ� ���� ��ġ���� ���� ���� ��ġ�� ��ũ��
				if (this->selectedStartXPos < currentLetterIndex)
				{
					//10.3.1.1 �ؽ�Ʈ�� �������� �����Ѵ�.
					this->selectingTexts->DoNext(this->selectedStartYPos, this->selectedStartXPos,
						currentRowIndex, currentLetterIndex);
				}
				//10.3.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ���� ��ġ���� �� ũ��
				else
				{
					//10.3.2.1 �ؽ�Ʈ�� �������� �����Ѵ�.
					this->selectingTexts->DoPrevious(this->selectedStartYPos,
						this->selectedStartXPos, currentRowIndex, currentLetterIndex);
				}
			}
			//10.4 ������ ���۵Ǵ� ���� ��ġ�� ���� ���� ��ġ���� �� ũ��
			else if (this->selectedStartYPos > currentRowIndex)
			{
				//10.4.1 �ؽ�Ʈ�� �������� �����Ѵ�.
				this->selectingTexts->DoPrevious(this->selectedStartYPos,
					this->selectedStartXPos, currentRowIndex, currentLetterIndex);
			}
			//10.5 ��������� �������鿡�� �˸���.
			this->Notify();
			this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
	
}

//���� ���콺 ��ư�� ���� ��
void NotepadForm::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	//���콺 ���� ��ư�� ���ٰ� ǥ���Ѵ�.
	this->isMouseLButtonClicked = false;
}

//�޸��忡�� ȭ���� ũ�Ⱑ ����� ��
void NotepadForm::OnSize(UINT nType, int cx, int cy)
{
	//���� �޸����� â�� ����(�ּ�ȭ, �ִ�ȭ, ���� ũ��� ����)�� ���� ���̿� ���� ���̸� �Է¹޴´�.
	CFrameWnd::OnSize(nType, cx, cy);
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->GetClientRect(&rect);
	cx = rect.Width();
	//2. ���� �޸����� ���°� �ּ�ȭ�� �ƴϸ�(�ּ�ȭ�̸� cx�� cy ���� �Ѵ� 0�� �ǰ�, 
	//cx�� 0�̸� �Ʒ����� cxũ��� �ݺ��� �����µ� ���ѹݺ��� �߻��ؼ� �ּ�ȭ��ư�� ������ ���̳�!) 
	if (nType != SIZE_MINIMIZED)
	{
		//2.1 �ڵ��ٹٲ��� �������̸�
		if (this->isRowAutoChanging == true)
		{
			//2.1.1 RowAutoChange�� �����Ѵ�.(���� �Ҵ��ϸ� ���߿� ���� �Ҵ������� �������
			//�׷��� �ּҾ��� ���ÿ� �Ҵ��ϸ� �� �Լ� ������ ����Ǹ� �ڵ����� ���� �����.)
			//���⼭�� ���ÿ����� RowAutoChange�� ������ ���� ���Ѱ��̱� ������ ���ÿ� �Ҵ��ϴ°� ȿ������!
			RowAutoChange rowAutoChange(this);
			//2.1.2. �ڵ����� ���� ���� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			Long changedRowPos = this->note->GetCurrent();
			Long changedLetterPos = this->current->GetCurrent();
			Long originRowPos = 0;
			Long originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//2.1.3 �޸����� ���� ȭ�� ũ�Ⱑ �ٲ������
			if (this->previousPageWidth != cx)
			{
				//2.1.3.1 �ڵ������� ����Ѵ�.
				rowAutoChange.UndoAllRows();
				//2.1.3.2 ȭ��ũ�� ���濡 ���� �ٽ� �ڵ������� ���ش�.
				rowAutoChange.DoAllRows();
				//2.1.3.3 �޸����� ���� ȭ���� ���� ���̰� �ٲ���� ������ �̸� �������ش�.
				this->previousPageWidth = cx;
			}
			//2.1.4 �޸����� ���� ȭ�� ũ�Ⱑ �ٲ��� �ʾ�����
			else
			{
				//2.1.4.1 �ڵ������� ����Ѵ�.
				rowAutoChange.UndoRow();
				//2.1.4.2 ȭ��ũ�� ���濡 ���� �ٽ� �ڵ������� ���ش�.
				rowAutoChange.DoRow();
			}
			//2.1.5 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//2.1.6 ���� ���� ��ġ�� ������ ��ġ�� �����Ѵ�.
			Long currentRowIndex = this->note->Move(changedRowPos);
			this->current = this->note->GetAt(currentRowIndex);
			Long currentLetterIndex = this->current->Move(changedLetterPos);
			
		}
		//2.3 ĳ���� ��ġ�� ��ũ�� ������ ����Ǿ����� �˸���.
		this->Notify();
		//2.4 ��������� �����Ѵ�.
		//Invalidate�ϸ� ������ �����ϴ°Ŷ� �ǽð� ������ �ʴµ� 
		//RedrawWindow�� �̿��ϸ� �ǽð����� �����ؼ� ���콺 Ŀ�� �̵��� �ξ� ������
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

//�޴���ư�� Ŭ������ ��
void NotepadForm::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	//1. �ܺ�Ŭ�����忡 ���ڿ��� ����Ǿ� ������
	unsigned int priority_list = CF_TEXT;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//1.1 �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	}
	//2. �ܺ�Ŭ�����忡 ���ڿ��� ����Ǿ� ���� ������
	else if (GetPriorityClipboardFormat(&priority_list, 1) == NULL)
	{
		//2.1 �ٿ��ֱ� �޴��� ��Ȱ��ȭ��Ų��.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->mouseRButtonMenu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
}

BOOL NotepadForm::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//�޸��忡�� �ݱ��ư�� Ŭ������ ��
void NotepadForm::OnClose()
{
	//1. file�� �����Ѵ�.
	File file;
	int messageBoxButton = IDCANCEL;
	//2. �޸��忡 ���� ������ ������
	if (this->isDirty == true)
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
				this->isDirty = false;
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
				this->isDirty = false;
				//2.5.2.5 �޸��� ������ �ٲ۴�.
				string name = this->fileName;
				name += " - �޸���";
				SetWindowText(CString(name.c_str()));
			}
		}
	}
	//3. �޼����ڽ����� CANCEL�� �������� �ʾҰų� ����� ������ ������
	if (messageBoxButton != IDCANCEL || this->isDirty == false)
	{
		//���α׷��� �����ϱ� ���� ���� �ڵ������� ���� ������ �ƴ��� ���θ� ������Ʈ���� ������.
		AfxGetApp()->WriteProfileInt("NotepadSection", "IsRowAutoChanging",
			!this->isRowAutoChanging);
		//���α׷��� �����ϱ� ���� ���� �۲� ������ ������Ʈ���� ������.
		AfxGetApp()->WriteProfileInt("NotepadSection", "FontSize", this->font.GetSize());
		string style = this->font.GetLogFont().lfFaceName;
		AfxGetApp()->WriteProfileString("NotepadSection", "FontStyle", style.c_str());
		//3.1 �޸����� �����.
		if (this->note != NULL)
		{
			delete this->note;
			//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
			//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
			//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
			//delete this->current;
		}
		//3.2 Ŭ�����带 �����.
		if (this->clipboard != NULL)
		{
			delete this->clipboard;
		}
		//3.3 TextExtent�� �Ҵ������Ѵ�.
		if (this->textExtent != NULL)
		{
			delete this->textExtent;
		}
		//3.4 SelectingTexts�� �Ҵ������Ѵ�.
		if (this->selectingTexts != NULL)
		{
			delete this->selectingTexts;
		}
		//3.5 CFindReplaceDialog�� �����.
		if (this->findReplaceDialog != 0)
		{
			this->findReplaceDialog->DestroyWindow();
		}
		//3.6 CommandHistory�� �Ҵ������Ѵ�.
		if (this->commandHistory != 0)
		{
			delete this->commandHistory;
		}
		//3.7 PreviewForm�� �����.
		if (this->previewForm != 0)
		{
			this->previewForm->SendMessage(WM_CLOSE);
			delete this->previewForm;
		}
		//3.8 ����Ʈ������ �Ҵ������Ѵ�.
		if (this->printInformation != 0)
		{
			delete this->printInformation;
		}
		if (this->pageSetUpInformation != 0)
		{
			delete this->pageSetUpInformation;
		}
		// �޸����� �ݴ´�.
		CFrameWnd::OnClose();
	}
}

//Save�޼����ڽ�
LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam)
{
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