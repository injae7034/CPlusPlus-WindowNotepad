#include "NotepadForm.h"
#include "GlyphCreator.h"
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
	ON_COMMAND(IDM_FILE_OPEN, OnFileOpenClicked)
	ON_COMMAND(IDM_FILE_SAVE, OnFileSaveClicked)
	ON_COMMAND(IDM_FILE_SAVEDIFFERENTNAME, OnFileSaveDifferentNameClicked)
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
	//CFont���� ����ϰ��� �ϴ� ����ũ��� ����ü�� �ʱ�ȭ��Ŵ.
	//�⺻�����ڷ� ������ this->font�� �Ű����� 2�������ڷ� ġȯ(=)��Ŵ
	this->font = Font(300, "�ü�ü");
	//�⺻�����ڷ� ������ this->caret�� �Ű����� 1�������ڷ� ġȯ(=)��Ŵ.
	this->caret = Caret(this);
	this->file = File(this);
}
#if 0
//NotepadForm�Ҹ���
NotepadForm::~NotepadForm()
{

}
#endif
//1. �޸��� �����찡 ������ ��
int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//1.1 CClientDC�� �����Ѵ�.
	//CClientDC dc(this);
	//1.2 CFont�� �����Ѵ�.
	//CFont font;
	//1.3 �۾�ũ��� �۾�ü�� ���ϴ�.
	//font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//1.4 ��Ʈ�� dc�� �����Ѵ�.
	//dc.SelectObject(font);
	//1.5 TEXTMETRIC�� �����Ѵ�.
	//TEXTMETRIC text;
	//1.6 �۲��� ������ ��´�.
	//dc.GetTextMetrics(&text);
	//1.7 ĳ���� �����Ѵ�.
	//this->caret.Create(0, text.tmHeight);
	//1.8 glyphCreator�� �����.
	GlyphCreator glyphCreator;
	//1.9 ��Ʈ�� �����.
	this->note = glyphCreator.Create((char*)"\0");
	//1.10 ���� �����.
	Glyph* row = glyphCreator.Create((char*)"\n");
	//1.11 ���� �޸��忡 �߰��Ѵ�.
	Long rowIndex;
	rowIndex = this->note->Add(row);
	//1.13 ���� ���� ��ġ�� �����Ѵ�.
	this->current = this->note->GetAt(rowIndex);
	
	//1.14 ĳ���� ��ġ�� �̵���Ų��.
	//this->caret.Move(0, 0);
	//1.15 ĳ���� �����ش�.
	//this->caret.Show();
	//m_wndStatusBar.Create(this);
	//Menu�� notepadForm�� ������� OnCreate�� ����Ǿ ������ ��� ����Ǿ� �ֱ⶧���� ���� �ȳ�
	//���࿡ �ӽú����� OnCreate������ Menu�� ������ OnCreate�� ����ɶ� Menu�� ��������� ���� �Ҹ��
	this->menu.LoadMenu(IDR_MENU1);
	SetMenu(&this->menu);
	//ó�� ��������� �޸��� �̸��� ���Ѵ�.
	string name = this->file.GetName();
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));

	return 0;
}

//2. Ű���忡 ���ڸ� �Է��� ��
void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//2.1 CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//2.2 glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//2.3 glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&nChar);
	Long index;
	//CPoint caretPosition;
	//2.4 CFont�� �����Ѵ�.
	CFont font;
	//2.5 �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//2.6 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//2.7 TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//2.8 �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//2.9 ĳ���� �����Ѵ�.
	//CreateSolidCaret(0, text.tmHeight);
	this->caret.Create(0, text.tmHeight);
	//2.10 �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (nChar != '\n' && nChar != '\r')
	{
		//2.10.1 ���� �ٿ� ���ڸ� �߰��Ѵ�.
		index = this->current->Add(glyph);
		//2.10.2 �߰��� ������ �ʺ� ���Ѵ�.
		CString letter = CString(this->current->GetContent().c_str());
		CSize letterSize = dc.GetTextExtent(letter);
		//2.10.3 ĳ���� �̵��� ��ġ�� ���Ѵ�.
		//caretPosition = CPoint(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
		//2.10.3 ĳ���� �̵���Ų��.
		this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//2.11 �Է¹��� ���ڰ� ���๮���̸�
	else if (nChar == '\n' || nChar == '\r')
	{
		//2.11.1 ���ο� ���� �߰��Ѵ�.
		index = this->note->Add(glyph);
		//2.11.2 ���� ���� ��ġ�� ���� �����Ѵ�.
		this->current = this->note->GetAt(index);
		//2.11.3 ĳ���� �̵��� ��ġ�� ���Ѵ�.
		//caretPosition = CPoint(0, (this->note->GetCurrent() - 1) * text.tmHeight);
		//2.11.3 ĳ���� �̵���Ų��.
		this->caret.Move(0, (this->note->GetCurrent() - 1) * text.tmHeight);
	}
	//2.12 ĳ���� ��ġ�� �̵��Ѵ�.
	//this->SetCaretPos(caretPosition);
	//2.13 ĳ���� ���̰� �Ѵ�.
	this->caret.Show();
	//ShowCaret();
	//2.14 isComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//2.15 �޸��� ���� *�� �߰��Ѵ�.
	string name = this->file.GetName();
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//2.16 �����Ѵ�.
	Invalidate(TRUE);
}

