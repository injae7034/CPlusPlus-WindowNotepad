#ifndef _ONREPLACEALLBUTTONCLICKEDCOMMAND_H
#define _ONREPLACEALLBUTTONCLICKEDCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"
#include"Stack.h"

class Glyph;

class OnReplaceAllButtonClickedCommand :public Command
{
public:
	OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm = 0, Long undoListCapacity = 256,
		Long redoListCapacity = 256);//����Ʈ������
	void Execute();
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual ~OnReplaceAllButtonClickedCommand();//�Ҹ���
	//�ζ����Լ�
	Long GetUndoListCapacity() const;
	Long GetUndoListLength() const;
	Long GetRedoListCapacity() const;
	Long GetRedoListLength() const;
private:
	string findingKeyword;//ã�� �ܾ�
	string replacingKeyword;//�ٲ� �ܾ�
	int matchCaseChecked;//��ҹ��� ���� üũ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	Stack<Command*> undoList;
	Long undoListCapacity;
	Long undoListLength;
	Stack<Command*> redoList;
	Long redoListCapacity;
	Long redoListLength;
};

//�ζ����Լ�����
inline Long OnReplaceAllButtonClickedCommand::GetUndoListCapacity() const
{
	return this->undoListCapacity;
}
inline Long OnReplaceAllButtonClickedCommand::GetUndoListLength() const
{
	return this->undoListLength;
}
inline Long OnReplaceAllButtonClickedCommand::GetRedoListCapacity() const
{
	return this->undoListCapacity;
}
inline Long OnReplaceAllButtonClickedCommand::GetRedoListLength() const
{
	return this->undoListLength;
}

#endif // !_ONREPLACEALLBUTTONCLICKEDCOMMAND_H