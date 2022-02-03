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
	void DoAllRows();//�޸����� ��ü ���� �ڵ������Ű��
	void UndoAllRows();//�޸����� ��ü ���� �ڵ����� ����ϴ�
	void DoRow();//���ٸ� �ڵ������Ű��
	void UndoRow();//���ٸ� �ڵ����� ����ϴ�
	//�ڵ����� �� ĳ���� ��ġ(x, y)�� ���� �ڵ����� �� ���� ĳ���� ��ġ(x, y)�� ���Ѵ�
	void GetOriginPos(Long changedLetterPos, Long changedRowPos,
		Long* originLetterPos, Long* originRowPos);
	//�ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� ���� �ڵ����� �� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
	void GetChangedPos(Long originLetterPos, Long originRowPos,
		Long* changedLetterPos, Long* changedRowPos);
	virtual ~RowAutoChange();//�Ҹ���
};

#endif // !_ROWAUTOCHANGE_H
