#include "FindCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일

//디폴트생성자
FindCommand::FindCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void FindCommand::Execute()
{
	
}

//소멸자
FindCommand::~FindCommand()
{

}