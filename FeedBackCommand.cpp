#include "FeedBackCommand.h"
#include "NotepadForm.h"

FeedBackCommand::FeedBackCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

FeedBackCommand::~FeedBackCommand()
{
	
}

void FeedBackCommand::Execute()
{
	//13.2 이메일 주소를 적은 메세지박스를 출력한다.
	string message = "사용하시면서 개선하시고 싶은 불편한 점이나 문제점이 있으면\n"
		"injae7034@gmail.com으로 메일 보내주세요.";
	int messageBoxButton = this->notepadForm->MessageBox(message.c_str(), "메모장", MB_OK);
}