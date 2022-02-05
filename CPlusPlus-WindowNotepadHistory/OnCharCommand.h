#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"
//class NotepadForm;

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, UINT nChar,
		Long rowIndex = 0, Long letterIndex = 0);//����Ʈ������
	virtual void Execute();
	virtual void Unexecute();
	virtual ~OnCharCommand();//�Ҹ���
	//�ζ����Լ� ����
	UINT GetNChar() const;
	Long GetRowIndex() const;
	Long GetLetterIndex() const;
private:
	UINT nChar;
	Long rowIndex;
	Long letterIndex;
	
};

//�ζ����Լ� ����
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