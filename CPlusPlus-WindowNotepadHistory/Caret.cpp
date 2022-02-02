#include "Caret.h"
#include "NotepadForm.h"


//디폴트생성자(캐럿을 생성하면서 notepadForm이랑 관계맺기)
Caret::Caret(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//복사생성자
Caret::Caret(const Caret& source)
{
	this->notepadForm = source.notepadForm;
}

//치환연산자
Caret& Caret::operator=(const Caret& source)
{
	this->notepadForm = source.notepadForm;
	
	return *this;
}

//소멸자
Caret::~Caret()
{

}

//캐럿생성하기
void Caret::Create(Long height, Long width)
{
	//수치계산은 밖에서 하기
	this->notepadForm->CreateSolidCaret(height, width);
}

//캐럿이동하기
void Caret::Move(Long x, Long y)
{
	this->notepadForm->SetCaretPos(CPoint(x, y));
}

//캐럿 보여주기
void Caret::Show()
{
	this->notepadForm->ShowCaret();
}
