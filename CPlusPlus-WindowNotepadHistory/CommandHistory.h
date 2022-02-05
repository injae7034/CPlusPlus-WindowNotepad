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
	Command* current;
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
	bool IsUndoListEmpty() const;
	Long GetRedoListCapacity() const;
	Long GetRedoListLength() const;
	bool IsRedoListEmpty() const;
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
inline bool CommandHistory::IsUndoListEmpty() const
{
	return this->undoList.IsEmpty();
}
inline Long CommandHistory::GetRedoListCapacity() const
{
	return this->undoListCapacity;
}
inline Long CommandHistory::GetRedoListLength() const
{
	return this->undoListLength;
}
inline bool CommandHistory::IsRedoListEmpty() const
{
	return this->redoList.IsEmpty();
}
#endif // !_COMMANDHISTORY_H
