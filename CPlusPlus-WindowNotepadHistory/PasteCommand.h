#ifndef _PASTECOMMAND_H
#define _PASTECOMMAND_H

#include "Command.h"

class NotepadForm;
class Glyph;

class PasteCommand :public Command
{
public:
	PasteCommand(NotepadForm* notepadForm = 0);
	void Execute();
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual Long GetStartRowIndex();
	virtual Long GetStartLetterIndex();
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
	~PasteCommand();//�Ҹ���
private:
	Long startRowIndex;//�Էµ� ���۵� ���� ��ġ
	Long startLetterIndex;//�Է��� ���۵� ������ ��ġ
	Long rowIndex;//�Էµ� ������ ����ġ(�Է��� ������ ���� ��ġ)
	Long letterIndex;//�Էµ� ������ ĭ��ġ(�Է��� ������ ������ ��ġ)
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* PasteCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_PASTECOMMAND_H