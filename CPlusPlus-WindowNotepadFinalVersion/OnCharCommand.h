#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, Glyph* glyph);//디폴트생성자
	virtual void Execute();//실행
	virtual void Unexecute();//실행취소
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual ~OnCharCommand();//소멸자
	virtual Long GetStartXPos();//시작하는 글자의 위치구하기
	virtual Long GetStartYPos();//시작하는 줄의 위치구하기
	virtual Long GetSelectedEndXPos();//선택이 끝나는 글자의 위치구하기
	virtual Long GetSelectedEndYPos();//선택이 끝나는 줄의 위치구하기
	virtual Long GetPastingEndXPos();//붙여넣기가 끝나는 글자의 위치구하기
	virtual Long GetPastingEndYPos();//붙여넣기가 끝나는 줄의 위치구하기
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual bool IsSelectedTextsRemoved();//선택영역을 지웠는지 안지웠는지 여부
	virtual Glyph* GetGlyph();//멤버로 저장된 글자 구하기
	virtual Glyph* GetNote();//멤버로 저장된 노트 구하기
private:
	Glyph* note;//OnCharCommand를 실행할 때 선택된 영역이 있으면 그 선택된 영역의 content를 담을 공간
	Glyph* glyph;//OnCharCommand를 실행할 때 입력된 글자
	Long startXPos;//시작하는 글자의 위치
	Long startYPos;//시작하는 줄의 위치
	Long selectedEndXPos;//선택이 끝나는 글자위치
	Long selectedEndYPos;//선택이 끝나는 줄위치
	Long pastingEndXPos;//붙여넣기가 끝나는 글자위치
	Long pastingEndYPos;//붙여넣기가 끝나는 줄위치
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isRedone;//다시실행인지 아닌지 여부
	bool isSelectedTextsRemoved;//선택영역을 지웠는지 안지웠는지 여부
};

#endif // !_ONCHARCOMMAND_H