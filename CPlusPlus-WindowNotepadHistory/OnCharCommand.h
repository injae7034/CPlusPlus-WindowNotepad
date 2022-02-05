#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, UINT nChar,
		Long rowIndex = 0, Long letterIndex = 0, Long startSplitIndex = 0);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	void SetUndoMacroEnd();//���������� �������� ����
	void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual ~OnCharCommand();//�Ҹ���
	//�ζ����Լ� ����
	UINT GetNChar() const;//����� ����� ���� ���ϱ�
	Long GetRowIndex() const;//����� ����� ������ ���� ��ġ�� ���ϱ�
	Long GetLetterIndex() const;//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	Long GetStartSplitIndex() const;//����� ����� ���ڰ� ���๮���� ��� split�Ǵ� ĭ�� ��ġ���ϱ�
	bool IsUndoMacroEnd() const;//���������� �������� ���ϱ�
	bool IsRedoMacroEnd() const;//�ٽý������ �������� ���ϱ�
private:
	UINT nChar;//OnCharCommand�� ������ �� �Էµ� ����
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Long startSplitIndex;//���๮���̸� split�Ǵ� ĭ�� ��ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
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

#endif // !_ONCHARCOMMAND_H