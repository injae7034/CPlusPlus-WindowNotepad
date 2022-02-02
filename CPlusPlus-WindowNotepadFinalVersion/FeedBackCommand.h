#ifndef _FEEDBACKCOMMAND_H
#define _FEEDBACKCOMMAND_H
#include "Command.h"

class FeedBackCommand : public Command {
public:
	FeedBackCommand(NotepadForm* notepadForm = 0);
	virtual ~FeedBackCommand();
	virtual void Execute();
};

#endif // _FEEDBACKCOMMAND_H