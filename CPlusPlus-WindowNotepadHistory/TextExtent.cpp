#include "TextExtent.h"
#include "NotepadForm.h"
#include "Glyph.h"

//����Ʈ������
TextExtent::TextExtent(NotepadForm* notepadForm)
{
	//1. notepadForm�� �ּҸ� �����Ѵ�.
	this->notepadForm = notepadForm;
	//2. CClientDC�� �����Ѵ�.
    CClientDC dc(this->notepadForm);
    //3. CFont�� �����Ѵ�.
    CFont font;
    //4. �۾�ũ��� �۾�ü�� ���ϴ�.
    font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
    //5. ��Ʈ�� dc�� �����Ѵ�.
    HFONT oldFont;
    oldFont = (HFONT)dc.SelectObject(font);
    //6. TEXTMETRIC�� �����Ѵ�.
    TEXTMETRIC textmetric;
    //7. �۲��� ������ ��´�.
    dc.GetTextMetrics(&textmetric);
    //8. 0~127���� �ƽ�Ű�ڵ�ǥ�� �ִ� ���ڵ��� ���� �ݺ��ؼ� widths �迭�� ���� �����Ѵ�.
    //(�ƽ�Ű�ڵ�ǥ�� ���ڵ��� ���� ���� ���� �ٸ��� ������ ���� ��������.)
    CString letter;
    CSize letterSize;
    //�ƽ�Ű�ڵ� 0��°�� NULL�̱� ������ NULL�� ���� ���ϸ� ���̳��� ������ 1���� ������.
    char i = 1;
    //char �ƽ�Ű�ڵ� ������ -128~127(256)�̹Ƿ� 127�� �Ѿ�� �ڵ����� -128�� �Ǳ⶧����
    //i>0 ������ ������ ���ѹݺ��� �Ǳ⶧���� ���ϴ� ���(�ƽ�Ű�ڵ� ������ �ݺ�)�� ��� ���ؼ�
    //i>0�� �ݵ�� �ʿ��ϴ�.
    while (i < 128 && i>0)
    {
        letter = i;
        letterSize = dc.GetTextExtent(letter);
        this->widths[i] = letterSize.cx;
        i++;
    }
    
    //9. 129��° �迭��ҿ� �ѱ��� ���� �����Ѵ�.(�ѱ��� ���� ���� ������ �� ���� ���� ��������.)
    //i�� �ִ� 127������ �̿밡���ϹǷ� �׳� ���� 128�� ����������!
    letter = "��";
    letterSize = dc.GetTextExtent(letter);
    this->widths[128] = letterSize.cx;
    letter = "��";
    letterSize = dc.GetTextExtent(letter);
    this->widths[129] = letterSize.cx;
    //10. i=0 ��°�� �����Ⱚ ��ſ� �����̽��� �����Ѵ�.
    //�ƽ�Ű�ڵ� 0��°�� null�̹Ƿ� ���� ����� �� ������ �����Ⱚ��� �����̽��� ��������.
    letter = " ";
    letterSize = dc.GetTextExtent(letter);
    this->widths[0] = letterSize.cx;
    //11. TabŰ�� ���̸� �������ش�.
    letter = "        ";
    letterSize = dc.GetTextExtent(letter);
    this->widths[9] = letterSize.cx;
    //12. ��� ��Ʈ ���̸� �����Ѵ�.
    this->height = textmetric.tmHeight;
    //13. ������.
    dc.SelectObject(oldFont);
    DeleteObject(font);
}

//GetTextWidth
Long TextExtent::GetTextWidth(string content)//count(ĳ���� ���� ��ġ)�� �ʿ����!
{
#if 0
    CString rowText = CString(this->notepadForm->current->
        GetPartOfContent(this->notepadForm->current->GetCurrent()).c_str());
    CString letter = CString(this->notepadForm->current->GetAt(i - 1)->GetContent().c_str());
#endif  
    //1. i�� length���� �������� �ݺ��Ѵ�.
    Long i = 0;
    Long width = 0;
    Long length = (Long)content.length();
    char letter[3];

    //�ѱ��� �迭��� 2���� �����ϰ� �ƽ�Ű�ڵ� ���ڴ� �迭��� 1���� �����ϱ� ������, count�� �ܺο���
    //�Է¹��� ����, string�� length�� ���ϸ� �迭����� ������ �˷��ֱ� ������ �̸� �̿�����!
    while (i < length)
    {
        letter[0] = content[i];
        letter[1] = '\0';
        //1.1 �ѱ��̸�
        if ((content[i] & 0x80))
        {
            letter[1] = content[i + 1];
            letter[2] = '\0';
            if (letter >= "��" && letter <= "��")
            {
                //1.1.1 �ѱ��� ���� �����ش�.
                width += this->widths[128];
            }
            else
            {
                width += this->widths[129];
            }
            i++;//�ѱ��� �迭��Ұ� 2���̱� ������ 1�� �����༭ 2���� �����ϰ� �Ѵ�.
        }
        //1.2 �ѱ��� �ƴϸ�(�ƽ�Ű�ڵ� �����̸�)
        //�ƽ�Ű�ڵ�� �迭��� 1���� �����ϱ� ������ ���� +1�� �����ʿ䰡 ����!
        else
        {
            //���⼭ content[i]�� �ѱ��� �ƴ� �ƽ�Ű�ڵ��̱⶧���� ���ڷ� Ȱ���� �����ϴ�!
            //���� �ƽ�Ű�ڵ�� ���ϸ� ���� ã�� �ʿ���� �ٷ� �迭��ҿ� �����ϴ� ���ڷ� Ȱ�밡��!
            width += this->widths[content[i]];
#if 0
            //1.2.1 j�� 128���� �������� �׸��� con
            char j = 0;
            while (j < 128 && content[i] != j)
            {
                j++;
            }
            if (j < 128)
            {
                width += this->widths[j];
            }
#endif
        }
        i++;
    }
    return width;
}

//�Ҹ���
TextExtent::~TextExtent()
{

}