#ifndef _KEYACTIONCREATOR_H
#define _KEYACTIONCREATOR_H

#include "NotepadForm.h"
//NotepadForm은 주소로 밖에 안쓰여서 전방선언을 해도 가능하지만 UINT자료형이 MFC와 관련이
//있는 자료형이기 때문에 "NotepadForm.h"가 없으면 UINT자료형은 사용할 수 없음!

class KeyAction;//주소로만 쓰이기 때문에 전방선언 해주면 됨.

class KeyActionCreator
{
public://notepadForm과 연관화
	NotepadForm* notepadForm;
public:
	KeyActionCreator(NotepadForm* notepadForm = 0);//디폴트생성자
	KeyAction* Create(UINT nChar);//팩토리 메소드 패턴
	~KeyActionCreator();//소멸자
};

#endif // !_KEYACTIONCREATOR_H

