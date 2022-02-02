#include "Caret.h"
#include "CaretController.h"
#include "NotepadForm.h"


//����Ʈ������(ĳ���� �����ϸ鼭 notepadForm�̶� ����α�)
Caret::Caret(CaretController* caretController)
{
	this->caretController = caretController;
}

//���������
Caret::Caret(const Caret& source)
{
	this->caretController = source.caretController;
}

//ġȯ������
Caret& Caret::operator=(const Caret& source)
{
	this->caretController = source.caretController;
	
	return *this;
}

//�Ҹ���
Caret::~Caret()
{
	//3. �޸����� ĳ���� �����.
	this->caretController->notepadForm->HideCaret();
	//4. �޸����� ĳ���� ���ش�.
	::DestroyCaret();
}

//ĳ�������ϱ�
void Caret::Create(Long height, Long width)
{
	//��ġ����� �ۿ��� �ϱ�
	this->caretController->notepadForm->CreateSolidCaret(height, width);
}

//ĳ���̵��ϱ�
void Caret::Move(Long x, Long y)
{
	this->caretController->notepadForm->SetCaretPos(CPoint(x, y));
}

//ĳ�� �����ֱ�
void Caret::Show()
{
	this->caretController->notepadForm->ShowCaret();
}
