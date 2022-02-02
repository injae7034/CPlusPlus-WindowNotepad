#ifndef _CARET_H
#define _CARET_H

typedef signed long int Long;
class CaretController;//전방선언(주소로 이용되기때문에 필요)

class Caret
{
public:
	CaretController* caretController;
public:
	//Caret()기본생성자가 필요함
	//Caret(NotepadForm* notepadForm)은 매개변수1개생성자이고
	//각 클래스는 무조건 Caret()과 같은 형태의 기본생성자가 필요함
	//기본생성자없이 쓰려면 Caret(NotepadForm* notepadForm = 0);과 같은 형태로
	//NotepadForm* notepadForm = 0 디폴트매개변수를 주면 된다.
	//Array에서 Long capacity=256으로 디폴트매개변수를 줘서 기본생성자로 이용했음!!!
	Caret(CaretController* caretController = 0);//디폴트생성자에서 notepadForm을 받아서 캐럿이랑 노트패드랑 관계맺어주기
	Caret(const Caret& source);//복사생성자
	~Caret();//소멸자
	Caret& operator=(const Caret& source);//치환연산자
	void Create(Long height, Long width);//캐럿 생성하기 높이,너비를 입력받아 캐럿을 생성하기 createsolidCaret
	void Move(Long x, Long y);//캐럿 이동하기 SetCaretPos
	void Show();//캐럿 보여주기 ShowCaret
};

#endif // !_CARET_H
