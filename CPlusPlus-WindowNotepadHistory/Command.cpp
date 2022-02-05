#include "Command.h"

//디폴트생성자
Command::Command(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Command패턴 실행
void Command::Execute()
{

}

//Command패턴 실행취소
void Command::Unexecute()
{

}

//소멸자
Command::~Command()
{

}