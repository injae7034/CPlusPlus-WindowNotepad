#ifndef _CARET_H
#define _CARET_H

typedef signed long int Long;
class NotepadForm;//전방선언(주소로 이용되기때문에 필요)

class Caret
{
public:
	//Caret()기본생성자가 필요함
	//Caret(NotepadForm* notepadForm)은 매개변수1개생성자이고
	//각 클래스는 무조건 Caret()과 같은 형태의 기본생성자가 필요함
	//기본생성자없이 쓰려면 Caret(NotepadForm* notepadForm = 0);과 같은 형태로
	//NotepadForm* notepadForm = 0 디폴트매개변수를 주면 된다.
	//Array에서 Long capacity=256으로 디폴트매개변수를 줘서 기본생성자로 이용했음!!!
	Caret(NotepadForm* notepadForm = 0);//디폴트생성자에서 notepadForm을 받아서 캐럿이랑 노트패드랑 관계맺어주기
	//Caret(NotepadForm* notepadForm, Long height, Long width);//매개변수3개생성자
	Caret(const Caret& source);//복사생성자
	~Caret();//소멸자
	Caret& operator=(const Caret& source);//치환연산자
	void Create(Long height, Long width);//캐럿 생성하기 높이,너비를 입력받아 캐럿을 생성하기 createsolidCaret
	void Move(Long x, Long y);//캐럿 이동하기 SetCaretPos
	void Show();//캐럿 보여주기 ShowCaret
	//인라인함수
	//CDC& GetDc() const; CDC랑 캐럿이랑 관계를 맺는게 아니라 notepadForm이랑 캐럭이랑 관계맺기
	//NotepadForm* GetNotepadForm() const;//notepadForm이 public이기 때문에 인라인함수가 필요없음
	//Long GetHeight() const;
	//Long GetWidth() const;
//private: AccoubtBookForm이 AccountBook의 주소를 public으로 가지고 있는것과 같다.
	//CDC dc;
public:
	NotepadForm* notepadForm;// 캐럿이랑 notepadForm이랑 관계를 맺기
	//캐럿의 길이와 너비는 항상 바뀌기 때문에 캐럿의 멤버에 너비나 길이가 굳이 있을 필요가 없다!
	//Long height;
	//Long width;
};

#if 0
//인라인함수정의 난 왜 바보같이 CDC랑 캐럿이랑 관계를 맺으려고 했을까?
//조금만 더 생각해보면 당연히 캐럿이랑 notepadForm이랑 관계를 맺는게 당연했는데
//정신차리자 조급하게 빨리하려고 하지말고 생각을 먼저 정리하고 코딩을하자
inline CDC& Caret::GetDc() const
{
	return const_cast<CDC&>(this->dc);
	
}
#endif
#if 0
inline NotepadForm* Caret::GetNotepadForm() const
{
	return this->notepadForm;
}
#endif
#if 0
inline Long Caret::GetHeight() const
{
	return this->height;
}
inline Long Caret::GetWidth() const
{
	return this->width;
}
#endif
#endif // !_CARET_H
