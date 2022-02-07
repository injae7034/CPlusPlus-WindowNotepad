#ifndef _ONREPLACEALLBUTTONCLICKEDCOMMAND_H
#define _ONREPLACEALLBUTTONCLICKEDCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class OnReplaceAllButtonClickedCommand :public Command
{
public:
	OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	void Execute();
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual ~OnReplaceAllButtonClickedCommand();//�Ҹ���
private:
	string findingKeyword;//ã�� �ܾ�
	string replacingKeyword;//�ٲ� �ܾ�
	int matchCaseChecked;//��ҹ��� ���� üũ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
};

#endif // !_ONREPLACEALLBUTTONCLICKEDCOMMAND_H