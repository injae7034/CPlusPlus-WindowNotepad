#ifndef _REMOVECOMMAND_H
#define _REMOVECOMMAND_H

#include "Command.h"

class Glyph;
class NotepadForm;

class RemoveCommand :public Command
{
public:
	RemoveCommand(NotepadForm* notepadForm = 0);
	virtual void Execute();
	virtual void Unexecute();
	virtual Long GetRowIndex();//멤버로 저장된 글자의 줄의 위치를 구하기
	virtual Long GetLetterIndex();//멤버로 저장된 글자의 칸의 위치를 구하기
	virtual Long GetSelectedStartXPos();//멤버로 저장된 선택이 시작되는 글자의 위치구하기
	virtual Long GetSelectedStartYPos();//멤버로 저장된 선택이 시작되는 줄의 위치구하기
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual ~RemoveCommand();//소멸자
	//인라인함수 선언(인라인함수는 부모클래스에서 물려받을 수 없음)
	inline Glyph* GetGlyph() const;//멤버로 저장된 글자 구하기
private:
	Glyph* glyph;//지우기 전에 지울 글자를 담을 공간
	bool isRedone;//다시실행인지 아닌지 여부
	Long rowIndex;//입력된 글자의 줄위치
	Long letterIndex;//입력된 글자의 칸위치
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	Long selectedStartXPos;//선택이 시작되는 글자위치
	Long selectedStartYPos;//선택이 종료되는 글자위치
};

//인라인함수 정의
inline Glyph* RemoveCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_REMOVECOMMAND_H