#include "Caret.h"
#include "NotepadForm.h"


//����Ʈ������(ĳ���� �����ϸ鼭 notepadForm�̶� ����α�)
Caret::Caret(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//���������
Caret::Caret(const Caret& source)
{
	this->notepadForm = source.notepadForm;
}

//ġȯ������
Caret& Caret::operator=(const Caret& source)
{
	this->notepadForm = source.notepadForm;
	
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
