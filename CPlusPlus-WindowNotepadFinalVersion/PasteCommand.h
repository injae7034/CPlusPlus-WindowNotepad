#ifndef _PASTECOMMAND_H
#define _PASTECOMMAND_H

#include "Command.h"

class NotepadForm;
class Glyph;

class PasteCommand :public Command
{
public:
	PasteCommand(NotepadForm* notepadForm = 0);
	void Execute();
	virtual void Unexecute();//실행취소
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual Long GetStartXPos();//시작하는 글자의 위치구하기
	virtual Long GetStartYPos();//시작하는 줄의 위치구하기
	virtual Long GetSelectedEndXPos();//선택이 끝나는 글자의 위치구하기
	virtual Long GetSelectedEndYPos();//선택이 끝나는 줄의 위치구하기
	virtual Long GetPastingEndXPos();//붙여넣기가 끝나는 글자의 위치구하기
	virtual Long GetPastingEndYPos();//붙여넣기가 끝나는 줄의 위치구하기
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual bool IsDirty();//변경사항이 있는지 확인 여부
	virtual bool IsSelectedTextsRemoved();//선택영역을 지웠는지 안지웠는지 여부
	//인라인함수 선언(인라인함수는 부모클래스에서 물려받을 수 없음)
	Glyph* GetNote() const;//멤버로 저장된 선택영역 구하기
	Glyph* GetGlyph() const;//멤버로 저장된 글자 구하기
	~PasteCommand();//소멸자
private:
	Glyph* note;//선택영역을 지우기 전에 선택영역을 저장할 공간
	Glyph* glyph;//지우기 전에 지울 글자를 담을 공간
	Long startXPos;//시작하는 글자의 위치
	Long startYPos;//시작하는 줄의 위치
	Long selectedEndXPos;//선택이 끝나는 글자위치
	Long selectedEndYPos;//선택이 끝나는 줄위치
	Long pastingEndXPos;//붙여넣기가 끝나는 글자위치
	Long pastingEndYPos;//붙여넣기가 끝나는 줄위치
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isRedone;//다시실행인지 아닌지 여부
	bool isDirty;//변경사항이 있는지 여부
	bool isSelectedTextsRemoved;//선택영역을 지웠는지 안지웠는지 여부
};

//인라인함수 정의
inline Glyph* PasteCommand::GetGlyph() const
{
	return this->glyph;
}
inline Glyph* PasteCommand::GetNote() const
{
	return this->note;
}

#endif // !_PASTECOMMAND_H