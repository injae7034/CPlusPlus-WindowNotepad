#include "Caret.h"
#include "NotepadForm.h"


//����Ʈ������(ĳ���� �����ϸ鼭 notepadForm�̶� ����α�)
Caret::Caret(NotepadForm* notepadForm)
{
#if 0
	//1.notepadForm�� �ּҸ� �Է¹޴´�.
	//2. notepadForm�� dc�� ���Ѵ�.
	//CDC* dc = notepadForm->GetDC();//���ο� DC�� ������. DC�� �ϳ��� �����쿡���� �������ϼ� ����
	//3. notepadForm�� �۲� ������ ��´�. //�۲��� ������ ��� ������ ���⼭ CFont�� �����������
	//TEXTMETRIC text; //�ۿ��� �۲ü����ϰ� ���⼭ GetDC�غ��� ���ο� DC�� �����Ǳ� ������ �ǹ̾���.
	//GetMetrics�� �۲��� ������ ��� ������ ���⼭ ���� CFont�� �����������.
	//dc->GetTextMetrics(&text);
#endif
	this->notepadForm = notepadForm;
	//5. notepadForm �۲��� ������ Caret�� ����� �����Ѵ�.
	//this->height = text.tmHeight;
	//this->height = 0;
	//this->width = 0;
}

#if 0
//�Ű�����3��������
Caret::Caret(NotepadForm* notepadForm, Long hegiht, Long width)
{
	this->notepadForm = notepadForm;
	this->height = height;
	this->width = width;
}
#endif


//���������
Caret::Caret(const Caret& source)
{
	this->notepadForm = source.notepadForm;
	//this->height = source.height;
	//this->width = source.width;
}

//ġȯ������
Caret& Caret::operator=(const Caret& source)
{
	this->notepadForm = source.notepadForm;
	//this->height = source.height;
	//this->width = source.width;
	
	return *this;
}

//�Ҹ���
Caret::~Caret()
{

}

//ĳ�������ϱ�
void Caret::Create(Long height, Long width)
{
	//��ġ����� �ۿ��� �ϱ�
#if 0
	//2. notepadForm�� dc�� ���Ѵ�.
	CDC *dc = this->notepadForm->GetDC();
	//3. notepadForm�� �۲� ������ ��´�.
	TEXTMETRIC text;
	dc->GetTextMetrics(&text);
	//4. notepadForm�� �ѱ� �������� �ƴϸ�
	if (notepadForm->GetIsComposing() == false)
	{
		//4.1 ĳ���� ���̸� 0���� �����Ѵ�.
		notepadForm->CreateSolidCaret(0, text.tmHeight)
	}
#endif
	//this->height = height;
	//this->width = width;
	this->notepadForm->CreateSolidCaret(height, width);
}

//ĳ���̵��ϱ�
void Caret::Move(Long x, Long y)
{
	this->notepadForm->SetCaretPos(CPoint(x, y));
}

//ĳ�� �����ֱ�
void Caret::Show()
{
	this->notepadForm->ShowCaret();
}
