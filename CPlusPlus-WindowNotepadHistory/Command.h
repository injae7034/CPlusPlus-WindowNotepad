#ifndef _COMMAND_H
#define _COMMAND_H

class NotepadForm;//주소로 이용되기 때문에 전방선언이 필요함!

class Command {
public:
	Command(NotepadForm* notepadForm = 0);//디폴트생성자
	virtual void Execute() = 0;//Command Pattern
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
