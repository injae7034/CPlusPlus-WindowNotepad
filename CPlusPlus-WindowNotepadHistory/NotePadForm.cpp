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

#include "CommandHistory.h"
#include "TextingOutVisitor.h"
#include "SelectingVisitor.h"
#include "SelectingTexts.h"

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
	//resource.h���� ���� ó���� �߰��Ȱ� ���۹����̰�, ���� �������� �߰��Ȱ� ������ ������
	//�������� �޴� �׸��̶��� �ƹ� ����� ����!!
	ON_COMMAND_RANGE(IDM_FILE_OPEN, ID_ONCHARCOMMAND, OnCommand)
	ON_WM_MENUSELECT(IDR_MENU1 ,OnMenuSelect)
	//ON_REGISTERED_MESSAGE() //ã������ȭ���ڿ��� �θ�������� �޼����� �����ϱ� ���� �ʿ���
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
	this->isComposing = false;//false�� �ʱ�ȭ��Ŵ
	this->isDirty = false;//false�� �ʱ�ȭ��Ŵ
	this->isSelecting = false;//false�� �ʱ�ȭ��Ŵ
	this->isRowAutoChanging = false;//false�� �ʱ�ȭ ��Ŵ.
	this->fileName = "���� ����";
	this->filePath = "";
	this->previousPageWidth = 0;//ó�������ɶ��� ���� ȭ�� �ʺ� 0���� �ʱ�ȭ����
	this->selectedStartXPos = 0;//ó�������ɶ��� ���õ� texts�� ���� ������ 0���� �ʱ�ȭ����
	this->selectedStartYPos = 0;//ó�������ɶ��� ���õ� texts�� ���� ������ 0���� �ʱ�ȭ����
	this->glyph = 0;//�޸����� ó�� �����Ǹ� ���ڰ� ���� ������ 0���� �ʱ�ȭ����
	//CFont���� ����ϰ��� �ϴ� ����ũ��� ����ü�� �ʱ�ȭ��Ŵ.
	//�⺻�����ڷ� ������ this->font�� �Ű����� 5�������ڷ� ġȯ(=)��Ŵ
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfHeight = 100;
	wsprintf(logFont.lfFaceName, "�ü�ü");
	this->font = Font(logFont, RGB(0, 0, 0));
	this->textExtent = NULL;
	this->selectingTexts = NULL;
}

//�޸��� �����찡 ������ ��
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
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
	//9. CMenu�� �޴����� �ڵ����� Enable�Ǵ� ���� ���� ���� FALSE ó���� �������
	//�ƴϸ� �ٸ� ������ Unenable���ѵ� ����ؼ� �ڵ����� Enable���ѹ���!
	this->m_bAutoMenuEnable = FALSE;
	//10. �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ��Ų��. ��Ȱ��ȭ�� ����Ʈ�� ���ÿ����� ����� Ȱ��ȭ������!
	this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//11. �ܺ�Ŭ�����忡 ���� ���ڿ��� ����Ǿ�������
	unsigned int priority_list = CF_TEXT;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//11.1 �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	}
	//12. �ܺ�Ŭ�����忡 ���� ���ڿ��� ����Ǿ� ����������
	else if (GetPriorityClipboardFormat(&priority_list, 1) == NULL)
	{
		//12.1 �ٿ��ֱ� �޴��� ��Ȱ��ȭ��Ų��.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//13. textExtent�� ���� �Ҵ��Ѵ�.
	this->textExtent = new TextExtent(this);
	//14. ������ �޸����� ��Ʈ(����)�� �ҷ��´�.
	File file;
	string path = "test.txt";
	file.Load(this, path);
	//15. ó�� ��������� �޸��� �̸��� ���Ѵ�.
	string name = this->fileName;
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//16. ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
	rowIndex = this->note->First();
	//17. ���� ���� ��ġ�� �ٽ� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	//18. ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
	Long letterIndex = this->current->First();
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
	if (nChar != VK_BACK)
	{
		if (!(ctrlPressedCheck &0x80))
		{
			//1.1.1 glyphCreator�� �����Ѵ�.
			GlyphCreator glyphCreator;
			//1.1.2 glyph�� �����ؼ� �����Ѵ�.
			this->glyph = glyphCreator.Create((char*)&nChar);
			//1.1.3 OnCommand�� �޼����� ������.
			this->SendMessage(WM_COMMAND, ID_ONCHARCOMMAND);
		}

	}
		

	
	
		//Ctrl+a�� Ctrl+z�� Ctrl+y�� ����� �� OnCharCommand�� ������� �ʱ� ���ؼ� �ʿ��� ���ǹ�
		//1.1 CtrlŰ�� �ȴ����� ������
		//if (ctrlPressedCheck >= 0)
		//{
		//}
	
}

