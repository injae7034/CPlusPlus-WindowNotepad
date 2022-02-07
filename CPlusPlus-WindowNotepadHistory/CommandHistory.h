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
		Long redoListCapacity = 256);//디폴트생성자
	Command* GetUndoListTop();//UndoList의 제일 마지막 배열 요소 구하기
	Command* GetRedoListTop();//RedoList의 제일 마지막 배열 요소 구하기
	Long PushUndoList(Command* command);
	Long PushRedoList(Command* command);
	Command* PopUndoList();
	Command* PopRedoList();
	void MakeRedoListEmpty();
	void Undo();//실행취소
	void Redo();//다시실행
	~CommandHistory();//소멸자
	//인라인함수
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

//인라인함수정의
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
