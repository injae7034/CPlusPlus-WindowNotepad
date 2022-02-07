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
#include "Note.h"
#include "OnImeCharCommand.h"
#include "BackSpaceKeyActionCommand.h"
#include "DeleteKeyActionCommand.h"
#include "CtrlBackSpaceKeyActionCommand.h"
#include "CtrlDeleteKeyActionCommand.h"
#include "ShiftCtrlBackSpaceKeyActionCommand.h"
#include "ShiftCtrlDeleteKeyActionCommand.h"
#include "OnReplaceButtonClickedCommand.h"
#include "OnReplaceAllButtonClickedCommand.h"
#include "FilePrintingCommand.h"
#include "FileSetupCommand.h"
#include "FilePreviewCommand.h"

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
		command = new OnCharCommand(this->notepadForm, this->notepadForm->GetGlyph());
	}
	else if (nId == IDM_NOTE_UNDO)
	{
		command = new UndoCommand(this->notepadForm);
	}
	else if (nId == IDM_NOTE_REDO)
	{
		command = new RedoCommand(this->notepadForm);
	}
	else if (nId == ID_ONIMECHARCOMMAND)
	{
		command = new OnImeCharCommand(this->notepadForm, this->notepadForm->GetGlyph());
	}
	else if (nId == ID_BACKSPACEKEYACTIONCOMMAND)
	{
		command = new BackSpaceKeyActionCommand(this->notepadForm);
	}
	else if (nId == ID_DELETEKEYACTIONCOMMAND)
	{
		command = new DeleteKeyActionCommand(this->notepadForm);
	}
	else if (nId == ID_CTRLBACKSPACEKEYACTIONCOMMAND)
	{
		command = new CtrlBackSpaceKeyActionCommand(this->notepadForm);
	}
	else if (nId == ID_CTRLDELETEKEYACTIONCOMMAND)
	{
		command = new CtrlDeleteKeyActionCommand(this->notepadForm);
	}
	else if (nId == ID_SHIFTCTRLBACKSPACEKEYACTIONCOMMAND)
	{
		command = new ShiftCtrlBackSpaceKeyActionCommand(this->notepadForm);
	}
	else if (nId == ID_SHIFTCTRLDELETEKEYACTIONCOMMAND)
	{
		command = new ShiftCtrlDeleteKeyActionCommand(this->notepadForm);
	}
	else if (nId == ID_ONREPLACEBUTTONCLICKEDCOMMAND)
	{
		command = new OnReplaceButtonClickedCommand(this->notepadForm);
	}
	else if (nId == ID_ONREPLACEALLBUTTONCLICKEDCOMMAND)
	{
		command = new OnReplaceAllButtonClickedCommand(this->notepadForm);
	}
	else if (nId == IDM_FILE_PRINT)
	{
		command = new FilePrintingCommand(this->notepadForm);
	}
	else if (nId == IDM_FILE_SETUP)
	{
		command = new FileSetupCommand(this->notepadForm);
	}
	else if (nId == IDM_FILE_PREVIEW)
	{
		command = new FilePreviewCommand(this->notepadForm);
	}

	return command;
}

//소멸자
CommandCreator::~CommandCreator()
{

}