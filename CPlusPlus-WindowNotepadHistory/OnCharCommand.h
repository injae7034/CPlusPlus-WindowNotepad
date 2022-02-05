#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, UINT nChar,
		Long rowIndex = 0, Long letterIndex = 0, Long startSplitIndex = 0);//디폴트생성자
	virtual void Execute();//실행
	virtual void Unexecute();//실행취소
	void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual ~OnCharCommand();//소멸자
	//인라인함수 선언
	UINT GetNChar() const;//멤버로 저장된 글자 구하기
	Long GetRowIndex() const;//멤버로 저장된 글자의 줄의 위치를 구하기
	Long GetLetterIndex() const;//멤버로 저장괸 글자의 칸의 위치를 구하기
	Long GetStartSplitIndex() const;//멤버로 저장된 글자가 개행문자일 경우 split되는 칸의 위치구하기
	bool IsUndoMacroEnd() const;//실행취소출력 종료지점 구하기
	bool IsRedoMacroEnd() const;//다시실행출력 종료지점 구하기
private:
	UINT nChar;//OnCharCommand를 실행할 때 입력된 글자
	Long rowIndex;//입력된 글자의 줄위치
	Long letterIndex;//입력된 글자의 칸위치
	Long startSplitIndex;//개행문자이면 split되는 칸의 위치
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
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