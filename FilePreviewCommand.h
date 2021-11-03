#ifndef _FILEPREVIEWCOMMAND_H
#define _FILEPREVIEWCOMMAND_H

#include "Command.h"

class NotepadForm;

class FilePreviewCommand : public Command
{
public:
	FilePreviewCommand(NotepadForm* notepadForm = 0);
	void Execute();
	~FilePreviewCommand();//¼Ò¸êÀÚ
};

#endif // !_FILEPREVIEWCOMMAND_H


