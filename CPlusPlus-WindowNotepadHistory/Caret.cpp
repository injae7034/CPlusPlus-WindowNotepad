#include "Caret.h"
#include "CaretManager.h"
#include "NotepadForm.h"


//����Ʈ������(ĳ���� �����ϸ鼭 notepadForm�̶� ����α�)
Caret::Caret(CaretManager* caretManager)
{
	this->caretManager = caretManager;
}

//���������
Caret::Caret(const Caret& source)
{
	this->caretManager = source.caretManager;
}

//ġȯ������
Caret& Caret::operator=(const Caret& source)
{
	this->caretManager = source.caretManager;
	
	return *this;
}

//�Ҹ���
Caret::~Caret()
{
	//3. �޸����� ĳ���� �����.
	this->caretManager->notepadForm->HideCaret();
	//4. �޸����� ĳ���� ���ش�.
	::DestroyCaret();
}

//ĳ�������ϱ�
void Caret::Create(Long height, Long width)
{
	//��ġ����� �ۿ��� �ϱ�
	this->caretManager->notepadForm->CreateSolidCaret(height, width);
}

//ĳ���̵��ϱ�
void Caret::Move(Long x, Long y)
{
	this->caretManager->notepadForm->SetCaretPos(CPoint(x, y));
}

//ĳ�� �����ֱ�
void Caret::Show()
{
	this->caretManager->notepadForm->ShowCaret();
}
