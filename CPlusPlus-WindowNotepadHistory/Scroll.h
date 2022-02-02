#ifndef _SCROLL_H
#define _SCROLL_H

#include<afxwin.h>//SCROLLINFO를 사용하기 위해

class ScrollController;//전방선언
typedef signed long int Long;

class Scroll
{
	friend ScrollController;//프렌드클래스 등록
	//ScrollController에서 Scroll에 정보를 접근하고 변경할 수 있음.
	//ScrollController는 Scroll에 대한 정보를 관장하므로 Scroll에 직접 접근하고 값을 변경시킬 수 있게
	//Friend 클래스 등록을 하는것이 맞다(아니면 정보변경을 위해 매번 ScrollController에서 Scroll의
	//생성자를 호출하고 할당해제를 해줘야하는데 이게 매번하려고 하면 비효율적임!)
public:
	ScrollController* scrollController;
public:
	Scroll(ScrollController* scrollController = 0);//디폴트생성자
	Scroll(ScrollController* scrollController, Long currentPos, Long min, Long max,
		Long pageSize);//매개변수를 5개가지는 생성자
	Scroll(const Scroll& source);//복사생성자
	Scroll& operator=(const Scroll& source);//치환연산자
	//Scroll 이동연산
	virtual Long First();
	virtual Long Last();
	virtual Long LinePrevious() = 0;
	virtual Long LineNext() = 0;
	virtual Long pagePrevious();
	virtual Long pageNext();
	virtual Long Move(Long currentPos);
	virtual ~Scroll() = 0;//추상클래스 소멸자
	//인라인함수 선언
	Long GetCurrentPos() const;
	Long GetMin() const;
	Long GetMax() const;
	Long GetPageSize() const;
protected:
	Long currentPos;//현재화면위치
	Long min;//가로 또는 세로 텍스트 최소값
	Long max;//가로 또는 세로 텍스트 최대값
	Long pageSize;//현재 화면의 가로 또는 세로 크기
};

//인라인함수 정의
inline Long Scroll::GetCurrentPos() const
{
	return this->currentPos;
}
inline Long Scroll::GetMin() const
{
	return this->min;
}
inline Long Scroll::GetMax() const
{
	return this->max;
}
inline Long Scroll::GetPageSize() const
{
	return this->pageSize;
}


#endif // !_SCROLL_H
