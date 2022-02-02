#include "FontChangeCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog�������

//����Ʈ������
FontChangeCommand::FontChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Command����
void FontChangeCommand::Execute()
{
    //1. this->notepadForm->font�� �۲��� �������� fontDialog�� �����Ѵ�.
    CFontDialog fontDialog(&this->notepadForm->font.GetLogFont(),
        CF_EFFECTS | CF_SCREENFONTS, NULL, NULL);
    COLORREF selectedColor;
    LOGFONT logFont;
    //logFont = LOGFONT(10, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 0, this->notepadForm->GetFont().GetFaceName());
    //2. this->notepadForm->font�� ������ �ݿ��� �ȵǾ��ֱ� ������
    //fontDialog ��ȭ���ڸ� ����ϱ����� this->notepadForm->font�� ������ �ݿ��Ѵ�.
    //m_cf�� Dialog�����ڿ��� ��ó �������� ���� �͵��� �����ϰ� ������ �� �ֵ��� ������!
    fontDialog.m_cf.rgbColors = this->notepadForm->font.GetColor();
    //3. this->notepadForm->font�� �۲ð� ���� �������� fontDialog�� ����Ѵ�.
    if (fontDialog.DoModal() == IDOK)
    {
        //���� Ŭ�������� �ٸ� Ŭ���� ����� ������ ���ö��� �ζ����Լ� Get�� �̿��ϸ��
        //���� Ŭ�������� �ٸ� Ŭ���� ����� ������ �����Ҷ��� �� �ٸ� Ŭ������ �����ڸ� ȣ���ؼ�
        //���ϴ� �������� �����Ű�� ��!!!
        //NotepadForm�� Font�� ����� ������ �����ϰ� ������ ���� ���⼭
        //NotepadForm�� Font�� �����ڸ� ȣ���ؼ� ������ �����ϸ� �ȴ�.
        //�ּҷ��� Correct���� ����� ������ ����!!!
        //2.1 ��Ʈ��ȭ���ڿ��� ������ �۲��� ������ ��´�.
        fontDialog.GetCurrentFont(&logFont);
        //2.2 ��Ʈ��ȭ���ڿ��� ������ �۲��� ���� ��´�.
        selectedColor = fontDialog.GetColor();
        //2.3 notepadForm�� font�� ������ �۲��� ������ �����Ѵ�.
        this->notepadForm->font = Font(logFont, selectedColor);
        //2.4 ĳ���� �ٽ� �����Ѵ�.
        //2.4.1 CClientDC�� �����Ѵ�.
        CClientDC dc(this->notepadForm);
        //2.4.2 CFont�� �����Ѵ�.
        CFont font;
        //2.4.3 �۾�ũ��� �۾�ü�� ���ϴ�.
        //font.CreatePointFont(this->notepadForm->GetFont().GetSize(), this->notepadForm->GetFont().GetFaceName().c_str());
        font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
        //2.4.5 ��Ʈ�� dc�� �����Ѵ�.
        dc.SelectObject(font);
        //2.4.6 TEXTMETRIC�� �����Ѵ�.
        TEXTMETRIC text;
        //2.4.7 �۲��� ������ ��´�.
        dc.GetTextMetrics(&text);
        //2.4.8 ĳ���� �����Ѵ�.
        this->notepadForm->GetCaret().Create(0, text.tmHeight);
        //2.4.9 �������� �ؽ�Ʈ���� �����Ѵ�.
        CString letter = CString(this->notepadForm->current->GetContent().c_str());
        //2.4.10 �������� �ؽ�Ʈ���� size�� ���Ѵ�.
        CSize letterSize = dc.GetTextExtent(letter);
        //2.4.11 ĳ���� �̵���Ų��.
        this->notepadForm->GetCaret().Move(letterSize.cx, (this->notepadForm->note->GetCurrent() - 1)* text.tmHeight);
        //2.4.12 ĳ���� ���̰� �Ѵ�.
        this->notepadForm->GetCaret().Show();
        //2.4.13 ��������� �����Ѵ�.
        this->notepadForm->Invalidate(TRUE);
    }
}

//�Ҹ���
FontChangeCommand::~FontChangeCommand()
{

}