//�޸��忡 �ؽ�Ʈ�� ����� ��//��½� Visitor��������
void NotepadForm::OnPaint()
{
	//1. CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//2. ���� �ȵ� ������ ����� ������ �����Ѵ�.
	TextingOutVisitor textingOutVisitor = TextingOutVisitor(this, &dc, 0, 0);
	//3. ���õ� ������ ����� ������ �����Ѵ�.
	SelectingVisitor selectingVisitor = SelectingVisitor(this, &dc, 0, 0);
	//4. ������ �ȵ� ������ ����Ѵ�.
	this->note->Accept(&textingOutVisitor);
	//5. ���õ� ������ ����Ѵ�.
	this->note->Accept(&selectingVisitor);
}

//�ѱ��� �Է¹��� ��
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//1. �޸��忡�� ���õ� texts�� ������
	if (this->isSelecting == true)
	{
		//1.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	//2. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	//3. Ű����κ��� �Է¹��� ������ �������� ���ο� �ѱ��� �����Ѵ�.
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long index = this->current->GetCurrent();
	//5. IsComposing���� '��'�̸�(�ѱ��� �������� �����̸�)
	if (this->isComposing == true)
	{
		//5.1 ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
		//�׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
		//���� �ѱ��� ������ ���� �Է� ���� �ѱ��� ��ü�� �� �ִ�.
		this->current->Remove(index - 1);
		//5.2 ���ŵ� current�� ��ġ�� index�� �����Ѵ�.
		index = this->current->GetCurrent();
	}
	//6. ������ġ�� �ѱ��� ������ ������ �ѱ��� �������� �ƴ����� ���¸� �����Ѵ�.
	this->isComposing = false;
	//7. ���ο� �ѱ��� �ԷµǾ�����(�ѱ� �����߿� ���ڸ� �� ���������� '\0'���ڷ�
	//OnComposition�� �Էµȴ� �齺���̽�Ű�� �ԷµǸ� ���� �ѱ��� �������� '\0'�� ��� ���� �ȴ�.)
	if (koreanLetter[0] != '\0')
	{
		//7.1 doubleByteLetter�� �����Ѵ�.
		Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
		//7.2 index�� ���� ���� length�� ������
		if (index == this->current->GetLength())
		{
			//7.2.1 ���� ���� ������ ���� �ڿ� ���ο� �ѱ��� �߰��Ѵ�.
			index = this->current->Add(doubleByteLetter);
		}
		//7.3 index�� ���� ���� length�� �ٸ���
		else
		{
			//7.3.1 ���� ���� index��°�� ���ο� �ѱ��� ���� ����.
			index = this->current->Add(index, doubleByteLetter);
		}
		//7.4 �ѱ��� ���� ��ġ�� �߰��߱⶧���� �ѱ��� �������� ���·� �����Ѵ�.
		this->isComposing = true;
	}
	//8. �ѱ� �����߿� �齺���̽�Ű�� ������ ���� ���� �ѱ��� ����������
	else
	{
		//BackSpace�� DeleteŰ�� ������ �ѱ������߿� ����� ��쵵 OnSize�� ������� �Ѵ�.
		//8.1 �ڵ� �� �ٲ��� �������̸�
		if (this->isRowAutoChanging == true)
		{
			//8.1.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->SendMessage(WM_SIZE);
		}
	}
	//9. ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
	this->Notify();
	//10. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//11. �޸��忡 ��������� ������ �����Ѵ�.
	this->isDirty = true;
	//12. �����Ѵ�.
	Invalidate(TRUE);

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
		//3.2 ���ڸ� �Է��ϴ� command�̸� 
		if (nId == ID_ONCHARCOMMAND || nId == ID_ONIMECHARCOMMAND || nId == IDM_NOTE_PASTE)
		{
			//3.2.1.1 UndoList�� �߰��Ѵ�.
			this->commandHistory->PushUndoList(command);
			//3.2.1.2 redoList�� �ʱ�ȭ��Ų��.
			this->commandHistory->MakeRedoListEmpty();
		}
		//3.3 ���ڸ� ����� command�̸�
		else if (nId == ID_BACKSPACEKEYACTIONCOMMAND || nId == ID_DELETEKEYACTIONCOMMAND
			|| nId == ID_CTRLBACKSPACEKEYACTIONCOMMAND || nId == ID_CTRLDELETEKEYACTIONCOMMAND
			|| nId == ID_SHIFTCTRLBACKSPACEKEYACTIONCOMMAND ||
			nId == ID_SHIFTCTRLDELETEKEYACTIONCOMMAND)
		{
			//3.3.1 Command�� ��������� ������
			if (command->IsDirty() == true)
			{
				//3.3.1.1 UndoList�� �߰��Ѵ�.
				this->commandHistory->PushUndoList(command);
				//3.3.1.2 redoList�� �ʱ�ȭ��Ų��.
				this->commandHistory->MakeRedoListEmpty();
			}
			//3.3.2 Command�� ��������� ������
			else
			{
				//3.3.2.1 command�� �Ҵ������Ѵ�.
				if (command != 0)
				{
					delete command;
				}
			}
		}
		//3.4 ���ÿ����� ����ų� ���ÿ����� �߶󳻴� command�̸�
		else if (nId == IDM_NOTE_REMOVE || nId == IDM_NOTE_CUT)
		{
			//3.3.1.1 command�� ���߰��ϴ� ǥ�ø� �Ѵ�.
			command->SetUndoMacroEnd();
			//3.3.1.2 UndoList�� �߰��Ѵ�.
			this->commandHistory->PushUndoList(command);
			//3.3.1.3 redoList�� �ʱ�ȭ��Ų��.
			this->commandHistory->MakeRedoListEmpty();
		}
		//3.3 ���ڸ� �Է��ϴ� command�� ���ڸ� ����� command�� �ƴϸ�
		//undoList�� �� command�� �ƴϸ� ���� �Ҵ������� ����� �޸� ������ �Ȼ����.
		//undoList�� �� command�� commandHistory�� �Ҹ�� ��, �Ҹ��ڿ��� ���� �Ҵ������ȴ�.
		else
		{
			//3.3.1 command�� �Ҵ������Ѵ�.
			if (command != 0)
			{
				delete command;
			}
		}
	}
	//4. ��ȭ�� �޸��忡 �����Ѵ�.
	this->Notify();
	this->Invalidate();
}

