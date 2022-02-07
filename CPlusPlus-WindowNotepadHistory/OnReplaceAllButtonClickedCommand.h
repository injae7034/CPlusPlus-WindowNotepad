#ifndef _ONREPLACEALLBUTTONCLICKEDCOMMAND_H
#define _ONREPLACEALLBUTTONCLICKEDCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class OnReplaceAllButtonClickedCommand :public Command
{
public:
	OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	void Execute();
	virtual void Unexecute();//실행취소
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual ~OnReplaceAllButtonClickedCommand();//소멸자
private:
	string findingKeyword;//찾을 단어
	string replacingKeyword;//바꿀 단어
	int matchCaseChecked;//대소문자 구분 체크
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isRedone;//다시실행인지 아닌지 여부
};

#endif // !_ONREPLACEALLBUTTONCLICKEDCOMMAND_H