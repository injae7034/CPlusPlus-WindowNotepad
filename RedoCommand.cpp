#include "RedoCommand.h"
#include "NotepadForm.h"
#include "CommandHistory.h"

//����Ʈ ������ ����
RedoCommand::RedoCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void RedoCommand::Execute()
{
	this->notepadForm->commandHistory->Redo();
}

//�Ҹ��� ����
RedoCommand::~RedoCommand()
{

}