#ifndef _COMMAND_H
#define _COMMAND_H

class NotepadForm;//�ּҷ� �̿�Ǳ� ������ ���漱���� �ʿ���!
typedef signed long int Long;

class Command {
public:
	Command(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute() = 0;//����
	virtual void Unexecute();//�������
	virtual bool IsUndoMacroEnd();//������� ��ũ�� ������������ �ƴ��� Ȯ��
	virtual bool IsRedoMacroEnd();//�ٽý��� ��ũ�� ������������ �ƴ��� Ȯ��
	virtual void SetUndoMacroEnd();//������� ��ũ�� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý��� ��ũ�� �������� ����
	virtual void SetRedone();//�ٽ� �����̶�� ������
	virtual bool IsRedone();//�ٽ� �������� �ƴ��� Ȯ����
	virtual bool IsDirty();//��������� �ִ��� Ȯ�� ����(����� ��ɰ� ���õ� Command ���� ����)
	virtual bool IsSelectedTextsRemoved();
	virtual Long GetRowIndex();
	virtual Long GetLetterIndex();
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