//3. �޸��忡 �ؽ�Ʈ�� ����� ��
void NotepadForm::OnPaint()
{
	//3.1 CPaintDC�� �����Ѵ�.
	CPaintDC dc(this);
	//3.2 �ؽ�Ʈ�� ����� �����ϰ���.
	dc.SetBkMode(TRANSPARENT);
	//3.3 �ؽ�Ʈ�� ������ ����.
	dc.SetTextColor(RGB(0, 0, 0));
	//3.4 ������ ���ؼ����� ����.
	dc.SetTextAlign(TA_LEFT);
	//3.5 CFont�� �����Ѵ�.
	CFont font;
	//3.6 �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//3.7 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//3.8 TEXTMETRIC�� �����Ѵ�.
	TEXTMETRIC text;
	//3.9 �۲��� ������ ��´�.
	dc.GetTextMetrics(&text);
	//3.10 note�� ����� ���ڵ��� ����Ѵ�.
	Long i = 0;
	CString content;
	//�ٴ����� �ݺ������� ���ؼ� ���� ������ �ٰ�����ŭ ����ϵ��� ��.
	while (i < this->note->GetLength())
	{
		content = CString(this->note->GetAt(i)->GetContent().c_str());
		//�ؽ�Ʈ ���� ��ġ���� ó������ (0,0)���� �����ϰ� �ι�°���� (0, 150)���� ������.
		dc.TextOut(0, i * text.tmHeight, content);
		i++;
	}
#if 0
	//��Ʈ�� �״�� ����ϸ� ���� ������ �ȵǰ� ���ٿ� ������ �ٳ���.
	content = CString(this->note->GetContent().c_str());
	dc.TextOut(0, i * 150, content);
#endif

#if 0
	//2.8 Note�� ����� Content�� �ҷ��´�.
	CString content = CString(this->note->GetContent().c_str());
	//2.9 Content�� ����Ѵ�.
	dc.TextOut(0, 0, _T(this->note->GetContent().c_str()));
#endif
}

