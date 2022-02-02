#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandCreator.h"
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
	//�ش����(IDM_FILE_OPEN ~ IDM_FILE_SAVEDIFFERENTNAME)�� id���� Ŭ���ϸ� OnCommand�Լ�����
	ON_COMMAND_RANGE(IDM_FILE_OPEN, IDM_FILE_SAVEDIFFERENTNAME, OnCommand)
	//ON_COMMAND(IDM_FILE_OPEN, OnFileOpenClicked)
	//ON_COMMAND(IDM_FILE_SAVE, OnFileSaveClicked)
	//ON_COMMAND(IDM_FILE_SAVEDIFFERENTNAME, OnFileSaveDifferentNameClicked)
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
	//�⺻�����ڷ� ������ this->font�� �Ű����� 2�������ڷ� ġȯ(=)��Ŵ
	this->font = Font(300, "�ü�ü");
	//�⺻�����ڷ� ������ this->caret�� �Ű����� 1�������ڷ� ġȯ(=)��Ŵ.
	this->caret = Caret(this);
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
	//7. ó�� ��������� �޸��� �̸��� ���Ѵ�.
	string name = this->fileName;
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));

	return 0;
}

//Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//2. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//3. glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//4. CFont�� �����Ѵ�.
	CFont font;
	//5. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//6. ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//7. TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//8. �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//9. ĳ���� �����Ѵ�.
	this->caret.Create(0, text.tmHeight);
	//10. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (nChar != '\n' && nChar != '\r')
	{
		//10.1 ���� �ٿ� ���ڸ� �߰��Ѵ�.
		index = this->current->Add(glyph);
		//10.2 �߰��� ������ �ʺ� ���Ѵ�.
		CString letter = CString(this->current->GetContent().c_str());
		CSize letterSize = dc.GetTextExtent(letter);
		//10.3 ĳ���� �̵���Ų��.
		this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//11. �Է¹��� ���ڰ� ���๮���̸�
	else if (nChar == '\n' || nChar == '\r')
	{
		//11.1 ���ο� ���� �߰��Ѵ�.
		index = this->note->Add(glyph);
		//11.2 ���� ���� ��ġ�� ���� �����Ѵ�.
		this->current = this->note->GetAt(index);
		//11.3 ĳ���� �̵���Ų��.
		this->caret.Move(0, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//12. ĳ���� ���̰� �Ѵ�.
	this->caret.Show();
	//13. isComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//14. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//15. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//16. �����Ѵ�.
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
	dc.SetTextColor(RGB(0, 0, 0));
	//4. ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//5. CFont�� �����Ѵ�.
	CFont font;
	//6. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
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
}

//�ѱ��� �Է¹��� ��
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//1. CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//2. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//3. doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//4. IsComposing���� '��'�̸�
	if (this->IsComposing == true)
	{
		//4.1 ���� ���� ���� �ѱ��� �����.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//5. isComposing���� '����'�̸�
	else
	{
		//5.1 isComposing���� '��'���� �ٲ۴�.
		this->IsComposing = true;
	}
	//6. ���� ���� DoubleByteLetter�� ���� �ٿ� �߰��Ѵ�.
	Long letterIndex = this->current->Add(doubleByteLetter);
	//7. CFont�� �����Ѵ�.
	CFont font;
	//8. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//9. ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//10. �߰��� ���ڸ� ���� ��ü �ؽ�Ʈ�� size�� ���Ѵ�.
	CString text = CString(this->current->GetContent().c_str());
	CSize textSize = dc.GetTextExtent(text);
	//11. �߰��� �ѱ��� size�� ���Ѵ�.
	CString letter = CString(this->current->GetAt(letterIndex)->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//12. ĳ���� �����Ѵ�.
	TEXTMETRIC koreanCaret;
	dc.GetTextMetrics(&koreanCaret);
	this->caret.Create(letterSize.cx, koreanCaret.tmHeight);
	//CreateSolidCaret(letterSize.cx, koreanCaret.tmHeight);
	//13. ĳ���� �̵���Ų��.
	this->caret.Move(textSize.cx - letterSize.cx, (this->note->GetCurrent() - 1) * koreanCaret.tmHeight);
	//14. ĳ���� ���̰� �Ѵ�.
	this->caret.Show();
	//15. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//16. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//17. �����Ѵ�.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//�ϼ��� �ѱ��� �Է¹޾��� ��
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//1. CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//2. CFont�� �����Ѵ�.
	CFont font;
	//3. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//4. ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//5. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//6. doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//7. ������ �������̴� �ѱ��� �����.
	this->current->Remove(this->current->GetLength() - 1);
	//8. �����ٿ� �ϼ��� �ѱ��� �߰��Ѵ�.
	this->current->Add(doubleByteLetter);
	//9. IsComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//10. ĳ���� �����Ѵ�
	TEXTMETRIC text;
	dc.GetTextMetrics(&text);
	this->caret.Create(0, text.tmHeight);
	//11. ĳ���� �̵��� ��ġ�� ���Ѵ�.
	CString letter = CString(this->current->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//12. ĳ���� �̵���Ų��.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//13. ĳ���� �����ش�.
	this->caret.Show();
	//14 �޸��� ���� *�� �߰��Ѵ�.
	string name = this->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//15. �޸��忡 ��������� ������ �����Ѵ�.
	this->IsDirty = true;
	//16. �����Ѵ�.
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
	//1. CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//2. CFont�� �����Ѵ�.
	CFont font;
	//3. �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//4. ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//5. TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//6. �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//7. ĳ���� �����Ѵ�.
	this->caret.Create(0, text.tmHeight);
	//8. �������� �ؽ�Ʈ���� �����Ѵ�.
	CString letter = CString(this->current->GetContent().c_str());
	//9. �������� �ؽ�Ʈ���� size�� ���Ѵ�.
	CSize letterSize = dc.GetTextExtent(letter);
	//10. ĳ���� �̵���Ų��.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//11. ĳ���� ���̰� �Ѵ�.
	this->caret.Show();
}

//�޸����� Focus�� ���� ��
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//1. ĳ���� �����.
	HideCaret();
	//2. ĳ���� �ı��Ѵ�.
	::DestroyCaret();
}

//Command����
void NotepadForm::OnCommand(UINT nId)
{
	//1. CommandCreator�� �����Ѵ�.
	CommandCreator commandCreator(this);
	//2. ConcreteCommand�� �����Ѵ�.
	Command* command = commandCreator.Create(nId);
	//3. ConcreteCommand�� execute�� �����Ѵ�.
	command->Execute();
	//4. command�� �Ҵ������Ѵ�.
	delete command;
}

#if 0
//�޸��� �޴����� �ҷ����⸦ Ŭ������ ��
void NotepadForm::OnFileOpenClicked()
{
	//1. �޸��� ������ �˾Ƴ���.
	CString noteTitle;
	GetWindowText(noteTitle);
	string title = string(noteTitle);
	//2. �޸��� ���� '*'�� ������(��������� ������)
	if (title[0] == '*')
	{
		//�޼����ڽ� ������ ���Ѵ�.
		string message;
		//2.1 ���ϰ�ΰ� ������ ������
		if (this->file.GetPath() != "")
		{
			message = this->file.GetPath();
		}
		//2.2 ���� ��ΰ� �������� ������
		else
		{
			message = this->file.GetName();
		}
		message.insert(0, "���� ������ ");
		message += "�� �����Ͻðڽ��ϱ�?";
		//2.1 Save�޼����ڽ��� ����Ѵ�.
		int messageBoxButton = SaveMessageBox(this->GetSafeHwnd(), message.c_str(), "�޸���", MB_YESNOCANCEL);

		//3. Save�޼����ڽ����� Yes�� ����������
		if (messageBoxButton == IDYES)
		{
			//1. ���ϰ�ΰ� ""�� �ƴϸ�(��Ʈ�� ó�� ������ ��찡 �ƴϸ�)
			if (this->file.GetPath() != "")
			{
				//1.1 ������ �޸����� �����Ѵ�.
				this->file.Save(this->file.GetName(), this->file.GetPath());
				//1.2 '*'�� �����ش�.
				title.erase(0, 1);
				SetWindowText(CString(title.c_str()));
				
			}
			//2.2 ���ϰ�ΰ� ������ ���� ������(��Ʈ�� ó�� ������ ���)
			else
			{
				//2.2.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
				LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
				CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
				//2.2.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
				if (fileDialog.DoModal() == IDOK)
				{
					//2.2.2.1 ������ ������ �̸��� ���Ѵ�.
					CString fileName = fileDialog.GetFileName();
					//2.2.2.2 ������ ������ ��θ� ���Ѵ�.
					CString filePathName = fileDialog.GetPathName();
					//2.5 ������ �̸����� .txt�� �����Ѵ�.
					string name = string(fileName);
					Long eraseIndex = name.find(".txt");
					name.erase(eraseIndex, 4);
					//2.2.2.3 ������ �޸����� �����Ѵ�.
					this->file.Save(name, string(filePathName));
					//2.13 �޸��� ������ �ٲ۴�.
					name += " - �޸���";
					SetWindowText(CString(name.c_str()));
				}
			}
		}
		//4. �޼����ڽ����� yes�� no�� ������ ���
		if (messageBoxButton == IDYES || messageBoxButton == IDNO)
		{
			//3.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
			LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
			CFileDialog fileDialog(TRUE, _T("txt"), NULL, 0, filesFilter, this, 0, 1);
			//3.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
			if (fileDialog.DoModal() == IDOK)
			{
				//3.2.1 �ҷ��� ������ �̸��� ���Ѵ�.
				CString fileName = fileDialog.GetFileName();
				//3.2.2 �ҷ��� ������ ��θ� ���Ѵ�.
				CString filePathName = fileDialog.GetPathName();
				//3.2.3 ������ �̸����� .txt�� �����Ѵ�.
				string name = string(fileName);
				Long eraseIndex = name.find(".txt");
				name.erase(eraseIndex, 4);
				//3.2.4 ���� �޸����� note�� �����.
				//�޸��忡 note�� �Ѱ��ۿ� ���� ���� �ٸ� �޸����� �ҷ����� ���� ����note�� ���ְ�
				//���� �ҷ����� �޸����� note�� Load�� ���� �ҷ����� ���ο� �޸��忡 ����������!!
				if (this->note != NULL)
				{
					delete this->note;
					//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
					//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
					//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
					//delete this->current;
				}
				//�ҷ����� �޸����� ���� ���ο� note�� ����.
				//3.2.5 glyphCreator�� �����.
				GlyphCreator glyphCreator;
				//3.2.6 ��Ʈ�� �����.
				this->note = glyphCreator.Create((char*)"\0");
				//3.2.7 ���� �����.
				Glyph* row = glyphCreator.Create((char*)"\n");
				//3.2.8 ���� �޸��忡 �߰��Ѵ�.
				Long rowIndex;
				rowIndex = this->note->Add(row);
				//3.2.9 ���� ���� ��ġ�� �����Ѵ�.
				this->current = this->note->GetAt(rowIndex);
				//3.2.10 ������ �޸����� ��Ʈ(����)�� �ҷ��´�.
				this->file.Load(name, string(filePathName));
				//3.2.11 �޸��� ������ �ٲ۴�.
				name += " - �޸���";
				SetWindowText(CString(name.c_str()));
				//3.2.12 �����Ѵ�.
				Invalidate(TRUE);
				//ĳ���� �ҷ��� note(����)�� �°� �ٽ� �������ش�.
				//3.2.13 CClientDC�� �����Ѵ�.
				CClientDC dc(this);
				//3.2.14 CFont�� �����Ѵ�.
				CFont font;
				//3.2.15 �۾�ũ��� �۾�ü�� ���ϴ�.
				font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
				//3.2.16 ��Ʈ�� dc�� �����Ѵ�.
				dc.SelectObject(font);
				//3.2.17 TEXTMETRIC�� �����Ѵ�.
				TEXTMETRIC text;
				//3.2.18 �۲��� ������ ��´�.
				dc.GetTextMetrics(&text);
				//3.2.19 ĳ���� �����Ѵ�.
				this->caret.Create(0, text.tmHeight);
				//3.2.20 �������� �ؽ�Ʈ���� �����Ѵ�.
				CString letter = CString(this->current->GetContent().c_str());
				//3.2.21 �������� �ؽ�Ʈ���� size�� ���Ѵ�.
				CSize letterSize = dc.GetTextExtent(letter);
				//3.2.22 ĳ���� �̵���Ų��.
				this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
				//3.2.23 ĳ���� ���̰� �Ѵ�.
				this->caret.Show();
			}
		}
	}
	//��������� ������(���� �տ� '*'�� ������)
	else
	{
		//3.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
		LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
		CFileDialog fileDialog(TRUE, _T("txt"), NULL, 0, filesFilter, this, 0, 1);
		//3.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
		if (fileDialog.DoModal() == IDOK)
		{
			//3.2.1 �ҷ��� ������ �̸��� ���Ѵ�.
			CString fileName = fileDialog.GetFileName();
			//3.2.2 �ҷ��� ������ ��θ� ���Ѵ�.
			CString filePathName = fileDialog.GetPathName();
			//3.2.3 ������ �̸����� .txt�� �����Ѵ�.
			string name = string(fileName);
			Long eraseIndex = name.find(".txt");
			name.erase(eraseIndex, 4);
			//3.2.4 ���� �޸����� note�� �����.
			//�޸��忡 note�� �Ѱ��ۿ� ���� ���� �ٸ� �޸����� �ҷ����� ���� ����note�� ���ְ�
			//���� �ҷ����� �޸����� note�� Load�� ���� �ҷ����� ���ο� �޸��忡 ����������!!
			if (this->note != NULL)
			{
				delete this->note;
				//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
				//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
				//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
				//delete this->current;
			}
			//�ҷ����� �޸����� ���� ���ο� note�� ����.
			//3.2.5 glyphCreator�� �����.
			GlyphCreator glyphCreator;
			//3.2.6 ��Ʈ�� �����.
			this->note = glyphCreator.Create((char*)"\0");
			//3.2.7 ���� �����.
			Glyph* row = glyphCreator.Create((char*)"\n");
			//3.2.8 ���� �޸��忡 �߰��Ѵ�.
			Long rowIndex;
			rowIndex = this->note->Add(row);
			//3.2.9 ���� ���� ��ġ�� �����Ѵ�.
			this->current = this->note->GetAt(rowIndex);
			//3.2.10 ������ �޸����� ��Ʈ(����)�� �ҷ��´�.
			this->file.Load(name, string(filePathName));
			//3.2.11 �޸��� ������ �ٲ۴�.
			name += " - �޸���";
			SetWindowText(CString(name.c_str()));
			//3.2.12 �����Ѵ�.
			Invalidate(TRUE);
			//ĳ���� �ҷ��� note(����)�� �°� �ٽ� �������ش�.
			//3.2.13 CClientDC�� �����Ѵ�.
			CClientDC dc(this);
			//3.2.14 CFont�� �����Ѵ�.
			CFont font;
			//3.2.15 �۾�ũ��� �۾�ü�� ���ϴ�.
			font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
			//3.2.16 ��Ʈ�� dc�� �����Ѵ�.
			dc.SelectObject(font);
			//3.2.17 TEXTMETRIC�� �����Ѵ�.
			TEXTMETRIC text;
			//3.2.18 �۲��� ������ ��´�.
			dc.GetTextMetrics(&text);
			//3.2.19 ĳ���� �����Ѵ�.
			this->caret.Create(0, text.tmHeight);
			//3.2.20 �������� �ؽ�Ʈ���� �����Ѵ�.
			CString letter = CString(this->current->GetContent().c_str());
			//3.2.21 �������� �ؽ�Ʈ���� size�� ���Ѵ�.
			CSize letterSize = dc.GetTextExtent(letter);
			//3.2.22 ĳ���� �̵���Ų��.
			this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
			//3.2.23 ĳ���� ���̰� �Ѵ�.
			this->caret.Show();
		}
	}
}

//�޸��� �޴����� �����ϱ⸦ Ŭ������ ��
void NotepadForm::OnFileSaveClicked()
{
	//1. ���ϰ�ΰ� ""�� �ƴϸ�(��Ʈ�� ó�� ������ ��찡 �ƴϸ�)
	if (this->file.GetPath() != "")
	{
		//1.1 ������ �޸����� �����Ѵ�.
		this->file.Save(this->file.GetName(), this->file.GetPath());
		//1.2 �޸��� ������ �˾Ƴ���.
		CString noteTitle;
		GetWindowText(noteTitle);
		string title = string(noteTitle);
		//1.3 �޸��� ���� '*'�� ������(��������� ������)
		if (title[0] == '*')
		{
			//1.3.4 '*'�� �����ش�.
			title.erase(0, 1);
			SetWindowText(CString(title.c_str()));
		}
	}
	//2.2 ���ϰ�ΰ� ������ ���� ������(��Ʈ�� ó�� ������ ���)
	else
	{
		//2.2.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
		LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
		CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
		//2.2.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
		if (fileDialog.DoModal() == IDOK)
		{
			//2.2.2.1 ������ ������ �̸��� ���Ѵ�.
			CString fileName = fileDialog.GetFileName();
			//2.2.2.2 ������ ������ ��θ� ���Ѵ�.
			CString filePathName = fileDialog.GetPathName();
			//2.5 ������ �̸����� .txt�� �����Ѵ�.
			string name = string(fileName);
			Long eraseIndex = name.find(".txt");
			name.erase(eraseIndex, 4);
			//2.2.2.3 ������ �޸����� �����Ѵ�.
			this->file.Save(name, string(filePathName));
			//2.13 �޸��� ������ �ٲ۴�.
			name += " - �޸���";
			SetWindowText(CString(name.c_str()));
		}
	}	
}

//�޸��忡�� �ٸ� �̸����� �����ϱ� ��ư�� Ŭ������ ��
void NotepadForm::OnFileSaveDifferentNameClicked()
{
	//2.2.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
	LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
	CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this, 0, 1);
	//2.2.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
	if (fileDialog.DoModal() == IDOK)
	{
		//2.2.2.1 ������ ������ �̸��� ���Ѵ�.
		CString fileName = fileDialog.GetFileName();
		//2.2.2.2 ������ ������ ��θ� ���Ѵ�.
		CString filePathName = fileDialog.GetPathName();
		//2.5 ������ �̸����� .txt�� �����Ѵ�.
		string name = string(fileName);
		Long eraseIndex = name.find(".txt");
		name.erase(eraseIndex, 4);
		//2.2.2.3 ������ �޸����� �����Ѵ�.
		this->file.Save(name, string(filePathName));
		//2.13 �޸��� ������ �ٲ۴�.
		name += " - �޸���";
		SetWindowText(CString(name.c_str()));
	}
}
#endif

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
		//3.2 �޸����� �ݴ´�.
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