#ifndef _ROWAUTOCHANGE_H
#define _ROWAUTOCHANGE_H

typedef signed long int Long;

class NotepadForm;

class RowAutoChange
{
public:
	NotepadForm* notepadForm;
public:
	RowAutoChange(NotepadForm* notepadForm = 0);//디폴트생성자
	void Do();//자동개행을 하다
	void Undo();//자동개행을 취소하다
	//자동개행 후 줄과 캐럿의 위치를 통해 자동개행 전 원래 줄과 캐럿의 위치를 구한다
	void GetOriginPos(Long changedCaretPos, Long changedRowPos,
		Long* originCaretPos, Long* originRowPos);
	//자동개행 전 원래 줄과 캐럿의 위치를 통해 자동개행 후 줄과 캐럿의 위치를 구한다.
	void GetChangedPos(Long originCaretPos, Long originRowPos,
		Long* changedCaretPos, Long* changedRowPos);
	virtual ~RowAutoChange();//소멸자
};

#endif // !_ROWAUTOCHANGE_H
