#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandCreator.h"
#include "KeyActionCreator.h"
#include "Command.h"
#include "Glyph.h"
#include "CaretManager.h"
#include "TextExtent.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog�������

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
	ON_COMMAND_RANGE(IDM_FILE_OPEN, IDM_FONT_CHANGE, OnCommand)
	ON_WM_KEYDOWN()
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
	wsprintf(logFont.lfFaceName, "���� ���");
	this->font = Font(logFont, RGB(0, 0, 0));
	this->textExtent = NULL;
	//this->textExtent = new TextExtent(this);

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
	//Menu�� notepadForm�� ������� OnCreate�� ����Ǿ ������ ��� ����Ǿ� �ֱ⶧���� ���� �ȳ�
	//���࿡ �ӽú����� OnCreate������ Menu�� ������ OnCreate�� ����ɶ� Menu�� ��������� ���� �Ҹ��
	//6. CMenu�� notepadForm�� �����Ѵ�.
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//7. textExtent�� ���� �Ҵ��Ѵ�.
	//notepadForm�� ������ �� �⺻(����Ʈ)�۲� ������ �������� TextExtent�� �����Ҵ��� �������ְ�,
	//notepadForm�� �Ҹ�� ��(OnClose) ���� �Ҵ�� TextExtent�� �Ҵ����� �Ҹ����ָ�ȴ�.
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

	return 0;
}

//Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//2. glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//3. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (nChar != '\n' && nChar != '\r')
	{
		//3.1 ���� �ٿ� ���ڸ� �߰��Ѵ�.
		index = this->current->Add(glyph);
	}
	//4. �Է¹��� ���ڰ� ���๮���̸�
	else if (nChar == '\n' || nChar == '\r')
	{
		//4.1 ���ο� ���� �߰��Ѵ�.
		index = this->note->Add(glyph);
		//4.2 ���� ���� ��ġ�� ���� �����Ѵ�.
		this->current = this->note->GetAt(index);
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
	dc.SetBkMode(TRANSPARENT);
	//3. �ؽ�Ʈ�� ������ ����.
	dc.SetTextColor(this->font.GetColor());
	//4. ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//5. CFont�� �����Ѵ�.
	CFont font;
	//6. �۾�ũ��� �۾�ü�� ���ϴ�.
	//font.CreatePointFont(this->font.GetSize(), this->font.GetFaceName().c_str());
	font.CreateFontIndirect(&this->font.GetLogFont());
	//7. ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//8. TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//9. �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//10. note�� ����� ���ڵ��� ����Ѵ�.
	Long i = 0;
	CString content;
	//11. �ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		//11.1 �ؽ�Ʈ ���� ��ġ���� ó������ (0,0)���� �����ϰ� �ι�°���� (0, 150)���� ������.
		dc.TextOut(0, i * text.tmHeight, content);
		i++;
	}
	//font�� ��Ʈ�����ȭ���ڿ��� ����Ǿ����� ���� font�� ������ ���� ����� font�� ������ �� ����. 
	font.DeleteObject();
}

//�ѱ��� �Է¹��� ��
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
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
	//3. IsComposing���� '��'�̸�
	if (this->IsComposing == true)
	{
		//3.1 ���� ���� ���� �ѱ��� �����.(ĳ���� ���� ���� ��ġ���� -1���� ���ڸ� �����Ѵ�.)
		this->current->Remove(this->current->GetLength() - 1);
	}
	//4. isComposing���� '����'�̸�
	else
	{
		//4.1 isComposing���� '��'���� �ٲ۴�.
		this->IsComposing = true;
	}
	//5. ���� ���� DoubleByteLetter�� ���� �ٿ� �߰��Ѵ�.
	Long letterIndex = this->current->Add(doubleByteLetter);
	//6. ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
	this->Notify();
	//7. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//8. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//9. �����Ѵ�.
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
	//3. ������ �������̴� �ѱ��� �����.
	this->current->Remove(this->current->GetLength() - 1);
	//4. �����ٿ� �ϼ��� �ѱ��� �߰��Ѵ�.
	this->current->Add(doubleByteLetter);
	//5. IsComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//6. ĳ���� ����Ǿ����� �˸���.
	this->Notify();
	//7. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//8. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//9. �����Ѵ�.
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
	CaretManager* caretManager = new CaretManager(this);
	//2. ĳ���� ����Ǿ����� �������鿡�� �˸���.
	this->Notify();
}

//�޸����� Focus�� ���� ��
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//observer�ּҹ迭���� CaretManager�� ã�� ������ �ݺ��Ѵ�.
	Long i = this->length - 1;
	//1. ������ ����Ʈ���� �������� ���Ѵ�.
	Observer* observer = this->observers.GetAt(i);
	//2. i�� 0���� ũ�ų� ���� ���� �������� ĳ���Ŵ����� �ƴѵ��� �ݺ��Ѵ�.
	while (i >= 0 && dynamic_cast<CaretManager*>(observer) != 0)
	{
		//2.1 ������ ����Ʈ���� �������� ���Ѵ�.
		observer = this->observers.GetAt(i);
		//2.2 i�� ���ҽ�Ų��.
		i--;
	}
	//3. �������� CaretManager�̸�
	if (dynamic_cast<CaretManager*>(observer))
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