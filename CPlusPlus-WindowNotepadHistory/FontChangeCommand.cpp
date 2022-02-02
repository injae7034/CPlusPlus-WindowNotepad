#include "FontChangeCommand.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "afxdlgs.h"//CFileDialog�������

//����Ʈ������
FontChangeCommand::FontChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

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
        //private����� ���뿡 �����ϰ� ������ �ζ����Լ�(Get)�� �̿��ϰ�
        //private����� ������ �����ϰ� ������ �����ڸ� �̿��Ѵ�!
        this->notepadForm->font = Font(logFont, selectedColor);
        //2.4 ���� �۲� ������ ������ �ִ� TextExtent�� �Ҵ�����(�Ҹ�)���ش�.
        if (this->notepadForm->textExtent != NULL)
        {
            delete this->notepadForm->textExtent;
        }
        //2.5 ���� ������ �۲� ������ �ݿ��� TextExtent�� ���� �Ҵ�(����)�Ѵ�.
        this->notepadForm->textExtent = new TextExtent(this->notepadForm);
        //2.6 ĳ���� ũ��� ��ġ�� ����Ǿ����� �˸���.
        this->notepadForm->Notify();
        //2.7 ��������� �����Ѵ�.
        this->notepadForm->Invalidate(TRUE);
    }
}

//�Ҹ���
FontChangeCommand::~FontChangeCommand()
{

}