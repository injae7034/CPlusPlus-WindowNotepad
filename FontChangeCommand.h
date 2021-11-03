#ifndef _FONTCHANGECOMMAND_H
#define _FONTCHANGECOMMAND_H

#include "Command.h"

class NotepadForm;

class FontChangeCommand : public Command
{
public:
	FontChangeCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FontChangeCommand();//¼Ò¸êÀÚ
};

#endif // !_FONTCHANGECOMMAND_H


