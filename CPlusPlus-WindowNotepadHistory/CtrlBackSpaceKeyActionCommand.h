#ifndef _CTRLBACKSPACEKEYACTIONCOMMAND_H
#define _CTRLBACKSPACEKEYACTIONCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class CtrlBackSpaceKeyActionCommand :public Command
{
public:
	CtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute();//실행
	virtual void Unexecute();//실행취소
	virtual void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	virtual void SetRedone();//다시실행이라는 표시를 해줌
	virtual Long GetRowIndex();//멤버로 저장된 글자의 줄의 위치를 구하기
	virtual Long GetLetterIndex();//멤버로 저장괸 글자의 칸의 위치를 구하기
	virtual bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	virtual bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	virtual bool IsRedone();//다시실행인지 아닌지 구하기
	virtual bool IsDirty();//변경사항이 있는지 확인 여부
	virtual bool IsSelectedTextsRemoved();//선택영역을 지웠는지 안지웠는지 여부
	virtual ~CtrlBackSpaceKeyActionCommand();//소멸자
	//인라인함수 선언(인라인함수는 부모클래스에서 물려받을 수 없음)
	Glyph* GetGlyph() const;//멤버로 저장된 글자 구하기
private:
	Long rowIndex;//입력된 글자의 줄위치
	Long letterIndex;//입력된 글자의 칸위치
	Glyph* glyph;//지우기 전에 지울 글자를 담을 공간
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isRedone;//다시실행인지 아닌지 여부
	bool isDirty;//변경사항이 있는지 여부
	bool isSelectedTextsRemoved;//선택영역을 지웠는지 안지웠는지 여부
};

//인라인함수 정의
inline Glyph* CtrlBackSpaceKeyActionCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_CTRLBACKSPACEKEYACTIONCOMMAND_H

