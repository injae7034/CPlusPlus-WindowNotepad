#ifndef _COMMANDCREATOR_H
#define _COMMANDCREATOR_H

#include "Command.h"
#include "NotepadForm.h"
//NotepadForm은 주소로 밖에 안쓰여서 전방선언을 해도 가능하지만 UINT자료형이 MFC와 관련이
//있는 자료형이기 때문에 "NotepadForm.h"가 없으면 UINT자료형은 사용할 수 없음!

class CommandCreator
{
public:
	CommandCreator(NotepadForm* notepadForm = 0);//디폴트생성자
	Command* Create(UINT nId);//팩토리 메소드 패턴
	~CommandCreator();//소멸자
public://notepadForm과 연관화
	NotepadForm* notepadForm;
};

#endif // !_COMMANDCREATOR_H

