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
	void SetUndoMacroEnd();//���������� �������� ����
	void SetRedoMacroEnd();//�ٽý������ �������� ����
	void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual ~CtrlBackSpaceKeyActionCommand();//�Ҹ���
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	inline Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
private:
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
};

//�ζ����Լ� ����
inline Glyph* CtrlBackSpaceKeyActionCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_CTRLBACKSPACEKEYACTIONCOMMAND_H

