#include "RedoCommand.h"
#include "NotepadForm.h"
#include "CommandHistory.h"

//디폴트 생성자 정의
RedoCommand::RedoCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void RedoCommand::Execute()
{
	this->notepadForm->commandHistory->Redo();
}

//소멸자 정의
RedoCommand::~RedoCommand()
{

}