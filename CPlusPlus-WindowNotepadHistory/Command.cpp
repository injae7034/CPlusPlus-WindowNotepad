#include "Command.h"

//����Ʈ������
Command::Command(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Command���� ����
void Command::Execute()
{

}

//Command���� �������
void Command::Unexecute()
{

}

//������� ��ũ�� ������������ �ƴ��� Ȯ��
bool Command::IsUndoMacroEnd()
{
	return -1;
}

//�ٽý��� ��ũ�� ������������ �ƴ��� Ȯ��
bool Command::IsRedoMacroEnd()
{
	return -1;
}

//������� ��ũ�� �������� ����
void Command::SetUndoMacroEnd()
{

}

//�ٽýǷ� ��ũ�� �������� ����
void Command::SetRedoMacroEnd()
{

}

//�ٽý����̶�� ǥ��
void Command::SetRedone()
{
	
}

//�ٽ� �������� �ƴ��� ���� Ȯ��
bool Command::IsRedone()
{
	return -1;
}

//��������� �ִ��� ���� Ȯ��
bool Command::IsDirty()
{
	return -1;
}

//���ÿ����� �������� ���� Ȯ��
bool Command::IsSelectedTextsRemoved()
{
	return -1;
}

//�����ϴ� ���� ��ġ���ϱ�
Long Command::GetStartPasteRowIndex()
{
	return - 1;
}

//�����ϴ� ������ ��ġ���ϱ�
Long Command::GetStartPasteLetterIndex()
{
	return -1;
}

//���� ��ġ���ϱ�
Long Command::GetRowIndex()
{
	return -1;
}

//���� ��ġ���ϱ�
Long Command::GetLetterIndex()
{
	return -1;
}

//�����ϴ� ������ ��ġ���ϱ�
Long Command::GetStartXPos()
{
	return -1;
}
//�����ϴ� ���� ��ġ���ϱ�
Long Command::GetStartYPos()
{
	return -1;
}
//������ ������ ���� ��ġ ���ϱ�
Long Command::GetSelectedEndYPos()
{
	return -1;
}
//������ ������ ���� ��ġ ���ϱ�
Long Command::GetSelectedEndXPos()
{
	return -1;
}
//�ٿ��ֱⰡ ������ ���� ��ġ���ϱ�
Long Command::GetPastingEndYPos()
{
	return -1;
}
//�ٿ��ֱⰡ ������ ������ ��ġ���ϱ�
Long Command::GetPastingEndXPos()
{
	return -1;
}

//�Ҹ���
Command::~Command()
{

}