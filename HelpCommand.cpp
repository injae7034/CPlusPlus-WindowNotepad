#include "HelpCommand.h"
#include "NotepadForm.h"
#include <htmlhelp.h>
#pragma comment(lib, "htmlhelp")

HelpCommand::HelpCommand(NotepadForm * notepadForm)
{
	this->notepadForm = notepadForm;
}

HelpCommand::~HelpCommand() 
{

}

void HelpCommand::Execute() 
{
	HWND hwnd = HtmlHelpA(GetDesktopWindow(), "메모장도움말.chm", HH_DISPLAY_TOPIC, NULL);
}