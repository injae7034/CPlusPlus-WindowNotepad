#ifndef _COMMAND_H
#define _COMMAND_H

class NotepadForm;//�ּҷ� �̿�Ǳ� ������ ���漱���� �ʿ���!

class Command {
public:
	Command(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute() = 0;//Command Pattern
	virtual ~Command() = 0;//�߻�Ŭ���� �Ҹ���
	//�ζ����Լ�����
	NotepadForm* GetNotepadForm() const;
protected://��ӹ��� subclass���� superclass�� �����͸���� �����ϱ� ���ؼ� protected�� �������.
	NotepadForm* notepadForm;
};

//�ζ����Լ� ����
inline NotepadForm* Command::GetNotepadForm() const
{
	return this->notepadForm;
}
#endif // !_COMMAND_H
