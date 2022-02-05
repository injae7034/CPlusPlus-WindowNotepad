#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, UINT nChar,
		Long rowIndex = 0, Long letterIndex = 0);//����Ʈ������
	virtual void Execute();
	virtual void Unexecute();
	void SetUndoMacroEnd();
	void SetRedoMacroEnd();
	void SetRedone();
	virtual ~OnCharCommand();//�Ҹ���
	//�ζ����Լ� ����
	UINT GetNChar() const;
	Long GetRowIndex() const;
	Long GetLetterIndex() const;
	Long GetStartSplitIndex() const;
	bool IsUndoMacroEnd() const;
	bool IsRedoMacroEnd() const;
	bool IsRedone() const;
private:
	UINT nChar;
	Long rowIndex;
	Long letterIndex;
	Long startSplitIndex;
	bool isUndoMacroEnd;
	bool isRedoMacroEnd;
	bool isRedone;
	
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
inline Long OnCharCommand::GetStartSplitIndex() const
{
	return this->letterIndex;
}
inline bool OnCharCommand::IsUndoMacroEnd() const
{
	return this->isUndoMacroEnd;
}
inline bool OnCharCommand::IsRedoMacroEnd() const
{
	return this->isRedoMacroEnd;
}
inline bool OnCharCommand::IsRedone() const
{
	return this->isRedone;
}

#endif // !_ONCHARCOMMAND_H