#ifndef _DELETEKEYACTIONCOMMAND_H
#define _DELETEKEYACTIONCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class DeleteKeyActionCommand :public Command
{
public:
	DeleteKeyActionCommand(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute();//실행
	virtual void Unexecute();//실행취소
	void SetUndoMacroEnd();//실행취소출력 종료지점 설정
	void SetRedoMacroEnd();//다시실행출력 종료지점 설정
	void SetRedone();//다시실행이라는 표시를 해줌
	Long GetRowIndex();//멤버로 저장된 글자의 줄의 위치를 구하기
	Long GetLetterIndex();//멤버로 저장괸 글자의 칸의 위치를 구하기
	bool IsUndoMacroEnd();//실행취소출력 종료지점 구하기
	bool IsRedoMacroEnd();//다시실행출력 종료지점 구하기
	bool IsRedone();//다시실행인지 아닌지 구하기
	virtual ~DeleteKeyActionCommand();//소멸자
	//인라인함수 선언(인라인함수는 부모클래스에서 물려받을 수 없음)
	inline Glyph* GetGlyph() const;//멤버로 저장된 글자 구하기
private:
	Long rowIndex;//입력된 글자의 줄위치
	Long letterIndex;//입력된 글자의 칸위치
	Glyph* glyph;//지우기 전에 지울 글자를 담을 공간
	bool isUndoMacroEnd;//실행취소출력 종료지점
	bool isRedoMacroEnd;//다시실행출력 종료지점
	bool isRedone;//다시실행인지 아닌지 여부
};

//인라인함수 정의
inline Glyph* DeleteKeyActionCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_DELETEKEYACTIONCOMMAND_H

