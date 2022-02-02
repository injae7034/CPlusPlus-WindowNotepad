#include "Command.h"

//디폴트생성자
Command::Command(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Command패턴
void Command::Execute()
{

}

//소멸자
Command::~Command()
{

}