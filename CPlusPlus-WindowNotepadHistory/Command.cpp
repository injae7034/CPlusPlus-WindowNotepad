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

//�� ��ġ �����ϱ�
void Command::SetRowIndex(Long rowIndex)
{

}

//���� ��ġ �����ϱ�
void Command::SetLetterIndex(Long letterIndex)
{

}

//�Ҹ���
Command::~Command()
{

}