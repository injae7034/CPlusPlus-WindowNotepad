#ifndef _ROWAUTOCHANGE_H
#define _ROWAUTOCHANGE_H

typedef signed long int Long;

class NotepadForm;

class RowAutoChange
{
public:
	NotepadForm* notepadForm;
public:
	RowAutoChange(NotepadForm* notepadForm = 0);//����Ʈ������
	void Do();//�ڵ������� �ϴ�
	void Undo();//�ڵ������� ����ϴ�
	//�ڵ����� �� �ٰ� ĳ���� ��ġ�� ���� �ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� ���Ѵ�
	void GetOriginPos(Long changedCaretPos, Long changedRowPos,
		Long* originCaretPos, Long* originRowPos);
	//�ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� ���� �ڵ����� �� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
	void GetChangedPos(Long originCaretPos, Long originRowPos,
		Long* changedCaretPos, Long* changedRowPos);
	virtual ~RowAutoChange();//�Ҹ���
};

#endif // !_ROWAUTOCHANGE_H
