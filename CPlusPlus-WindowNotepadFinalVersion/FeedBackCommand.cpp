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
	//13.2 �̸��� �ּҸ� ���� �޼����ڽ��� ����Ѵ�.
	string message = "����Ͻø鼭 �����Ͻð� ���� ������ ���̳� �������� ������\n"
		"injae7034@gmail.com���� ���� �����ּ���.";
	int messageBoxButton = this->notepadForm->MessageBox(message.c_str(), "�޸���", MB_OK);
}