//�޸��忡�� Ű����� �̵��ϱ�
void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//���ڰ� �ƹ��͵� ���� ù�� ù��° ������ġ���� shift������ �̵��ϸ� �����Ұ� ���µ� ������ �Ǿ
	//�װ� �������� OnKeyDown���� ���ǹ����� ó������ ������ ���Ҵµ� �̷��� �Ǹ� ���߿� ���ڰ� �ϳ���
	//���� ��� �������(CtrlZKeyAction)�� �ٽ� ����(CtrlYKeyAction) ���� �ȵ��� ������ �߻���!
	//�̷��� ó������ ���� ���� ���� shift���� ���������� �̵��� ������ ������ ���ְ�,
	//�̵��� ������ ������ ���ϰ� �׳� ���������� �ϴ°� �ξ� �� ����ϴ�.
	//1. �޸����� ��Ʈ���� ���� ������ ���Ѵ�.
	//Long rowCountOfNote = this->note->GetLength();
	//2. �޸����� ��Ʈ�� ������ ���� ���� ������ ���Ѵ�.
	//Long letterCountOfFirstRow = this->note->GetAt(rowCountOfNote - 1)->GetLength();
	//3. �޸����� ��Ʈ�� ���� ������ �ϳ��ִµ� �� ���� ���ڰ� �ϳ��� ���� ��찡 �ƴ϶��
	//���ڰ� �ϳ��� ���� �ٸ� 2���̻� �ִ� �����ʹ� ������ �����ϱ� ������ ����, �߶󳻱�, ������ �����ϴ�.
	//if (rowCountOfNote != 1 || letterCountOfFirstRow != 0)
	//{
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
			}
			//3.3.4 �޸��忡 ������ �Ǿ� ������
			else
			{
				this->menu.EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
				this->menu.EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
			}
			//3.3.5 ��ȭ�� �޸��忡 �����Ѵ�.
			//if �����ȿ��� Notify�� ����� Ctrl�̳� Shift, Alt Capslock�� ���� Ư�����Ű�� ������ ��
			//Notify�� ȣ���� ĳ���� �ִ� ������ ��ũ���� �̵����� �ʴ´�. OnKeyDown�� Ű����Ű �� ���
			//Ű�� �������� ȣ��Ǳ� ������ ���ϴ� keyAction�� �ƴѰ�� Notify�� ������� �ʰ� �ؾ��Ѵ�!
			this->Notify();
			this->Invalidate();	
		}
	//}
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
			//2.1.8 ĳ���� ��ġ�� ����Ǿ����� �˸���.
			this->Notify();
			//2.1.9 ��������� �����Ѵ�.
			this->Invalidate(TRUE);
		}
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
	}
	//2. �ܺ�Ŭ�����忡 ���ڿ��� ����Ǿ� ���� ������
	else if (GetPriorityClipboardFormat(&priority_list, 1) == NULL)
	{
		//2.1 �ٿ��ֱ� �޴��� ��Ȱ��ȭ��Ų��.
		this->menu.EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
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
		
		// �޸����� �ݴ´�.
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