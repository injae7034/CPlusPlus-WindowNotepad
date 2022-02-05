#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, UINT nChar,
		Long rowIndex = 0, Long letterIndex = 0);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	void SetUndoMacroEnd();//���������� �������� ����
	void SetRedoMacroEnd();//�ٽý������ �������� ����
	void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual ~OnCharCommand();//�Ҹ���
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	UINT GetNChar() const;//����� ����� ���� ���ϱ�
	inline Long GetStartSplitIndex() const;//����� ����� ���ڰ� ���๮���� ��� split�Ǵ� ĭ�� ��ġ���ϱ�
private:
	UINT nChar;//OnCharCommand�� ������ �� �Էµ� ����
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Long startSplitIndex;//���๮���̸� split�Ǵ� ĭ�� ��ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
};

//�ζ����Լ� ����
inline UINT OnCharCommand::GetNChar() const
{
	return this->nChar;
}
//�и��� ���� ��ġ���ϱ�
inline Long OnCharCommand::GetStartSplitIndex() const
{
	return this->letterIndex;
}


#endif // !_ONCHARCOMMAND_H