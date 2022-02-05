#include "UndoCommand.h"
#include "NotepadForm.h"
#include "CommandHistory.h"

//디폴트 생성자 정의
UndoCommand::UndoCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void UndoCommand::Execute()
{
	this->notepadForm->commandHistory->Undo();
}

//소멸자 정의
UndoCommand::~UndoCommand()
{

}