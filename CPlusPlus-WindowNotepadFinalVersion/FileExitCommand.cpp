#include "FileExitCommand.h"
#include "NotepadForm.h"

//����Ʈ ������ ����
FileExitCommand::FileExitCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void FileExitCommand::Execute()
{
	this->notepadForm->PostMessage(WM_CLOSE);
}

//�Ҹ��� ����
FileExitCommand::~FileExitCommand()
{

}