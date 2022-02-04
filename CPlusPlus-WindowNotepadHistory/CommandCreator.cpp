#include "CommandCreator.h"
#include "FileOpenCommand.h"
#include "FileSaveCommand.h"
#include "FileSaveDifferentNameCommand.h"
#include "FontChangeCommand.h"
#include "RowAutoChangeCommand.h"
#include "CopyCommand.h"
#include "PasteCommand.h"
#include "CutCommand.h"

//디폴트 생성자
CommandCreator::CommandCreator(NotepadForm *notepadForm)
{
	this->notepadForm = notepadForm;
}

//팩토리 메소드 패턴
Command* CommandCreator::Create(UINT nId)
{
	Command* command = 0;

	if (nId == IDM_FILE_OPEN)
	{
		command = new FileOpenCommand(this->notepadForm);
	}
	else if (nId == IDM_FILE_SAVE)
	{
		command = new FileSaveCommand(this->notepadForm);
	}
	else if (nId == IDM_FILE_SAVEDIFFERENTNAME)
	{
		command = new FileSaveDifferentNameCommand(this->notepadForm);
	}
	else if (nId == IDM_FONT_CHANGE)
	{
		command = new FontChangeCommand(this->notepadForm);
	}
	else if (nId == IDM_ROW_AUTOCHANGE)
	{
		command = new RowAutoChangeCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_COPY)
	{
		command = new CopyCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_PASTE)
	{
		command = new PasteCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_CUT)
	{
		command = new CutCommand(this->notepadForm);
	}

	return command;
}

//소멸자
CommandCreator::~CommandCreator()
{

}