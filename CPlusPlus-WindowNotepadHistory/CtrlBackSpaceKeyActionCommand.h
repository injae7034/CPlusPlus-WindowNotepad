#ifndef _CTRLBACKSPACEKEYACTIONCOMMAND_H
#define _CTRLBACKSPACEKEYACTIONCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class CtrlBackSpaceKeyActionCommand :public Command
{
public:
	CtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsDirty();//��������� �ִ��� Ȯ�� ����
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	virtual ~CtrlBackSpaceKeyActionCommand();//�Ҹ���
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
private:
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isDirty;//��������� �ִ��� ����
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* CtrlBackSpaceKeyActionCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_CTRLBACKSPACEKEYACTIONCOMMAND_H

