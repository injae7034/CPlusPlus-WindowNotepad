#ifndef _COMMANDHISTORY_H
#define _COMMANDHISTORY_H

#include"Stack.h"

typedef signed long int Long;

class NotepadForm;
class Command;

class CommandHistory
{
public:
	NotepadForm* notepadForm;
public:
	CommandHistory(NotepadForm* notepadForm = 0, Long undoListCapacity = 256,
		Long redoListCapacity = 256);//����Ʈ������
	Command* GetUndoListTop();//UndoList�� ���� ������ �迭 ��� ���ϱ�
	Command* GetRedoListTop();//RedoList�� ���� ������ �迭 ��� ���ϱ�
	Long PushUndoList(Command* command);
	Long PushRedoList(Command* command);
	Command* PopUndoList();
	Command* PopRedoList();
	void MakeRedoListEmpty();
	void Undo();//�������
	void Redo();//�ٽý���
	~CommandHistory();//�Ҹ���
	//�ζ����Լ�
	Long GetUndoListCapacity() const;
	Long GetUndoListLength() const;
	Long GetRedoListCapacity() const;
	Long GetRedoListLength() const;
private:
	Stack<Command*> undoList;
	Long undoListCapacity;
	Long undoListLength;
	Stack<Command*> redoList;
	Long redoListCapacity;
	Long redoListLength;
};

//�ζ����Լ�����
inline Long CommandHistory::GetUndoListCapacity() const
{
	return this->undoListCapacity;
}
inline Long CommandHistory::GetUndoListLength() const
{
	return this->undoListLength;
}
inline Long CommandHistory::GetRedoListCapacity() const
{
	return this->undoListCapacity;
}
inline Long CommandHistory::GetRedoListLength() const
{
	return this->undoListLength;
}
#endif // !_COMMANDHISTORY_H