//4. �ѱ��� �Է¹��� ��
LRESULT NotepadForm::OnComposition(WPARAM wParam, LPARAM lParam)
{
	//4.1 CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//4.2 glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4.3 doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//4.4 IsComposing���� '��'�̸�
	if (this->IsComposing == true)
	{
		//4.4.1 ���� ���� ���� �ѱ��� �����.
		this->current->Remove(this->current->GetLength() - 1);
	}
	//4.5 isComposing���� '����'�̸�
	else
	{
		//4.5.1 isComposing���� '��'���� �ٲ۴�.
		this->IsComposing = true;
	}
	//4.6 ���� ���� DoubleByteLetter�� ���� �ٿ� �߰��Ѵ�.
	Long letterIndex = this->current->Add(doubleByteLetter);
	//4.7 CFont�� �����Ѵ�.
	CFont font;
	//4.8 �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//4.9 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//4.10 �߰��� ���ڸ� ���� ��ü �ؽ�Ʈ�� size�� ���Ѵ�.
	CString text = CString(this->current->GetContent().c_str());
	CSize textSize = dc.GetTextExtent(text);
	//4.11 �߰��� �ѱ��� size�� ���Ѵ�.
	CString letter = CString(this->current->GetAt(letterIndex)->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//4.12 ĳ���� �����Ѵ�.
	TEXTMETRIC koreanCaret;
	dc.GetTextMetrics(&koreanCaret);
	this->caret.Create(letterSize.cx, koreanCaret.tmHeight);
	//CreateSolidCaret(letterSize.cx, koreanCaret.tmHeight);
	//4.13 ĳ���� �̵��� ��ġ�� ���Ѵ�.
	//CPoint caretPosition = CPoint(textSize.cx - letterSize.cx, (this->note->GetCurrent() - 1) * koreanCaret.tmHeight);
	//4.14 ĳ���� �̵���Ų��.
	this->caret.Move(textSize.cx - letterSize.cx, (this->note->GetCurrent() - 1) * koreanCaret.tmHeight);
	//this->SetCaretPos(caretPosition);
	//4.15 ĳ���� ���̰� �Ѵ�.
	this->caret.Show();
	//ShowCaret();
	//4.16 �޸��� ���� *�� �߰��Ѵ�.
	string name = this->file.GetName();
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//4.17 �����Ѵ�.
	Invalidate(TRUE);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

//5. �ϼ��� �ѱ��� �Է¹޾��� ��
LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam)
{
	//5.1 CClientDC�� �����Ѵ�.
	CClientDC dc(this);
	//5.2 CFont�� �����Ѵ�.
	CFont font;
	//5.3 �۾�ũ��� �۾�ü�� ���ϴ�.
	font.CreatePointFont(this->font.Getsize(), this->font.GetStyle().c_str());
	//5.4 ��Ʈ�� dc�� �����Ѵ�.
	dc.SelectObject(font);
	//5.5 glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//5.6 doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//5.7 ������ �������̴� �ѱ��� �����.
	this->current->Remove(this->current->GetLength() - 1);
	//5.8 �����ٿ� �ϼ��� �ѱ��� �߰��Ѵ�.
	this->current->Add(doubleByteLetter);
	//5.9 IsComposing�� false�� �ٲ۴�.
	this->IsComposing = false;
	//5.10 ĳ���� �����Ѵ�
	TEXTMETRIC text;
	dc.GetTextMetrics(&text);
	this->caret.Create(0, text.tmHeight);
	//CreateSolidCaret(0, text.tmHeight);
	//5.11 ĳ���� �̵��� ��ġ�� ���Ѵ�.
	CString letter = CString(this->current->GetContent().c_str());
	CSize letterSize = dc.GetTextExtent(letter);
	//CPoint caretPosition = CPoint(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//5.12 ĳ���� �̵���Ų��.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//SetCaretPos(caretPosition);
	//5.13 ĳ���� �����ش�.
	this->caret.Show();
	//ShowCaret();
	//5.14 �޸��� ���� *�� �߰��Ѵ�.
	string name = this->file.GetName();
	name.insert(0, "*");
	name += " - �޸���";
	SetWindowText(CString(name.c_str()));
	//5.15 �����Ѵ�.
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
	//CreateSolidCaret(0, text.tmHeight);
	//8. �������� �ؽ�Ʈ���� �����Ѵ�.
	CString letter = CString(this->current->GetContent().c_str());
	//9. �������� �ؽ�Ʈ���� size�� ���Ѵ�.
	CSize letterSize = dc.GetTextExtent(letter);
	//10. ĳ���� �̵��� ��ġ�� ���Ѵ�.
	//CPoint caretPosition = CPoint(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//11. ĳ���� �̵���Ų��.
	this->caret.Move(letterSize.cx, (this->note->GetCurrent() - 1) * text.tmHeight);
	//SetCaretPos(caretPosition);
	//12. ĳ���� ���̰� �Ѵ�.
	this->caret.Show();
	//ShowCaret();

#if 0
	//Caret�� ��ġ ����

		CSize strSize;

	strSize = dc.GetTextExtent(str);

	pt.x = strSize.cx;

	pt.y = Line * txtKey.tmHeight;

	SetCaret(pt);

	ShowCaret();
#endif
}

//�޸����� Focus�� ���� ��
void NotepadForm::OnKillFocus(CWnd* pNewWnd)
{
	//1. ĳ���� �����.
	HideCaret();
	//2. ĳ���� �ı��Ѵ�.
	::DestroyCaret();
}

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

//�޸��忡�� �ݱ��ư�� Ŭ������ ��
void NotepadForm::OnClose()
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
			//4.1 �޸����� �����.
			if (this->note != NULL)
			{
				delete this->note;
				//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
				//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
				//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
				//delete this->current;
			}
			CFrameWnd::OnClose();
		}
	}
	//��������� ������(�޸��� ���� �տ� '*'�� ������)
	else
	{
		//9.1 �޸����� �����.
		if (this->note != NULL)
		{
			delete this->note;
			//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
			//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
			//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
			//delete this->current;
		}
		CFrameWnd::OnClose();
	}
}

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