#include "Caret.h"
#include "CaretController.h"
#include "NotepadForm.h"


//디폴트생성자(캐럿을 생성하면서 notepadForm이랑 관계맺기)
Caret::Caret(CaretController* caretController)
{
	this->caretController = caretController;
}

//복사생성자
Caret::Caret(const Caret& source)
{
	this->caretController = source.caretController;
}

//치환연산자
Caret& Caret::operator=(const Caret& source)
{
	this->caretController = source.caretController;
	
	return *this;
}

//소멸자
Caret::~Caret()
{
	//3. 메모장의 캐럿을 숨긴다.
	this->caretController->notepadForm->HideCaret();
	//4. 메모장의 캐럿을 없앤다.
	::DestroyCaret();
}

//캐럿생성하기
void Caret::Create(Long height, Long width)
{
	//수치계산은 밖에서 하기
	this->caretController->notepadForm->CreateSolidCaret(height, width);
}

//캐럿이동하기
void Caret::Move(Long x, Long y)
{
	this->caretController->notepadForm->SetCaretPos(CPoint(x, y));
}

//캐럿 보여주기
void Caret::Show()
{
	this->caretController->notepadForm->ShowCaret();
}
