#ifndef _REMOVECOMMAND_H
#define _REMOVECOMMAND_H

#include "Command.h"

class Glyph;
class NotepadForm;

class RemoveCommand :public Command
{
public:
	RemoveCommand(NotepadForm* notepadForm = 0);
	virtual void Execute();
	virtual void Unexecute();
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ����� ������ ĭ�� ��ġ�� ���ϱ�
	virtual Long GetSelectedStartXPos();//����� ����� ������ ���۵Ǵ� ������ ��ġ���ϱ�
	virtual Long GetSelectedStartYPos();//����� ����� ������ ���۵Ǵ� ���� ��ġ���ϱ�
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual ~RemoveCommand();//�Ҹ���
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	inline Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
private:
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isRedone;//�ٽý������� �ƴ��� ����
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	Long selectedStartXPos;//������ ���۵Ǵ� ������ġ
	Long selectedStartYPos;//������ ����Ǵ� ������ġ
};

//�ζ����Լ� ����
inline Glyph* RemoveCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_REMOVECOMMAND_H