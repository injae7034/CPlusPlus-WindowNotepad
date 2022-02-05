#include "UndoCommand.h"
#include "NotepadForm.h"
#include "CommandHistory.h"

//����Ʈ ������ ����
UndoCommand::UndoCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void UndoCommand::Execute()
{
	this->notepadForm->commandHistory->Undo();
}

//�Ҹ��� ����
UndoCommand::~UndoCommand()
{

}