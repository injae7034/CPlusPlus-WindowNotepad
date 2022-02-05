#include "CommandCreator.h"
#include "FileOpenCommand.h"
#include "FileSaveCommand.h"
#include "FileSaveDifferentNameCommand.h"
#include "FontChangeCommand.h"
#include "RowAutoChangeCommand.h"
#include "CopyCommand.h"
#include "PasteCommand.h"
#include "CutCommand.h"
#include "SelectAllCommand.h"
#include "RemoveCommand.h"
#include "FindCommand.h"
#include "ReplaceCommand.h"
#include "OnCharCommand.h"
#include "UndoCommand.h"
#include "RedoCommand.h"

//����Ʈ ������
CommandCreator::CommandCreator(NotepadForm *notepadForm)
{
	this->notepadForm = notepadForm;
}

//���丮 �޼ҵ� ����
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
	else if (nId == IDM_NOTE_SELECTALL)
	{
		command = new SelectAllCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_REMOVE)
	{
		command = new RemoveCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_FIND)
	{
		command = new FindCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_REPLACE)
	{
		command = new ReplaceCommand(this->notepadForm);
	}
	else if (nId == ID_ONCHARCOMMAND)
	{
		command = new OnCharCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_UNDO)
	{
		command = new UndoCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_REDO)
	{
		command = new RedoCommand(this->notepadForm);
	}

	return command;
}

//�Ҹ���
CommandCreator::~CommandCreator()
{

}