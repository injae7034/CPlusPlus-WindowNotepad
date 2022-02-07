#ifndef _COMMAND_H
#define _COMMAND_H

class Glyph;
class NotepadForm;//주소로 이용되기 때문에 전방선언이 필요함!
typedef signed long int Long;

class Command {
public:
	Command(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute() = 0;//실행
	virtual void Unexecute();//실행취소
	virtual bool IsUndoMacroEnd();//실행취소 매크로 종료지점인지 아닌지 확인
	virtual bool IsRedoMacroEnd();//다시실행 매크로 종료지점인지 아닌지 확인
	virtual void SetUndoMacroEnd();//실행취소 매크로 종료지점 설정
	virtual void SetRedoMacroEnd();//다시실행 매크로 종료지점 설정
	virtual void SetRedone();//다시 실행이라고 설정함
	virtual bool IsRedone();//다시 실행인지 아닌지 확인함
	virtual bool IsDirty();//변경사항이 있는지 확인 여부(지우는 기능관 관련된 Command 에서 사용됨)
	virtual bool IsSelectedTextsRemoved();
	virtual Long GetStartPasteRowIndex();
	virtual Long GetStartPasteLetterIndex();
	virtual Long GetRowIndex();
	virtual Long GetLetterIndex();
	virtual Long GetStartXPos();//시작하는 글자의 위치구하기
	virtual Long GetStartYPos();//시작하는 줄의 위치구하기
	virtual Long GetSelectedEndXPos();//선택이 끝나는 글자의 위치구하기
	virtual Long GetSelectedEndYPos();//선택이 끝나는 줄의 위치구하기
	virtual Long GetPastingEndXPos();//붙여넣기가 끝나는 글자의 위치구하기
	virtual Long GetPastingEndYPos();//붙여넣기가 끝나는 줄의 위치구하기
	virtual Glyph* GetGlyph();//command가 가지고 있는 Glyph구하기
	virtual ~Command() = 0;//추상클래스 소멸자
	//인라인함수정의
	NotepadForm* GetNotepadForm() const;
protected://상속받은 subclass들이 superclass의 데이터멤버에 접근하기 위해서 protected로 해줘야함.
	NotepadForm* notepadForm;
};

//인라인함수 정의
inline NotepadForm* Command::GetNotepadForm() const
{
	return this->notepadForm;
}
#endif // !_COMMAND_H
