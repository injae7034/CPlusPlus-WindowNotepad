#ifndef _ONIMECHARCOMMAND_H
#define _ONIMECHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class OnImeCharCommand :public Command
{
public:
	OnImeCharCommand(NotepadForm* notepadForm, WPARAM wParam,
		Long rowIndex = 0, Long letterIndex = 0);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	void SetUndoMacroEnd();//���������� �������� ����
	void SetRedoMacroEnd();//�ٽý������ �������� ����
	void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual ~OnImeCharCommand();//�Ҹ���
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	WPARAM GetWParam() const;//����� ����� ���� ���ϱ�

private:
	WPARAM wParam;//OnCharCommand�� ������ �� �Էµ� ����
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
};

//�ζ����Լ� ����
inline WPARAM OnImeCharCommand::GetWParam() const
{
	return this->wParam;
}


#endif // !_ONIMECHARCOMMAND_H