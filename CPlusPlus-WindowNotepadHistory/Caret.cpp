#include "Caret.h"
#include "NotepadForm.h"


//디폴트생성자(캐럿을 생성하면서 notepadForm이랑 관계맺기)
Caret::Caret(NotepadForm* notepadForm)
{
#if 0
	//1.notepadForm의 주소를 입력받는다.
	//2. notepadForm의 dc를 구한다.
	//CDC* dc = notepadForm->GetDC();//새로운 DC가 생성됨. DC는 하나의 윈도우에서도 여러개일수 있음
	//3. notepadForm의 글꼴 정보를 얻는다. //글꼴의 정보를 얻고 싶으면 여기서 CFont를 설정해줘야함
	//TEXTMETRIC text; //밖에서 글꼴설정하고 여기서 GetDC해봤자 새로운 DC가 생성되기 때문에 의미없음.
	//GetMetrics로 글꼴의 정보를 얻고 싶으면 여기서 따로 CFont를 설정해줘야함.
	//dc->GetTextMetrics(&text);
#endif
	this->notepadForm = notepadForm;
	//5. notepadForm 글꼴의 정보를 Caret의 멤버에 저장한다.
	//this->height = text.tmHeight;
	//this->height = 0;
	//this->width = 0;
}

#if 0
//매개변수3개생성자
Caret::Caret(NotepadForm* notepadForm, Long hegiht, Long width)
{
	this->notepadForm = notepadForm;
	this->height = height;
	this->width = width;
}
#endif


//복사생성자
Caret::Caret(const Caret& source)
{
	this->notepadForm = source.notepadForm;
	//this->height = source.height;
	//this->width = source.width;
}

//치환연산자
Caret& Caret::operator=(const Caret& source)
{
	this->notepadForm = source.notepadForm;
	//this->height = source.height;
	//this->width = source.width;
	
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
#if 0
	//2. notepadForm의 dc를 구한다.
	CDC *dc = this->notepadForm->GetDC();
	//3. notepadForm의 글꼴 정보를 얻는다.
	TEXTMETRIC text;
	dc->GetTextMetrics(&text);
	//4. notepadForm이 한글 조립중이 아니면
	if (notepadForm->GetIsComposing() == false)
	{
		//4.1 캐럿의 넓이를 0으로 생성한다.
		notepadForm->CreateSolidCaret(0, text.tmHeight)
	}
#endif
	//this->height = height;
	//this->width = width;
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
