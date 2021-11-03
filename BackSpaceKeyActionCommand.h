#ifndef _BACKSPACEKEYACTIONCOMMAND_H
#define _BACKSPACEKEYACTIONCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class BackSpaceKeyActionCommand :public Command
{
public:
	BackSpaceKeyActionCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute();//실행
	virtual void Unexecute();//실행취소
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual Long GetStartXPos();//시작하는 글자의 위치구하기
	virtual Long GetStartYPos();//시작하는 줄의 위치구하기
	virtual Long GetSelectedEndXPos();//선택이 끝나는 글자의 위치구하기
	virtual Long GetSelectedEndYPos();//선택이 끝나는 줄의 위치구하기
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual bool IsDirty();//변경사항이 있는지 확인 여부
	virtual bool IsSelectedTextsRemoved();//선택영역을 지웠는지 안지웠는지 여부
	virtual ~BackSpaceKeyActionCommand();//소멸자
	virtual Glyph* GetGlyph();//멤버로 저장된 글자 구하기
private:
	Long startXPos;//시작하는 글자의 위치
	Long startYPos;//시작하는 줄의 위치
	Long selectedEndXPos;//선택이 끝나는 글자위치
	Long selectedEndYPos;//선택이 끝나는 줄위치
	Glyph* glyph;//지우기 전에 지울 글자를 담을 공간
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isRedone;//다시실행인지 아닌지 여부
	bool isDirty;//변경사항이 있는지 여부
	bool isSelectedTextsRemoved;//선택영역을 지웠는지 안지웠는지 여부
};

#endif // !_BACKSPACEKEYACTIONCOMMAND_H

