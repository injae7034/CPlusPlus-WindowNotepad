#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"
//class NotepadForm;

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, UINT nChar,
		Long rowIndex = 0, Long letterIndex = 0);//디폴트생성자
	virtual void Execute();
	virtual void Unexecute();
	virtual ~OnCharCommand();//소멸자
	//인라인함수 선언
	UINT GetNChar() const;
	Long GetRowIndex() const;
	Long GetLetterIndex() const;
private:
	UINT nChar;
	Long rowIndex;
	Long letterIndex;
	
};

//인라인함수 정의
inline UINT OnCharCommand::GetNChar() const
{
	return this->nChar;
}
inline Long OnCharCommand::GetRowIndex() const
{
	return this->rowIndex;
}
inline Long OnCharCommand::GetLetterIndex() const
{
	return this->letterIndex;
}

#endif // !_ONCHARCOMMAND_H