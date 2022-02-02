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
	HWND hwnd = HtmlHelpA(GetDesktopWindow(), "�޸��嵵��.chm", HH_DISPLAY_TOPIC, NULL);
}