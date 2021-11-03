#include "FileExitCommand.h"
#include "NotepadForm.h"

//디폴트 생성자 정의
FileExitCommand::FileExitCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void FileExitCommand::Execute()
{
	this->notepadForm->PostMessage(WM_CLOSE);
}

//소멸자 정의
FileExitCommand::~FileExitCommand()
{

}