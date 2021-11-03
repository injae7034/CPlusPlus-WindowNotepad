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
	virtual Long GetStartXPos();//시작하는 글자의 위치구하기
	virtual Long GetStartYPos();//시작하는 줄의 위치구하기
	virtual Long GetSelectedEndXPos();//선택이 끝나는 글자의 위치구하기
	virtual Long GetSelectedEndYPos();//선택이 끝나는 줄의 위치구하기
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual bool IsSelectedTextsRemoved();//선택영역을 지웠는지 안지웠는지 여부
	virtual Glyph* GetGlyph();//멤버로 저장된 글자 구하기
	virtual ~RemoveCommand();//소멸자
private:
	Glyph* glyph;//지우기 전에 지울 글자를 담을 공간
	bool isRedone;//다시실행인지 아닌지 여부
	Long startXPos;//시작하는 글자의 위치
	Long startYPos;//시작하는 줄의 위치
	Long selectedEndXPos;//선택이 끝나는 글자위치
	Long selectedEndYPos;//선택이 끝나는 줄위치
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isSelectedTextsRemoved;//선택영역을 지웠는지 안지웠는지 여부
};

#endif // !_REMOVECOMMAND_H